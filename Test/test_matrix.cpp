/*
    Test files to test the Matrix Class.
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// Local
#include "../Math/Matrix.hpp"
#include "../external/doctest/doctest.h"

// System
#include <Eigen/Dense>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <ranges>
#include <type_traits>

using Clock = std::chrono::high_resolution_clock;

template<typename T>
using Eigen_mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

// Fills the matrix with random values.
template<typename T>
static void
fill_random_pair (Matrix<T>& m, Eigen_mat<T>& e, std::mt19937& rng)
{
    std::uniform_real_distribution<double> dist (-1.0, 1.0);

    for (unsigned i : std::views::iota (0u, m.rows ()))
        for (unsigned j : std::views::iota (0u, m.cols ()))
        {
            T v = static_cast<T> (dist (rng));
            m[i, j] = v;
            e (i, j) = v;
        }
}

// Compare the elements with some tolerance.
// Cant directly use == since we there will be FP error accum
template<typename T>
static void
except_close (Matrix<T> const& m, Eigen_mat<T> const& e, double epsilon)
{

    for (unsigned i : std::views::iota (0u, m.rows ()))
    {
        for (unsigned j : std::views::iota (0u, m.cols ()))
        {
            REQUIRE (m[i, j] == doctest::Approx (e (i, j)).epsilon (epsilon));
        }
    }
}

// tolarance range
template<typename T>
double
tol ()
{
    return std::is_same_v<T, float> ? 1e-3 : 1e-9;
}

// ------------------------
// Accuracy Test with Eigen

TEST_CASE_TEMPLATE ("Does multiple match Eigen? ", T, double, float)
{

    std::mt19937 rng (3);
    std::uniform_int_distribution<int> dim_dist (1, 500);

    // running 20 tests
    for (unsigned i : std::views::iota (0u, 20u))
    {
        unsigned M = dim_dist (rng);
        unsigned K = dim_dist (rng);
        unsigned N = dim_dist (rng);

        Matrix<T> A (M, K);
        Matrix<T> B (K, N);

        Eigen_mat<T> EA (M, K);
        Eigen_mat<T> EB (K, N);

        fill_random_pair (A, EA, rng);
        fill_random_pair (B, EB, rng);

        except_close (A * B, Eigen_mat<T> (EA * EB), tol<T> ());
    }
}

TEST_CASE_TEMPLATE ("Does add match Eigen? ", T, double, float)
{

    std::mt19937 rng (3);
    std::uniform_int_distribution<int> dim_dist (1, 500);

    // running 20 tests
    for (unsigned i : std::views::iota (0u, 20u))
    {
        unsigned M = dim_dist (rng);
        unsigned K = dim_dist (rng);

        Matrix<T> A (M, K);
        Matrix<T> B (M, K);

        Eigen_mat<T> EA (M, K);
        Eigen_mat<T> EB (M, K);

        fill_random_pair (A, EA, rng);
        fill_random_pair (B, EB, rng);

        except_close (A + B, Eigen_mat<T> (EA + EB), tol<T> ());
    }
}

TEST_CASE_TEMPLATE ("Does subtract match Eigen? ", T, double, float)
{

    std::mt19937 rng (3);
    std::uniform_int_distribution<int> dim_dist (1, 500);

    // running 20 tests
    for (unsigned i : std::views::iota (0u, 20u))
    {
        unsigned M = dim_dist (rng);
        unsigned K = dim_dist (rng);

        Matrix<T> A (M, K);
        Matrix<T> B (M, K);

        Eigen_mat<T> EA (M, K);
        Eigen_mat<T> EB (M, K);

        fill_random_pair (A, EA, rng);
        fill_random_pair (B, EB, rng);

        except_close (A - B, Eigen_mat<T> (EA - EB), tol<T> ());
    }
}

// ----------------------
// Testing speed with Eigen. Eigen is very fast!

TEST_CASE ("Testing Speed with Eigen (double)")
{
    const unsigned n = 512;
    std::mt19937 rng (3);

    Matrix<double> A (n, n);
    Matrix<double> B (n, n);

    Eigen_mat<double> EA (n, n);
    Eigen_mat<double> EB (n, n);

    fill_random_pair (A, EA, rng);
    fill_random_pair (B, EB, rng);

    auto t0 = Clock::now ();
    Matrix<double> C = A * B;
    auto t1 = Clock::now ();

    Eigen_mat<double> EC (n, n);
    auto t2 = Clock::now ();
    EC.noalias () = EA * EB;
    auto t3 = Clock::now ();

    const double mine =
      std::chrono::duration<double, std::milli> (t1 - t0).count ();
    const double eig =
      std::chrono::duration<double, std::milli> (t3 - t2).count ();
    const double flops = 2.0 * n * n * n;

    std::printf ("\n[timing] %ux%u double matmul\n", n, n);
    std::printf ("    Matrix : %8.2f ms   (%.2f GFLOP/s)\n",
                 mine,
                 flops / (mine / 1e3) / 1e9);
    std::printf ("    Eigen  : %8.2f ms   (%.2f GFLOP/s)\n",
                 eig,
                 flops / (eig / 1e3) / 1e9);
    std::printf ("    Eigen is %.1fx faster\n\n", mine / eig);

    // accuracy must still hold at this size
    except_close (C, EC, 1e-8);
}