/*
    Test files to test the Matrix Class.
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// Local
#include "../Math/Matrix.hpp"
#include "../external/doctest/doctest.h"

// System
#include <Eigen/Dense>
#include <chrono>
#include <cstdio>
#include <random>
#include <type_traits>

using Clock = std::chrono::high_resolution_clock;

template<typename T>
using Eigen_mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

template<typename T>
static void
fill_random_pair (Matrix<T>& m, EigenMat<T>& e)
{
}