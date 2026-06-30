/*
    Network Class, we will use this to build the dence layers
*/

#pragma once

// System Includes
#include <cstddef>
#include <ranges>
#include <vector>

// local includs
#include "Dense.hpp"
#include "Loss.hpp"
#include "Matrix.hpp"

// Structs
template<typename T>
class Network
{
  public:
    Network () = default;

    // used to add layes to the network
    void
    add (unsigned input,
         unsigned output,
         Activation<T> activation = ActivationFun::relu<T> ())
    {
        m_layers.push_back (Dense<T> (input, output, std::move (activation)));
    }

    Matrix<T>
    predict (Matrix<T> input)
    {
        for (auto& layer : m_layers)
        {
            input = layer.forward_pass (input);
        }
        return input;
    }

    Matrix<T>
    forward_pass (Matrix<T> input)
    {
        Matrix<T> output = input;
        for (auto& layer : m_layers)
        {
            output = layer.forward_pass (output);
        }
        return output;
    }

    T
    train_step (Matrix<T> const& x, Matrix<T> const& y, T learning_rate)
    {
        Matrix<T> a = predict (x);

        T loss = loss_mse (a, y);
        Matrix<T> grad = loss_mse_grad (a, y);

        for (auto& layer : std::views::reverse (m_layers))
        {
            grad = layer.backward_pass (grad);
        }

        // update the layes
        for (auto& layer : m_layers)
        {
            layer.update (learning_rate);
        }

        return loss;
    }

    std::vector<Dense<T>>&
    layers ()
    {
        return m_layers;
    }

  private:
    std::vector<Dense<T>> m_layers;
};