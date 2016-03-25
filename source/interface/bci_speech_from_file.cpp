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

#include "interface/bci_speech_from_file.hpp"
#include "brain/neurotransmitter.hpp"
#include "brain/neuron.hpp"
#include "brain/layer.hpp"

namespace cort {

bci_speech_from_file::bci_speech_from_file(layer *layer, const std::string &filename) :
    filename_(filename)
{
    set_layer(layer);
    initialize();
}

bci_speech_from_file::~bci_speech_from_file()
{

}

void bci_speech_from_file::initialize()
{
    load_script(filename_);
}

void bci_speech_from_file::load_script(const std::string &/*filename*/)
{
/*
    std::fstream f("../scripts/" + filename);
    
    if(!f.is_open())
    {
	std::printf("Error opening file: %s\n",filename.c_str());
	return;
    }
    
    std::printf("Loading BCI commands from file: %s\n",filename.c_str());
    int numCommands = 0;
    f >> numCommands;
    int index;
    std::string line;
    int i = 0;
    
    while(i++ < numCommands && f.good())
    {
	f >> index;
	f >> line;
	Neuron *neuron = m_Interface->GetNeuron(index);
	
	if(m_Type == BciType_Input)
	{
	    m_SynapseCommandMap[line] = new Synapse(0, neuron);
	}
	else if(m_Type == BciType_Output)
	{
	    m_SynapseCommandMap[line] = new Synapse(neuron, 0);
	}
    }
    
    f.close();
*/
}

std::string bci_speech_from_file::get_state()
{
/*
    for(std::unordered_map<std::string, Synapse *>::iterator i = m_SynapseCommandMap.begin(); i != m_SynapseCommandMap.end(); i++)
    {
	if(i->second->m_Presynaptic->m_LastFired == i->second->m_Presynaptic->m_CurrentTime)
	{
	    return i->first;
	}
    }
*/  
    return "";
}

void bci_speech_from_file::set_state(const std::string &/*state*/)
{
/*
	if(m_SynapseCommandMap.find(state) != m_SynapseCommandMap.end())
	{
		std::printf("Set state: %s\n", state.c_str());
		m_SynapseCommandMap[state]->ActivateReceptor(NT_Glutamate);
	}
*/
}
/*
Neuron *bci_speech_from_file::GetNeuron(const std::string &state)
{
	Synapse *syn = 0;

	if(m_SynapseCommandMap.find(state) != m_SynapseCommandMap.end())
	{
		syn = m_SynapseCommandMap[state];
	}

	if(syn && m_Type == BciType_Input)
	{
		return syn->m_Postsynaptic;
	}
	else if(syn && m_Type == BciType_Output)
	{
		return syn->m_Presynaptic;
	}
	else
	{
		return 0;
	}
}
*/

} // namespace cort
