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
#include <iostream>
#include <json.hpp>

#include "brain/brain.hpp"
#include "brain/connection.hpp"
#include "brain/layer_factory.hpp"
#include "simulation/timer.hpp"

namespace cort {

brain::brain() :
    current_time_(0)
{
}

brain::~brain()
{
}

void brain::load(const std::string &json_string)
{
    auto json = nlohmann::json::parse(json_string);

    for(auto &layer : json["layers"])
    {
	   add_layer(layer);
    }

    for(auto &connection : json["connections"])
    {
	   add_connection(connection);
    }
}

void brain::add_layer(const nlohmann::json &settings)
{
    std::string name = settings["name"];
    layers_[name] = layer_factory::create_layer(settings);
}

void brain::update(double time)
{
    current_time_ = time;

    for(auto &layer : layers_)
    {
        for(auto &neuron : *layer.second.get())
        {
            neuron->update_inputs(current_time_);
        }
    }

    for(auto &layer : layers_)
    {
        for(auto &neuron : *layer.second.get())
        {
            neuron->update(current_time_);
        }
    }
}

void brain::reset()
{
    
}

void brain::add_connection(const nlohmann::json &settings)
{
    std::string from = settings["from"];
    std::string to = settings["to"];
    get_layer(from)->connect_to(*get_layer(to), settings);
}

std::string brain::serialize_structure() const
{
    auto structure = nlohmann::json::object();
    auto &neurons = structure["neurons"];
    neurons = nlohmann::json::object();

    for(auto &layer : layers_)
    {
	for(auto &neuron : *layer.second.get())
	{
	    auto neuron_id = std::to_string(neuron->get_id());
	    auto &neuron_obj = neurons[neuron_id] = nlohmann::json::object();
	    auto position = neuron->get_world_position();
	    neuron_obj["position"] = nlohmann::json::array({position.x, position.y, position.z});
	    auto &connections = neuron_obj["connections"];
	    connections = nlohmann::json::object();

	    for(auto connection : neuron->get_outputs())
	    {
		auto postsynaptic = connection->get_postsynaptic();
		auto postsynaptic_id = std::to_string(postsynaptic->get_id());
		auto &connection_obj = connections[postsynaptic_id];
		connection_obj = nlohmann::json::object();
		connection_obj["weight"] = 0;
	    }
	}
    }

    return structure.dump();
}

std::string brain::serialize_state() const
{
    auto state = nlohmann::json::object();
    auto &neurons = state["neurons"];
    neurons = nlohmann::json::object();

    for(auto &layer : layers_)
    {
        for(auto &neuron : *layer.second.get())
        {
            auto neuron_id = std::to_string(neuron->get_id());
            auto &neuron_obj = neurons[neuron_id] = nlohmann::json::object();
            neuron_obj["voltage"] = neuron->get_voltage();
            auto &signals = neuron_obj["signals"] = nlohmann::json::object();
            auto &new_signals = neuron_obj["new_signals"] = nlohmann::json::object();
            auto &deleted_signals = neuron_obj["deleted_signals"] = nlohmann::json::object();

            for(auto connection : neuron->get_outputs())
            {
                while(connection->has_new_signal())
                {
                    auto signal = connection->pop_new_signal();
                    auto &signal_obj = new_signals[std::to_string(signal.first)] = nlohmann::json::object();
                    signal_obj["target_id"] = (int)connection->get_postsynaptic()->get_id();
                    signal_obj["type"] = signal.second.second == 0 ? "action_potential" : signal.second.second == 1 ? "backpropagation_up" : signal.second.second == 2 ? "backpropagation_down" : "teaching";
                }

                while(connection->has_deleted_signal())
                {
                    auto signal = connection->pop_deleted_signal();
                    auto &signal_obj = deleted_signals[std::to_string(signal.first)] = nlohmann::json::object();
                    signal_obj["target_id"] = (int)connection->get_postsynaptic()->get_id();
                }

                for(auto &signal : *connection)
                {
                    auto &signal_obj = signals[std::to_string(signal.first)] = nlohmann::json::object();
                    signal_obj["state"] = signal.second.first / connection->get_length();
                    signal_obj["target_id"] = (int)connection->get_postsynaptic()->get_id();
                }
            }
        }
    }

    return state.dump();
}

void brain::set_input_image(const std::string &image_string)
{
    for(auto &layer : *this)
    {
        if(!layer.second->has_class("image")) continue;

        std::stringstream ss(image_string);
        int part;

        for(auto &neuron : *layer.second)
        {
            ss >> part;
            
            if(part > 50)
            {
                neuron->set_external_current(14);
            }
            else
            {
                neuron->set_external_current(0);
            }
        }		    
    }
}

void brain::train(const std::string &input_image, const std::string &expected_output)
{
    set_input_image(input_image);

    if(expected_output == "") return;

    for(auto &layer : *this)
    {
        if(!layer.second->has_class("trainable")) continue;

	for(auto &neuron : *layer.second.get())
	{
	    neuron->clear_class();

	    if(neuron->get_label() == expected_output)
	    {
		neuron->add_class("expected_fire");
	    }
	    else
	    {
		neuron->add_class("expected_no_fire");
	    }
	}
    }
}

} // namespace cort
