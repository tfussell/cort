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
#include <string>
#include <sstream>

#include "brain/layer.hpp"
#include "brain/neuron.hpp"
#include "brain/synapse.hpp"

namespace cort {

std::size_t layer::num_layers_ = 0;

layer::layer() : 
    position_(0, 0, 0), 
    rotation_(1, 0, 0, 0), 
    id_(num_layers_++)
{
}

layer::~layer()
{

}

void layer::add_neuron(std::unique_ptr<neuron> n) 
{ 
    neurons_.push_back(std::move(n));
}

void layer::oscillate(double period)
{
    for(auto &neuron : *this)
    {
	neuron->oscillate(period);
    }
}

} // namespace cort
