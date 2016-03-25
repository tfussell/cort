<img height="100" src="http://tfussell.github.io/cort/images/cort.png"><br/>
====
cort is a platform for simulating, exploring, evolving, and interacting with biologically-inspired neural networks. It is written in C++, but can be compiled into JavaScript using Emscripten for easy cross-platform deployment.

To try cort, click on [this demo](http://tfussell.github.io/cort/demo).

Build
=====
This repository includes the latest emscripten compiled version of cort in bin/cort.html. If you'd like to compile cort yourself, make sure you have a C++ compiler that supports C++11, an updated browser, and emscripten.

1. Copy repository

        git clone https://github.com/tfussell/cort
2. Compile using premake and platform build system

        cd cort/build && ./build-emscripten.sh
3. Run/view the simulation

        open cort/bin/cort.html

License
=======
cort is licenced under the MIT license.
cort uses a modified version of [nxxcxx's Neural-Network](https://github.com/nxxcxx/Neural-Network) for visualization which is in turn based on [three.js](http://threejs.org). Both of these are also licensed under the MIT license.
