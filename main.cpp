/*
    Main file
*/

// Includes System
#include <numeric>
#include <print>

// Local
#include "Math/Matrix.hpp"

int
main ()
{

    Matrix<int> A (2, 2);
    std::iota (A.begin (), A.end (), 1);

    Matrix<int> B (2, 2);
    std::iota (B.begin (), B.end (), 1);

    std::println ("A:\n{}", A);
    std::println ("B:\n{}", B);

    Matrix<int> C = A + B;
    std::println ("C:\n{}", C);

    Matrix<int> D = A - B;
    std::println ("D:\n{}", D);

    Matrix<int> E = A * B;
    std::println ("E:\n{}", E);
}