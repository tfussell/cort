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
#include <list>
#include <memory>
#include <unordered_map>
#include <utility>

#include "neuron.hpp"
#include "synapse.hpp"

namespace cort {

class connection
{
public:
    using signal_type = std::pair<double, int>;
    using signal_id_pair = std::pair<std::size_t, signal_type>;
    using signal_container = std::list<signal_id_pair>;
    using signal_assoc_container = std::unordered_map<std::size_t, signal_type>;
    
    static std::size_t next_signal() { return num_signals_++; }

    connection(neuron *presynaptic, neuron *postsynaptic, double receptor_current);
    ~connection();

    bool has_updates() const;

    double get_length() const { return length_; }

    void update(double time);
    void send_signal(int type);
    synapse &get_synapse() { return synapse_; }

    neuron *get_presynaptic() const { return presynaptic_; }
    neuron *get_postsynaptic() const { return postsynaptic_; }

    bool has_deleted_signal() const { return !deleted_signals_.empty(); }
    signal_id_pair pop_deleted_signal();
    bool has_new_signal() const { return !new_signals_.empty(); }
    signal_id_pair pop_new_signal();

    signal_container::iterator begin() { return signals_.begin(); }
    signal_container::const_iterator begin() const { return signals_.cbegin(); }
    signal_container::iterator end() { return signals_.end(); }
    signal_container::const_iterator end() const { return signals_.cend(); }

private:
    static const double transmission_velocity_;
    static std::size_t num_signals_;

    double length_;
    signal_container signals_;
    signal_assoc_container new_signals_;
    signal_assoc_container deleted_signals_;

    neuron *presynaptic_;
    neuron *postsynaptic_;

    synapse synapse_;
};

} // namespace cort
