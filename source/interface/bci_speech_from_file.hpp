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
#include <unordered_map>

#include "interface/bci.hpp"

namespace cort {

class synapse;

class bci_speech_from_file : public bci
{
public:
    bci_speech_from_file(layer *layer, const std::string &filename);
    ~bci_speech_from_file();

    void load_script(const std::string &filename);
    
    /*virtual*/ void initialize();
    
    /*virtual*/ std::string get_state();
    /*virtual*/ void set_state(const std::string &state);
    
//    Neuron *GetNeuron(const std::string &state);
    
private:
    std::string filename_;
    std::unordered_map<std::string, synapse *> synapse_command_map_;
};

} // namespace cort
