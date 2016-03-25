cmake_minimum_required(VERSION 3.4.3)

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

option(DEBUG "Set to ON to for debug configuration" OFF)
option(ASMJS "Set to OFF to skip compiling ${PROJECT_NAME} as an emscripten library to be run in the browser" ON)
option(SERVER "Set to ON to compile a standalone executable which acts as a server" OFF)

if(ASMJS)
    set(CMAKE_CXX_COMPILER em++)
    set(CMAKE_C_COMPILER emcc)
    include_directories(SYSTEM /usr/local/include)
    set(CMAKE_CXX_CREATE_STATIC_LIBRARY "em++ <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
endif()


project(cort)


if(ASMJS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-warn-absolute-paths")
endif()

if(CMAKE_CONFIGURATION_TYPES)
    if(DEBUG)
        set(CMAKE_BUILD_TYPE "Debug")
    else()
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

if(APPLE)
    execute_process(COMMAND "sw_vers -productVersion | awk -F'.' '{print $1\".\"$2}'"
                    OUTPUT_VARIABLE OSX_VERSION)
    set(CMAKE_OSX_DEPLOYMENT_TARGET ${OSX_VERSION})
endif(APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PROJECT_VENDOR "Thomas Fussell")
set(PROJECT_CONTACT "thomas.fussell@gmail.com")
set(PROJECT_URL "https://github.com/tfussell/cort")
set(PROJECT_DESCRIPTION "A biological neural network simulation platform")

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "9")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(PROJECT_VERSION_FULL "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(LIBRARY_VERSION ${PROJECT_VERSION_FULL})

include_directories(source)

file(GLOB BRAIN_SOURCES source/brain/*.*pp)
source_group(brain FILES ${BRAIN_SOURCES})
file(GLOB INTERFACE_SOURCES source/interface/*.*pp)
source_group(interface FILES ${INTERFACE_SOURCES})
file(GLOB MATH_SOURCES source/math/*.*pp)
source_group(math FILES ${MATH_SOURCES})
file(GLOB SIMULATION_SOURCES source/simulation/*.*pp)
source_group(simulation FILES ${SIMULATION_SOURCES})

set(SOURCES ${BRAIN_SOURCES} ${INTERFACE_SOURCES} ${MATH_SOURCES} ${SIMULATION_SOURCES})

if(SERVER)
    file(GLOB SERVER_SOURCES source/server/*.*pp)
    source_group(server FILES ${SERVER_SOURCES})

    add_executable(cort ${SOURCES} ${SERVER_SOURCES})
    target_link_libraries(cort boost_system)

    add_custom_command(TARGET cort
                       POST_BUILD
                       COMMAND cp cort ../viewer
                       COMMAND cd ../viewer && ./cort
 	               DEPENDS cort)
else()
    file(GLOB EMBIND_SOURCES source/embind/*.*pp)
    source_group(embind FILES ${EMBIND_SOURCES})

    add_library(cort.js STATIC ${SOURCES} ${EMBIND_SOURCES})

    set_target_properties(cort.js PROPERTIES PREFIX "")
    set_target_properties(cort.js PROPERTIES SUFFIX ".bc")

    add_custom_command(TARGET cort.js
                       POST_BUILD
                       COMMAND emcc cort.js.bc -o cort.js --bind
                       COMMAND cp cort.js ../viewer/js/cort.js
                       COMMAND cd ../viewer && python3 -m http.server
 		       DEPENDS cort.js.bc)
endif()
