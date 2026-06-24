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
              std::function<T (T)> Activation_func = ActivationFun::relu)
        : m_input_size (input_size),
          m_output_size (output_size),
          m_activation (Activation_func)
    {
        m_weights = Matrix<T> (m_input_size, 1);
        m_weights.fill_random ();

        m_bias = Matrix<T> (m_input_size, 1);
        m_bias.fill_zero ();
    }

    Matrix<T>
    forward (const Matrix<T>& input)
    {
        Matrix<T> A = (input * m_weights) + m_bias;
        m_output = A.apply (m_activation);
        return m_output;
    }

    Matrix<T>
    backward ()
    {
        // I need to do this but later
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
    std::function<T (T)> m_activation;

    // Each layer will have input and output
    Matrix<T> m_input;
    Matrix<T> m_output;
};