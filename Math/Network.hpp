/*
    Network Class, we will use this to build the dence layers
*/

#pragma once

// System Includes
#include <cstddef>
#include <vector>

// local includs
#include "Dense.hpp"
#include "Matrix.hpp"

// Structs
template<typename T>
class Network
{
  public:
    Network () {};

    Network (unsigned input,
             unsigned output,
             std::function<T (T)> layer_activation)
    {
        m_layers.push_back (Dense<T> (input, output, layer_activation));
    }

    // used to add layes to the network
    void
    add (unsigned input, unsigned output, std::function<T (T)> layer_activation)
    {
        m_layers.push_back (Dense<T> (input, output, layer_activation));
    }

    Matrix<T>
    predict (Matrix<T> input)
    {
        Matrix<T> output = input;
        for (auto& layer : m_layers)
        {
            output = layer.forward (output);
        }
        return output;
    }

  private:
    std::vector<Dense<T>> m_layers;
};