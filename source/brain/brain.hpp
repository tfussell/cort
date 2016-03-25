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

#include "brain/neuron.hpp"
#include "brain/layer2.hpp"
#include "simulation/entity.hpp"
#include "simulation/trainer.hpp"

namespace cort {

/*
  This class represents a simulated brain. It can be constructed programmatically or from a "geneotype" which encodes its function and structure. It holds an internal state which may be serialized/deserialized. It functions in discrete time increments based on external time signals. Effectively, it has three parts. The input layers provide afferent signals. The central part processes these signals and stores an internal state. The final part is the set of motor layers which translate efferent signals into actions.
 */
class brain : public entity
{
public:
    using layer_container = std::unordered_map<std::string, std::unique_ptr<layer>>;
    using iterator = layer_container::iterator;
    using const_iterator = layer_container::const_iterator;

    brain();
    virtual ~brain();
    
    iterator begin() { return layers_.begin(); }
    const_iterator begin() const { return cbegin(); }
    const_iterator cbegin() const { return layers_.cbegin(); }
    iterator end() { return layers_.end(); }
    const_iterator end() const { return cend(); }
    const_iterator cend() const { return layers_.cend(); }

    void load(const std::string &json_string);
    void add_layer(const nlohmann::json &settings);
    void add_connection(const nlohmann::json &settings);

    void set_input_image(const std::string &image_string);

//    void PrintState();
    void reset() override;
    void update(double time) override;
    layer *get_layer(const std::string &name) override { return layers_.at(name).get(); }
    trainer *get_trainer() { return trainer_; }
    void set_trainer(trainer &trainer) { trainer_ = &trainer; }
    std::string serialize();
    void deserialize(const std::string &str);

    std::string serialize_structure() const override;
    std::string serialize_state() const override;
    
    void train(const std::string &input_image, const std::string  &expected_output);

private:
    double current_time_;
    layer_container layers_;
    trainer *trainer_;
};
    
} // namespace cort
