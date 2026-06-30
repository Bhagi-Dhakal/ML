/*
    Loss fucntion using MSE.
*/

#pragma once

// System Includes
#include <ranges>

// local includes
#include "Matrix.hpp"

template<typename T>
T
loss_mse (Matrix<T> const& prediction, Matrix<T> const& actual)
{
    // size of matrix must be r x c ==> n x 1;

    T loss_value = 0.0;

    for (unsigned i : std::views::iota (0u, prediction.rows ()))
    {
        T diff = prediction[i, 0] - actual[i, 0];
        loss_value += diff * diff;
    }

    return loss_value / static_cast<float> (prediction.rows ());
}

template<typename T>
Matrix<T>
loss_mse_grad (Matrix<T> const& prediction, Matrix<T> const& actual)
{
    // size of matrix must be r x c ==> n x 1;
    Matrix<T> grad (prediction.rows (), 1);
    const T n = static_cast<float> (prediction.rows ());
    for (unsigned i : std::views::iota (0u, prediction.rows ()))
    {
        grad[i, 0] = 2.0 * (prediction[i, 0] - actual[i, 0]) / n;
    }

    return grad;
}