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
#include <unordered_set>
#include <vector>

#include "brain/neurotransmitter.hpp"
#include "brain/receptor.hpp"
#include "math/quaternion.hpp"
#include "math/vector3.hpp"

namespace cort {

class connection;

class neuron
{
public:
    neuron();
    ~neuron();

    std::size_t get_id() const { return id_; }
    
    void update(double time);
    void augment_voltage(double amount);
    void add_input(connection *conn);
    void add_output(connection *conn);
    void update_inputs(double time);
    void activate_receptor(neurotransmitter_type neurotrans);
    void backpropagate();
    
    double get_last_fired() { return last_fired_; }
    
    std::vector<neurotransmitter_type> get_emitted_neurotransmitters() { return emitted_neurotransmitters_; }
    std::vector<neurotransmitter_type> get_received_neurotransmitters() { return received_neurotransmitters_; }
    
    std::vector<connection *> &get_inputs() { return inputs_; }
    std::vector<connection *> &get_outputs() { return outputs_; }

    void set_voltage(double v) { v_ = v; }
    double get_voltage() { return v_; }

    vector3 get_local_position() const { return local_position_; }
    void set_local_position(vector3 position) { local_position_ = position; }
    void update_world_position(vector3 parent_position, const quaternion &parent_rotation);
    vector3 get_world_position() const { return world_position_; }

    void set_external_current(double current) { external_current_ = current; }

    void oscillate(double period) { is_oscillator_ = true; oscillation_period_ = period; }

    void add_class(const std::string &to_add) { class_.insert(to_add); }

    void clear_class() { class_.clear(); }

    bool has_class(const std::string &to_check) { return class_.count(to_check) == 1; }

    void set_label(const std::string &label) { label_ = label; }

    std::string get_label() const { return label_; }
    
private:
    friend class bci_speech_from_file;

    static std::size_t num_neurons_;
    
    void on_action_potential_begin();
    void on_action_potential_end();

    std::size_t id_;
    
    std::vector<neurotransmitter_type> emitted_neurotransmitters_;
    std::vector<neurotransmitter_type> received_neurotransmitters_;
    
    double current_time_;
    double delta_time_;
    double last_time_;
    double backpropagation_time_;
    
    std::vector<connection *> inputs_;
    std::vector<connection *> outputs_;
    
    double input_current_;
    double cm_;
    double v_;
    double percent_na_channels_;
    double percent_k_channels_;
    double gna_, gk_, gl_;
    double vna_, vk_, vl_;
    double n_, m_, h_;
    double activation_voltage_;
    bool firing_;
    bool backpropagating_;
    double last_fired_;
    double external_current_;

    bool is_oscillator_;
    double oscillation_period_;

    vector3 local_position_;
    vector3 world_position_;

    std::unordered_set<std::string> class_;
    std::string label_;
};

} // namespace cort
