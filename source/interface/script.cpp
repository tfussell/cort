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
#include "interface/script.hpp"

namespace cort {

script::script(const std::string &filename) :
	filename_(filename)
{
	std::string fullpath = "../scripts/" + filename_;
	stream_.open(fullpath, std::ifstream::in);

	if(!stream_.is_open())
	{
		throw std::runtime_error("Error opening file in __func__(__LINE__) in __FILE__: " + filename_);
	}
}

script::~script()
{
	stream_.close();
}

std::string script::get_next_string()
{
	std::string ret("");
	stream_ >> ret;
	return ret;
}

int script::get_next_int()
{
	int ret = 0;
	stream_ >> ret;
	return ret;
}

double script::get_next_double()
{
	double ret = 0;
	stream_ >> ret;
	return ret;
}

bool script::is_finished()
{
	return !stream_.good();
}

} // namespace cort
