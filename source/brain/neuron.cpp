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
#include <cstdio>
#include <cmath>

#include "brain/neuron.hpp"
#include "brain/connection.hpp"
#include "simulation/random_number_generator.hpp"

namespace cort {

/*
 * Translated from: http://www.afodor.net/HHModel.htm
 */

std::size_t neuron::num_neurons_ = 0;

neuron::neuron() :
    id_(num_neurons_++),
    current_time_(0),
    input_current_(0),
    backpropagation_time_(0),
    cm_(1.0), //Membrane capaciatance in uF/cm^2
    v_(-70.0), //Initial membrane potential
    percent_na_channels_(100.0),
    percent_k_channels_(100.0),
    gna_(percent_na_channels_ * 120 / 100), //Maximum conductances in mS/cm^2
    gk_(percent_k_channels_ * 36 / 100),
    gl_(0.3),
    vna_(45.0), //Nernst reversal potentials in mV
    vk_(-82.0),
    vl_(-59.387),
    activation_voltage_(-20.0),
    firing_(false),
    backpropagating_(false),
    last_fired_(-100),
    external_current_(0),
    is_oscillator_(false),
    oscillation_period_(0)
{		
    double alphaH = 0.07 * std::exp(-1 * (v_ + 70.0) / 20.0);
    double betaH = 1.0 / (1.0 + std::exp(-1 * (v_ + 40.0) / 10.0));
    h_ = alphaH / (alphaH + betaH);
    
    double alphaM = 0.1 * (v_ + 45.0) / (1.0 - std::exp(-1 * (v_ + 45.0) / 10.0));
    double betaM = 4.0 * std::exp(-1 * (v_ + 70.0) / 18.0);
    m_ = alphaM / (alphaM + betaM);
    
    double alphaN = 0.01 * (v_ + 60) / (1.0 - std::exp(-1 * (v_ + 60.0) / 10.0));
    double betaN = 0.125 * std::exp(-1 * (v_ + 70.0) / 80.0);
    n_ = alphaN / (alphaN + betaN);
    
    received_neurotransmitters_.push_back(neurotransmitter_type::glutamate);
    emitted_neurotransmitters_.push_back(neurotransmitter_type::glutamate);
}
    
neuron::~neuron()
{
    
}

void neuron::update(double time)
{
    double last_time = current_time_;
    
    current_time_ = time;
    delta_time_ = current_time_ - last_time;

    if(is_oscillator_)
    {
	if(!firing_ && current_time_ - last_fired_ > oscillation_period_)
	{
	    on_action_potential_begin();
	}
	else if(firing_)
	{
	    on_action_potential_end();
	}

	return;
    }
    
    double alphaH = 0.07 * std::exp(-1 * (v_ + 70.0) / 20.0);
    double betaH = 1.0 / (1.0 + std::exp(-1 * (v_ + 40.0) / 10.0));
    double deltaH = (alphaH * (1 - h_) - betaH * h_) * delta_time_ * 1000;
    
    double alphaM = 0.1 * (v_ + 45.0) / (1.0 - std::exp(-1 * (v_ + 45.0) / 10.0));
    double betaM = 4.0 * std::exp(-1 * (v_ + 70.0) / 18.0);
    double deltaM = (alphaM * (1 - m_) - betaM * m_) * delta_time_ * 1000;
    
    double alphaN = 0.01 * (v_ + 60) / (1.0 - std::exp(-1 * (v_ + 60.0) / 10.0));
    double betaN = 0.125 * std::exp(-1 * (v_ + 70.0) / 80.0);
    double deltaN = (alphaN * (1 - n_) - betaN * n_) * delta_time_ * 1000;	
    
    double naCurrent = gna_ * (v_ - vna_) * m_ * m_ * m_ * h_;
    double kCurrent = gk_ * (v_ - vk_) * n_ * n_ * n_ * n_;
    double lCurrent = gl_ * (v_ - vl_);
    
    double deltaV = ((input_current_ - kCurrent - naCurrent - lCurrent) / cm_) * delta_time_ * 1000;
    
    v_ += deltaV;
    h_ += deltaH;
    m_ += deltaM;
    n_ += deltaN;
    
    if(!firing_ && v_ >= activation_voltage_)
    {
	   on_action_potential_begin();
    }
    else if(firing_ && v_ < activation_voltage_)
    {
    	on_action_potential_end();
    }

    if(backpropagating_ && current_time_ - backpropagation_time_ > 0.001)
    {
        backpropagating_ = false;
    }
}
    
void neuron::on_action_potential_begin()
{
    firing_ = true;
    last_fired_ = current_time_;
    
    for(auto connection : outputs_)
    {
	    connection->send_signal(0);
    }

    if(has_class("expected_fire") || has_class("expected_no_fire"))
    {
        std::cout << get_label() << " " << has_class("expected_fire") << " " << has_class("expected_no_fire") << std::endl;

        for(auto in_connection : inputs_)
        {
            for(auto sibling_connection : in_connection->get_presynaptic()->outputs_)
            {
                sibling_connection->get_postsynaptic()->backpropagate();
            }
        }
    }
}
    
void neuron::on_action_potential_end()
{
    firing_ = false;
}

void neuron::backpropagate()
{
    if(backpropagating_) return;

    bool fired_recently = std::abs(current_time_ - last_fired_) < 0.003;
    bool up = false;
    bool down = false;

    if(has_class("expected_fire") && !fired_recently)
    {
        std::cout << get_label() << " should have fired, didn't " << current_time_ << " " << last_fired_ << " " << (current_time_ - last_fired_) << std::endl;
        up = true;
    }
    else if(has_class("expected_no_fire") && fired_recently)
    {
        std::cout << get_label() << " shouldn't have fired, did " << current_time_ << " " << last_fired_ << " " << (current_time_ - last_fired_) << std::endl;
        down = true;
    }

    if(!up && !down)
    {
        std::cout << get_label() << " no change" << current_time_ << " " << last_fired_ << " " << (current_time_ - last_fired_) << std::endl;
        return;
    }

    backpropagating_ = true;
    backpropagation_time_ = current_time_;

    for(auto connection : inputs_)
    {
	if(connection->get_presynaptic()->has_class("collector")) continue;

	if(current_time_ - connection->get_presynaptic()->last_fired_ < 0.005)
	{
	    if(up)
	    {
		connection->get_synapse().backpropagate_up();
		connection->send_signal(1);
	    }
	    else
	    {
		connection->get_synapse().backpropagate_down();
		connection->send_signal(2);
	    }
	}
    }
}

void neuron::add_input(connection *conn)
{
    inputs_.push_back(conn);
}

void neuron::add_output(connection *conn)
{
    outputs_.push_back(conn);
}

void neuron::update_inputs(double time)
{
    input_current_ = external_current_;

    for(auto connection : inputs_)
    {
        connection->update(time);
        input_current_ += connection->get_synapse().get_total_current();
    }
}

void neuron::update_world_position(vector3 parent_position, const quaternion &parent_rotation)
{
    world_position_ = (parent_rotation * local_position_) + parent_position;
}

} // namespace cort
