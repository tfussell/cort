/*
The MIT License (MIT)

Copyright (c) 2006-2016 Thomas Fussell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <emscripten/bind.h>

#include "embind/cort_api.hpp"
#include "brain/connection.hpp"

using namespace emscripten;

cort_api::cort_api() :
	simulation_(2000, 50000000, 100000)
{
	 simulation_.add_entity(brain_);
}

void cort_api::load_script(std::string contents)
{
	brain_.load(contents);
}

std::string cort_api::get_structure()
{
    return brain_.serialize_structure();
}

std::string cort_api::get_state()
{
    return brain_.serialize_state();
}

void cort_api::step()
{
    simulation_.step();
}

double cort_api::get_sim_time()
{
    return simulation_.get_total_sim_time();
}

void cort_api::set_input_image(std::string image_string)
{
    brain_.set_input_image(image_string);
}

void cort_api::train(std::string image_string, std::string expected_output)
{
    brain_.train(image_string, expected_output);
}

EMSCRIPTEN_BINDINGS(cort_bindings) {
	class_<cort_api>("cort_api")
		.constructor<>()
		.function("load_script", &cort_api::load_script)
		.function("step", &cort_api::step)
		.function("set_input_image", &cort_api::set_input_image)
		.function("train", &cort_api::train)
		.function("get_sim_time", &cort_api::get_sim_time)
		.function("get_structure", &cort_api::get_structure)
		.function("get_state", &cort_api::get_state);
}
