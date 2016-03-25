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
#include <string>

#include "interface/bci_speech_from_file.hpp"
#include "interface/bci_reward_punishment.hpp"
#include "interface/script.hpp"
#include "simulation/entity.hpp"

namespace cort {

class brain;

enum training_state
{
	uninitialized,
	initialized,
	teach_start_message,
	teach_set_input,
	teach_set_output,
	teach_end_message,
	test_start_message,
	test_set_input,
	test_attend_output,
	test_end_message,
	completed
};

class trainer : public entity
{
public:
	trainer();
	trainer(const std::string &scriptFilename, brain *brain);
	~trainer();

	bool is_completed();
	void update(double time);
	void reset();
	void print_state();
	layer *get_layer(const std::string &inter);
	
private:
	void wait(double duration);

	double current_time_;
	double delta_time_;
	double wait_start_;
	double wait_duration_;
	bool waiting_;
	
	int repeat_;
	int num_repeats_;

	double time_between_start_message_and_input_;
	double time_to_wait_for_test_response_;
	double time_between_output_and_end_;

	double time_between_train_in_and_out_;
	double time_between_train_iterations_;
	double time_between_test_iterations_;

	int total_training_iterations_;
	int total_testing_iterations_;

	int training_iteration_;
	int testing_iteration_;

	std::string input_;
	std::string expected_output_;

	bci_reward_punishment *bci_reward_;
	bci_speech_from_file *bci_input_;
	bci_speech_from_file *bci_output_;
	bci_speech_from_file *bci_output_trainer_;

	script script_;
	training_state state_;
};

} // namespace cort
