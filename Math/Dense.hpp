/*
    Dense Layer Class, we will use this to build networks.
*/

#pragma once

// System Includes
#include <cstddef>
#include <functional>

// Local includes
#include "Matrix.hpp"
#include "activation_functions.hpp"

// Structs
template<typename T>
class Dense
{

  public:
    // Constructor
    Dense<T> (unsigned input_size,
              unsigned output_size,
              Activation<T> Activation_func = ActivationFun::relu)
        : m_input_size (input_size),
          m_output_size (output_size),
          m_activation (std::move (Activation_func)),
          m_weights (output_size, input_size),
          m_bias (output_size, 1),
          m_input (input_size, 1),
          m_z (output_size, 1),
          m_output (output_size, 1),
          m_dW (output_size, input_size),
          m_db (output_size, 1)

    {
        m_weights.fill_random (T (0), T (1));
        m_bias.fill_zero ();
    }

    Matrix<T>
    forward_pass (Matrix<T> const& input)
    {
        m_input = input;
        // out x 1
        m_z = (m_weights * input) + m_bias;
        // out x 1
        m_output = m_z.apply (m_activation.f);
        return m_output;
    }

    Matrix<T>
    backward_pass (Matrix<T> const& grad_output)
    {
        Matrix<T> dz = grad_output.hadamard (m_z.apply (m_activation.df));
        m_dW = dz * m_input.transpose ();
        m_db = dz;
        return m_weights.transpose () * dz;
    }

    // updates the weight
    void
    update (T learning_rate)
    {
        m_weights = m_weights - m_dW.scale (learning_rate);
        m_bias = m_bias - m_db.scale (learning_rate);
    }

    // some accessors
    Matrix<T>&
    weights ()
    {
        return m_weights;
    }

    Matrix<T>&
    bias ()
    {
        return m_bias;
    }

    Matrix<T>&
    grad_weights ()
    {
        return m_dW;
    }

    Matrix<T>&
    grad_bias ()
    {
        return m_db;
    }

  private:
    // input is a vector of m X 1
    unsigned m_input_size;
    // output is a vector of n x 1
    unsigned m_output_size;

    // Each layer has weights and some bias
    Matrix<T> m_weights;
    Matrix<T> m_bias;

    // layer will also have some activation function which we will save
    // Deafult of Relu.
    Activation<T> m_activation;

    // Each layer will have input and output, cahed for backward
    Matrix<T> m_input;
    // pre-activations
    Matrix<T> m_z;
    Matrix<T> m_output;

    // gradients from last backword step
    Matrix<T> m_dW;
    Matrix<T> m_db;
};