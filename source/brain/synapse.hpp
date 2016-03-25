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
#include <deque>
#include <map>
#include <vector>

#include "brain/emitter.hpp"
#include "brain/neurotransmitter.hpp"
#include "brain/receptor.hpp"

namespace cort {

class neuron;

class synapse
{
public:
    synapse(neuron *presynaptic, neuron *postsynaptic, double receptor_current);
    ~synapse();

    void activate();
    void update(double time);
    double get_total_current();

    void backpropagate_up();
    void backpropagate_down();

    neuron *get_presynaptic_neuron() const { return presynaptic_; }
    neuron *get_postsynaptic_neuron() const { return postsynaptic_; }

private:
    friend class bci_speech_from_file;
    
    std::vector<neurotransmitter_type> emitted_;
    std::vector<neurotransmitter_type> received_;
    
    std::map<neurotransmitter_type, emitter> emitters_;
    std::map<neurotransmitter_type, receptor> receptors_;

    double current_time_;
    double delta_time_;
    double activation_duration_;
    double time_last_activated_;
    double total_current_;
    
    neuron *presynaptic_;
    neuron *postsynaptic_;
    
    bool hebbian_;
};

} // namespace cort
