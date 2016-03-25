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
#include "brain/emitter.hpp"

namespace cort {

emitter::emitter() :
	open_(false),
	duration_(0.0),
	//type_(neurotransmitter_type::invalid),
	time_last_opened_(-(std::numeric_limits<double>::max)()),
	//max_amount_(0.0),
	//regen_rate_(0.0),
	amount_released_(0)
{

}

emitter::emitter(neurotransmitter_type /*type*/, double duration, double amountPerOpening, double /*maxAmount*/, double /*regenRate*/) :
	open_(false),
	duration_(duration),
	//type_(type),
	time_last_opened_(-(std::numeric_limits<double>::max)()),
	//max_amount_(maxAmount),
	//regen_rate_(regenRate),
	amount_released_(amountPerOpening)
{

}

void emitter::open(double time)
{
	time_last_opened_ = time;
}

void emitter::update(double time)
{
	if(time - time_last_opened_ >= duration_)
	{
		open_ = false;
	}
}

bool emitter::is_open()
{
	return open_;
}

void emitter::change_weight(double amount)
{
	amount_released_ += amount;
}

} // namespace cort
