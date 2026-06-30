/*
    Contains some activation functions, and it's derivitives
*/

#pragma once

// Include System
#include <cmath>
#include <functional>

// Include Local

template<typename T>
struct Activation
{
    std::function<T (T)> f;
    std::function<T (T)> df;
};

namespace ActivationFun
{

// Sigmoid activation function
// returns 1 / (1 + e^(-x)),  or it's derivatives
template<typename T>
Activation<T>
sigmoid ()
{
    return { [] (T x) { return T (1) / (T (1) + std::exp (-x)); },
             [] (T x)
             {
                 T s = T (1) / (T (1) + std::exp (-x));
                 return s * (T (1) - s);
             } };
}

// Relu activation function
// Returans max or value of zero;
template<typename T>
Activation<T>
relu ()
{
    return { [] (T x) { return x > T (0) ? x : T (0); },
             [] (T x) { return x > T (0) ? T (1) : T (0); } };
}

// Tanh
template<typename T>
Activation<T>
tanh ()
{
    return { [] (T x) { return std::tanh (x); },
             [] (T x)
             {
                 T t = std::tanh (x);
                 return T (1) - t * t;
             } };
}

template<typename T>
Activation<T>
identity ()
{
    return { [] (T x) { return x; }, [] (T) { return T (1); } };
}

}
