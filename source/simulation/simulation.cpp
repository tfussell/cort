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
#include <iostream>

#include "simulation/simulation.hpp"
#include "brain/connection.hpp"
#include "simulation/entity.hpp"
#include "simulation/timer.hpp"

namespace cort {

simulation::simulation(double timeToSimulate, double stepsPerRealSec, double stepsPerSimSec) :
	current_step_(0),
	end_step_(static_cast<unsigned int>(timeToSimulate * stepsPerSimSec)),
	real_seconds_per_step_(1 / stepsPerRealSec),
	steps_per_real_second_(stepsPerRealSec),
	sim_seconds_per_step_(1 / stepsPerSimSec),
	steps_per_sim_second_(stepsPerSimSec),
	sim_time_(timeToSimulate)
{

}

simulation::~simulation()
{

}

void simulation::start()
{
	std::cout << "simulation started" << std::endl;

	timer_.reset();
	double currentTime = timer_.get_time_in_seconds();
	double startTime = currentTime;

	step();

	while(current_step_ < end_step_)
	{
		while(timer_.get_time_in_seconds() - currentTime < real_seconds_per_step_) 
		{
		}

		currentTime = timer_.get_time_in_seconds();
		std::cout << current_step_ << std::endl;

		step();
	}

    std::printf("Final simulation rate: %fs(sim)/s(real)\n", sim_time_ / (currentTime - startTime));
}

void simulation::stop()
{

}

void simulation::step()
{
	for(auto entity : entities_)
	{
		entity->update(current_step_ * sim_seconds_per_step_);
	}

	current_step_++;
}

void simulation::reset()
{
	for(auto entity : entities_)
	{
		entity->reset();
	}
}

std::string simulation::serialize_entity_structure(std::size_t entity_index)
{
    return entities_[entity_index]->serialize_structure();
}

std::string simulation::serialize_entity_state(std::size_t entity_index)
{
    return entities_[entity_index]->serialize_state();
}

} // namespace cort
