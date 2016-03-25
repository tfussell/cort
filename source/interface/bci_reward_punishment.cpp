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
#include <fstream>

#include "interface/bci_reward_punishment.hpp"
#include "brain/layer.hpp"
#include "brain/neurotransmitter.hpp"
#include "brain/neuron.hpp"

namespace cort {

bci_reward_punishment::bci_reward_punishment(layer *layer)
{
    set_layer(layer);
    initialize();
}

bci_reward_punishment::~bci_reward_punishment()
{
}

void bci_reward_punishment::initialize()
{

}

std::string bci_reward_punishment::get_state()
{
	return "";
}

void bci_reward_punishment::set_state(const std::string &/*state*/)
{
/*
    std::printf("Reward: %s\n",state.c_str());
    
    if(state == "Good")
    {
	for(unsigned int i = 0; i < m_Interface->GetSize(); i++)
	{
	    m_Interface->GetNeuron(i)->BackpropagateUp();
	}
    }
    else if(state == "Bad")
    {
	for(unsigned int i = 0; i < m_Interface->GetSize(); i++)
	{
	    m_Interface->GetNeuron(i)->BackpropagateDown();
	}
    }
*/
}

} // namespace cort
