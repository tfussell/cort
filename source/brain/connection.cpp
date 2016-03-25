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
#include "brain/connection.hpp"

namespace cort {

std::size_t connection::num_signals_ = 0;
const double connection::transmission_velocity_ = 0.1;

connection::connection(neuron *presynaptic, neuron *postsynaptic, double receptor_current) : 
	length_(1),
	presynaptic_(presynaptic), 
	postsynaptic_(postsynaptic),
	synapse_(presynaptic, postsynaptic, receptor_current)
{
	if(presynaptic_ != nullptr)
	{
		presynaptic_->add_output(this);
	}

	if(postsynaptic_ != nullptr)
	{
		postsynaptic_->add_input(this);
	}

	if(presynaptic_ && postsynaptic_)
	{
	    auto pre_pos = presynaptic_->get_world_position();
	    auto post_pos = postsynaptic_->get_world_position();
	    auto dist = (pre_pos - post_pos).length();
	    length_ = dist;
	}
}

connection::~connection() {}

bool connection::has_updates() const
{
	return !signals_.empty() || has_deleted_signal() || has_new_signal();
}

void connection::update(double time) 
{ 
	auto iter = signals_.begin();
	bool activate = false;

	while(iter != signals_.end())
	{
		auto &signal = *iter;
		signal.second.first += transmission_velocity_;

		if(signal.second.first > length_)
		{
  		    if(signal.second.second == 0) 
			{
			    activate = true;
			}

			iter = signals_.erase(iter);

			if(new_signals_.find(signal.first) != new_signals_.end())
			{
				// signal hasn't been popped from new list since it was created, so we don't care if it was deleted
				new_signals_.erase(signal.first);
			}
			else
			{
				deleted_signals_[signal.first] = signal.second;
			}
		}
		else
		{
			iter++;
		}
	}

	if(activate)
	{
		synapse_.activate();
	}

	synapse_.update(time); 
}

void connection::send_signal(int type)
{
    signal_type signal = {0, type};
	signals_.push_front({next_signal(), signal});
	new_signals_[signals_.front().first] = signal;
}

connection::signal_id_pair connection::pop_deleted_signal()
{
	auto front = *deleted_signals_.begin(); 
	deleted_signals_.erase(deleted_signals_.begin());
    
	return front;
}

connection::signal_id_pair connection::pop_new_signal()
{ 
	auto front = *new_signals_.begin(); 
	new_signals_.erase(new_signals_.begin());
    
	return front;
}

} // namespace cort
