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
#pragma once
#include <vector>

#include "brain/brain.hpp"
#include "simulation/trainer.hpp"
#include "simulation/timer.hpp"

namespace cort {

class entity;

class simulation
{
 public:
  simulation(double timeToSimulate, double stepsPerRealSec, double stepsPerSimSec);
  ~simulation();

  void start();
  void stop();
  void reset();
  void step();

  std::string serialize_entity_structure(std::size_t entity_index);
  std::string serialize_entity_state(std::size_t entity_index);

  void add_entity(entity &ent) { entities_.push_back(&ent); }

  double get_percent_completion() { return ((double)current_step_) / end_step_; }
  unsigned int get_current_step() { return current_step_; }
  double get_total_sim_time() { return current_step_ * sim_seconds_per_step_; }
  
  void set_real_seconds_per_step(double seconds) 
  { 
    real_seconds_per_step_ = seconds;
    steps_per_real_second_ = 1 / seconds;
  }
  double get_real_seconds_per_step() { return real_seconds_per_step_; }

  void set_steps_per_real_second(double steps) 
  { 
    steps_per_real_second_ = steps; 
    real_seconds_per_step_ = 1 / steps;
  }
  double get_steps_per_real_second() { return steps_per_real_second_; }

  void set_sim_seconds_per_step(double seconds) 
  { 
    sim_seconds_per_step_ = seconds;
    steps_per_sim_second_ = 1 / seconds;
  }
  double get_sim_seconds_per_step() { return sim_seconds_per_step_; }

  void set_steps_per_sim_second(double steps) 
  { 
    steps_per_sim_second_ = steps; 
    sim_seconds_per_step_ = 1 / steps;
  }
  double get_steps_per_sim_second() { return steps_per_sim_second_; }

 private:
  std::vector<entity *> entities_;
  int current_step_;
  int end_step_;
  double real_seconds_per_step_;
  double steps_per_real_second_;
  double sim_seconds_per_step_;
  double steps_per_sim_second_;
  double sim_time_;
  timer timer_;
};

} // namespace cort
