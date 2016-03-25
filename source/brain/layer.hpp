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
#include <json.hpp>

#include "math/quaternion.hpp"
#include "math/vector3.hpp"

namespace cort {

class neuron;

class layer
{
 public:
    using neuron_container = std::vector<std::unique_ptr<neuron>>;
    using iterator = neuron_container::iterator;
    using const_iterator = neuron_container::const_iterator;

    layer();
    virtual ~layer() = 0;

    std::size_t size() const { return neurons_.size(); }
    
    iterator begin() { return neurons_.begin(); }
    const_iterator begin() const { return cbegin(); }
    const_iterator cbegin() const { return neurons_.cbegin(); }
    iterator end() { return neurons_.end(); }
    const_iterator end() const { return cend(); }
    const_iterator cend() const { return neurons_.cend(); }

    virtual void connect_to(layer &target, const nlohmann::json &settings) = 0;
    virtual void set_position(vector3 position) = 0;
    virtual void set_rotation(const quaternion &rotation) = 0;
    virtual std::string get_type() const = 0;

    void add_class(const std::string &_class) { class_.insert(_class); }
    bool has_class(const std::string &_class) const { return class_.find(_class) != class_.end(); }

    void oscillate(double period);

protected:
    void add_neuron(std::unique_ptr<neuron> neuron);

    vector3 position_;
    quaternion rotation_;

private:
    static std::size_t num_layers_;
    std::size_t id_;
    neuron_container neurons_;
    std::unordered_set<std::string> class_;
};

} // namespace cort
