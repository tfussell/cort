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
#include <cmath>
#include <algorithm>

#include "brain/synapse.hpp"
#include "brain/neuron.hpp"
#include "simulation/random_number_generator.hpp"

namespace {

template<typename T>
std::vector<T> intersect(const std::vector<T> &left, const std::vector<T> &right)
{
	std::vector<T> intersection;

	for(const auto &i : left)
	{
		if(std::find(right.begin(), right.end(), i) != right.end())
		{
			intersection.push_back(i);
		}
	}

	return intersection;
}

}

namespace cort {

synapse::synapse(neuron *pre, neuron *post, double receptor_current) :
    current_time_(0),
    delta_time_(1),
    activation_duration_(0.001),
    time_last_activated_(0),
    presynaptic_(pre),
    postsynaptic_(post),
    hebbian_(true)
{
    double duration = 0.001;
    
    if(pre != nullptr && post == nullptr)
    {
	for(auto nt : pre->get_emitted_neurotransmitters())
	{
	    emitters_[nt] = emitter(nt, 0.1, 1.0, 10.0, 2.0);
	}
    }
    
    if(post != nullptr && pre == nullptr)
    {
	for(auto nt : post->get_received_neurotransmitters())
	{
	    receptors_[nt] = receptor(nt, duration, receptor_current);
	}
    }
    
    // ignore signals that are emitted but not received and vice versa
    if(post != nullptr && pre != nullptr)
    {
	auto intersection = intersect(pre->get_emitted_neurotransmitters(), 
	    post->get_received_neurotransmitters());
	
	emitted_ = intersection;
	received_ = intersection;
	
	for(auto nt : intersection)
	{
	    emitters_[nt] = emitter(nt, 0.01, 1, 10.0, 2.0);
	    receptors_[nt] = receptor(nt, duration, receptor_current);
	}
    }
}
    
synapse::~synapse()
{
}

void synapse::update(double time)
{
    double lastTime = current_time_;   
    current_time_ = time;
    delta_time_ = current_time_ - lastTime;
	
	for(std::size_t i = 0; i < emitted_.size(); i++)
	{
		emitters_[emitted_[i]].update(time);
	}

	for(std::size_t i = 0; i < received_.size(); i++)
	{
		receptors_[received_[i]].update(time);
	}
}

void synapse::activate()
{
    for(std::size_t i = 0; i < received_.size(); i++)
    {
		receptors_[received_[i]].open(current_time_);
    }
}

void synapse::backpropagate_up()
{
	if(!postsynaptic_ || !presynaptic_)
	{
		return;
	}

	double timeDiff = postsynaptic_->get_last_fired() - presynaptic_->get_last_fired();
    double weightChange = 0.5;//0.8 * std::exp(-timeDiff);

	for(std::size_t i = 0; i < emitted_.size(); i++)
	{
		emitters_[emitted_[i]].change_weight(weightChange);
	}

	for(std::size_t i = 0; i < received_.size(); i++)
	{
		receptors_[received_[i]].change_weight(weightChange);
	}

	//presynaptic_->backpropagate_up();
}

void synapse::backpropagate_down()
{
	if(!postsynaptic_ || !presynaptic_)
	{
		return;
	}

	double timeDiff = postsynaptic_->get_last_fired() - presynaptic_->get_last_fired();
    double weightChange = -0.5;//-0.5 * std::exp(-timeDiff);

	for(std::size_t i = 0; i < emitted_.size(); i++)
	{
		emitters_[emitted_[i]].change_weight(weightChange);
	}

	for(std::size_t i = 0; i < received_.size(); i++)
	{
		receptors_[received_[i]].change_weight(weightChange);
	}

	//presynaptic_->backpropagate_down();
}

/*
void Synapse::activate_receptor(NeurotransmitterType receptorType)
{
	if(m_Receptors.find(receptorType) != m_Receptors.end())
	{
		m_Receptors[receptorType].Open(m_CurrentTime);
	}
}
*/

double synapse::get_total_current()
{
	total_current_ = 0.;

	for(auto i = receptors_.begin(); i != receptors_.end(); i++)
	{
		if(i->second.is_open())
		{
			total_current_ += i->second.get_current_change();
		}
	}

	return total_current_;
}

} // namespace cort
