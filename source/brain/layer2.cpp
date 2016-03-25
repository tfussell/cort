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
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "brain/layer2.hpp"
#include "brain/connection.hpp"
#include "brain/neuron.hpp"

namespace {

std::vector<std::string> split_string(const std::string &to_split, char delim)
{
    std::size_t index = to_split.find(delim);
    std::size_t prev = 0;
    std::vector<std::string> split;
    
    while(index != std::string::npos)
    {
	split.push_back(to_split.substr(prev, index - prev));
	prev = index + 1; // skip delim
	index = to_split.find(delim, prev);
    }

    split.push_back(to_split.substr(prev));

    return split;
}

} // namespace

namespace cort {

layer2::layer2(const nlohmann::json &settings)
{
    int width = settings["width"];
    int length = settings["length"];

    for(int i = 0; i < width; i++)
    {
	for(int j = 0; j < length; j++)
	{
	    auto n = std::unique_ptr<neuron>(new neuron());
	    n->set_local_position(vector3(0.5 + i - width / 2.0, 0, 0.5 + j - length / 2.0));
	    add_neuron(std::move(n));
	}
    }

    auto center = settings["center"];
    vector3 pos(center[0], center[1], center[2]);
    set_position(pos);

    if(settings.find("orientation") != settings.end())
    {
	auto orientation = settings["orientation"];
	auto deg2rad = [](double degrees) 
	    { 
		// mod for floating point
		// TODO: this can be more efficient but I don't feel like thinking about it right now
		int rotations = std::abs(static_cast<int>(degrees)) / 360 + (degrees < 0 ? 1 : 0);
		degrees += 360 * rotations;
		return degrees / 180.0 * 3.141592653589793238462643383279;
	    };
	auto rotation = quaternion::from_yaw_pitch_roll(deg2rad(orientation[0]), 
							deg2rad(orientation[1]), 
							deg2rad(orientation[2]));
	set_rotation(rotation);
    }
}

layer2::~layer2()
{

}

void layer2::connect_to(layer &target, const nlohmann::json &settings)
{
    if(target.get_type() != "layer2")
    {
	throw std::runtime_error("not implemented");
    }

    double current_change = 10;

    if(settings.find("current-change") != settings.end())
    {
	current_change = settings["current-change"];
    }

    if(settings["type"] == "full")
    {
	for(auto &source_neuron : *this)
	{
	    for(auto &target_neuron : target)
	    {
		new connection(source_neuron.get(), target_neuron.get(), current_change);
	    }
	}
    }
    else if(settings["type"] == "projection")
    {
	double source_density = settings["source-density"];
	double target_density = settings["target-density"];

	double min_distance = 0;
	if(settings.find("minimum-distance") != settings.end())
	{
	    min_distance = settings["minimum-distance"];
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> source_dist(0.0,1.0);
	std::poisson_distribution<> target_dist(target_density);

	auto source_region_strings = split_string(settings["source-region"], ',');
	auto source_x0 = std::stod(source_region_strings[0]);
	auto source_y0 = std::stod(source_region_strings[1]);
	auto source_x1 = std::stod(source_region_strings[2]);
	auto source_y1 = std::stod(source_region_strings[3]);

	double source_width = source_x1 - source_x0;
	double source_height = source_y1 - source_y0;

	auto target_region_strings = split_string(settings["target-region"], ',');
	auto target_x0 = std::stod(target_region_strings[0]);
	auto target_y0 = std::stod(target_region_strings[1]);
	auto target_x1 = std::stod(target_region_strings[2]);
	auto target_y1 = std::stod(target_region_strings[3]);

	double target_width = target_x1 - target_x0;
	double target_height = target_y1 - target_y0;

	double x_scale = target_width / source_width;
	double y_scale = target_height / source_height;

	for(auto &source_neuron : *this)
	{
	    if(source_dist(gen) > source_density)
	    {
		continue;
	    }

	    auto source_pos = source_neuron->get_local_position();

	    // use z here because local coordinates are in the xz plane, not xy
	    if(source_pos.x >= source_x0 
	       && source_pos.z >= source_y0 
	       && source_pos.x <= source_x1 
	       && source_pos.z <= source_y1)
	    {
		auto dest_x = target_x0 + (source_pos.x - source_x0) * x_scale;
		auto dest_y = target_y0 + (source_pos.z - source_y0) * y_scale;
		
		std::unordered_set<neuron *> connected;
		auto connections = static_cast<std::size_t>(target_density);//target_dist(gen));

		while(connected.size() < connections)
		{
		    std::pair<neuron *, double> closest = {nullptr, -1};

		    for(auto &target_neuron : target)
		    {
			if(target_neuron.get() == source_neuron.get()) continue;

			auto target_pos = target_neuron->get_local_position();
			auto delta_x = dest_x - target_pos.x;
			auto delta_y = dest_y - target_pos.z;
			auto distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);
			
			if(connected.find(target_neuron.get()) == connected.end() 
			   && (closest.second == -1 || closest.second > distance)
			   && distance >= min_distance)
			{
			    closest = {target_neuron.get(), distance};
			}
		    }
		    
		    if(closest.first != nullptr)
		    {
			new connection(source_neuron.get(), closest.first, current_change);
			connected.insert(closest.first);
		    }
		    else
		    {
			if(connected.empty())
			{
				std::printf("no connections");
			    std::cerr << "warning: no neurons found in target region" << std::endl;
			}

			break;
		    }
		}
	    }
	}
    }
}

void layer2::set_position(vector3 position)
{
    position_ = position;
    
    for(auto &neuron : *this)
    {
	neuron->update_world_position(position_, rotation_);
    }
}

std::string layer2::get_type() const
{
    return "layer2";
}

void layer2::set_rotation(const quaternion &rotation)
{
    rotation_ = rotation;
    
    for(auto &neuron : *this)
    {
	neuron->update_world_position(position_, rotation_);
    }
}

} // namespace cort
