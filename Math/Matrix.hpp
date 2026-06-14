/*
    Simple Matrix Class, built for cassshh efficency.
*/

#pragma once

// System Includes
#include <algorithm>
#include <cstddef>
#include <format>
#include <functional>
#include <memory>
#include <new>
#include <stdexcept>

// Structs

struct Shape
{
    std::size_t rows;
    std::size_t cols;
};

template<typename T>
class Matrix
{
  public:
    using iterator = T*;
    using const_iterator = T const*;

    Matrix () = delete;
    ~Matrix () = default;

    // Constructor
    Matrix (unsigned rows, unsigned cols)
        : m_data (std::make_unique<T[]> (rows * cols)),
          m_rows (rows),
          m_cols (cols) {};

    // Copy Constructor
    Matrix (Matrix const& m)
        : Matrix (m.m_rows, m.m_cols)
    {
        std::copy (m.begin (), m.end (), this->begin ());
    }

    // Move Constructor
    Matrix (Matrix&& other) noexcept
        : m_data (std::move (other.m_data)),
          m_rows (other.m_rows),
          m_cols (other.m_cols)
    {
    }

    // swap
    void
    swap (Matrix& other) noexcept
    {
    }

    // Copy Assingment
    Matrix&
    operator= (Matrix m)
    {
        swap (m);
        return *this;
    }

    Matrix&
    operator= (Matrix&& m) noexcept
    {
        swap (m);
        return *this;
    }

    // Other methods
    unsigned
    rows () const
    {
        return m_rows;
    }
    unsigned
    cols () const
    {
        return m_cols;
    };

    // returns the shape of the matrix
    // Rows X Cols
    Shape
    order () const
    {
        return Shape { m_rows, m_cols };
    };

    // return the number of elements in matrix
    unsigned
    size () const
    {
        return m_rows * m_cols;
    }

    // returning the data
    iterator
    begin ()
    {
        return m_data.get ();
    };

    const_iterator
    begin () const
    {
        return m_data.get ();
    };

    iterator
    end ()
    {
        return this->begin () + this->size ();
    };

    const_iterator
    end () const
    {
        return this->begin () + this->size ();
    };

    // return specefic elements

    T&
    operator[] (unsigned row, unsigned col)
    {
        return m_data[row * m_cols + col];
    }

    T const&
    operator[] (unsigned row, unsigned col) const
    {
        return m_data[row * m_cols + col];
    }

    // Adding two Matrix with + operator
    // Returns: new result Matrix
    Matrix<T>
    operator+ (Matrix<T> const& B)
    {
        if (!(m_rows == B.rows () && m_cols == B.cols ()))
        {
            throw std::runtime_error ("Invalid Matrix for + operations. ");
        }

        Matrix<T> result (m_rows, m_cols);

        std::transform (
          begin (), end (), B.begin (), result.begin (), std::plus<> ());

        return result;
    }

    // Subtraction of two Matrix with - operator
    // Returns: new result Matrix, This - B.
    Matrix<T>
    operator- (Matrix<T> const& B)
    {
        if (!(m_rows == B.rows () && m_cols == B.cols ()))
        {
            throw std::runtime_error ("Invalid Matrix for - operations. ");
        }

        Matrix<T> result (m_rows, m_cols);

        std::transform (
          begin (), end (), B.begin (), result.begin (), std::minus<> ());

        return result;
    }

    // Multiply two Matrix with * operator
    // Returns: new result Matrix, This x B
    // Done using intrinsics, blocked for performance optimization

    // Big Problem here
    /*
        TODO: only use blocking for big matrix not smaller ones
        Fix multiply issue.
    */
    Matrix<T>
    operator* (Matrix<T> const& B)
    {
        if (!(m_cols == B.rows ()))
        {
            throw std::runtime_error ("Invalid Matrix for * operation. ");
        }

        Matrix<T> result (m_rows, B.cols ());
        int BLOCK_SIZE = 128;

        unsigned A_order = m_cols * m_rows;
        unsigned B_order = B.cols () * B.rows ();

        for (unsigned k = 0; k < A_order; k += BLOCK_SIZE)
        {
            for (unsigned i = 0; i < A_order; i += BLOCK_SIZE)
            {
                for (unsigned j = 0; j < A_order; j += BLOCK_SIZE)
                {
                    // Same for Blocks + intrinsics
                    for (unsigned k1 = k; k1 < k + BLOCK_SIZE; ++k1)
                    {
                        for (unsigned i1 = i; i1 < i + BLOCK_SIZE; ++i1)
                        {

                            T r = m_data[i * k];

                            for (unsigned j1 = j; j1 < j + BLOCK_SIZE; ++j1)
                            {
                                result[i, j] += r * B[k, i];
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

  private:
    static constexpr std::align_val_t CACHE_LINE_BYTES { 64 };
    struct AlignedDeleter
    {
        void
        operator() (T* p) const
        {
            ::operator delete[] (p, CACHE_LINE_BYTES);
        }
    };
    std::unique_ptr<T[]> m_data;
    unsigned m_rows;
    unsigned m_cols;
};

template<typename T>
struct std::formatter<Matrix<T>> : std::formatter<std::string>
{
    auto
    format (const Matrix<T>& m, format_context& ctx) const
    {
        std::string s;

        for (std::size_t r = 0; r < m.rows (); ++r)
        {
            s += "[ ";
            for (std::size_t c = 0; c < m.cols (); ++c)
            {
                s += std::format ("{} ", m[r, c]);
            }
            s += "]\n";
        }

        return std::formatter<std::string>::format (s, ctx);
    }
};