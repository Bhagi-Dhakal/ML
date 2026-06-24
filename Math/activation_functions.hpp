/*
    Contains some activation functions.
*/

#pragma once

// Include System
#include <cmath>

// Include Local

namespace ActivationFun
{

// Sigmoid activation function
// returns 1 / (1 + e^(-x))
template<typename T>
auto
sigmoid (T input)
{
    return 1.0 / (1.0 + std::exp (-input));
}

// Relu activation function
// Returans max or value of zero;
template<typename T>
auto
relu (T input)
{
    return (input > 0.0) ? input : 0.0;
}

// Tanh
template<typename T>
auto
tanh (T input)
{
    return std::tanh (input);
}

}
