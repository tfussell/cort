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
#include <iostream>

#include "brain/receptor.hpp"

namespace cort {

receptor::receptor() :
	open_(false),
	duration_(0),
	current_change_per_molecule_(0),
	time_last_opened_(-(std::numeric_limits<double>::max)())
{

}

receptor::receptor(neurotransmitter_type /*type*/, double duration, double currentChange) :
	open_(false),
	duration_(duration),
	current_change_per_molecule_(currentChange),
	time_last_opened_(-1 * (duration + 0.01)) //can't be zero as it would make it appear to be open, can't be very negative as that would be large delta-time
{

}

receptor::~receptor()
{
	
}

void receptor::open(double currentTime)
{
	time_last_opened_ = currentTime;
	open_ = true;
}

void receptor::update(double currentTime)
{
	if(open_ && currentTime - time_last_opened_ >= duration_)
	{
		open_ = false;
	}
}

bool receptor::is_open()
{
	return open_;
}

double receptor::get_current_change()
{
	return current_change_per_molecule_;
}

void receptor::change_weight(double amount)
{
    std::cout << "receptor changed from " << current_change_per_molecule_ << " to " << current_change_per_molecule_ + amount << std::endl;
	current_change_per_molecule_ += amount;
}

} // namespace cort
