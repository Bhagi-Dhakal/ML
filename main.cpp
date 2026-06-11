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

    std::println ("{}", A);
}