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
#include "brain/layer_factory.hpp"
#include "brain/layer2.hpp"
#include "brain/neuron.hpp"

namespace cort {

std::unique_ptr<layer> layer_factory::create_layer(const nlohmann::json &settings)
{
    std::unique_ptr<layer> layer_ptr(new layer2(settings));

    if(settings.find("class") != settings.end())
    {
	for(auto &_class : settings.at("class"))
	{
	    layer_ptr->add_class(_class);

	    if(_class == "oscillator")
	    {
		auto period = settings["oscillation-period"];
		layer_ptr->oscillate(period);
	    }
	    else if(_class == "labeled")
	    {
		auto labels = settings["neuron-labels"];
		auto label_iter = labels.begin();

		for(auto &neuron : *layer_ptr.get())
		{
		    neuron->set_label(*label_iter++);
		}
	    }
	}
    }

    return std::move(layer_ptr);
}

} // namespace cort
