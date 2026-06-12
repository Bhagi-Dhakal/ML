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
#include <ranges>
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

    Shape
    size () const
    {
        return Shape { m_rows, m_cols };
    };

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
        return this->begin () + this->rows () * this->cols ();
    };

    const_iterator
    end () const
    {
        return this->begin () + this->rows () * this->cols ();
    };

    // return specefic elements

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
        if (!(m_rows == B.size ().rows && m_cols == B.size ().cols))
        {
            throw std::runtime_error ("Invalid Matrix for + operations. ");
        }

        Matrix<T> result (m_rows, m_cols);

        std::transform (
          m_data.begin (), m_data.end (), result.begin (), std::plus<T> ());

        return result;
    }

    // Subtraction of two Matrix with - operator
    // Returns: new result Matrix, This - B.
    Matrix<T>
    operator- (Matrix<T> const& B)
    {
        if (!(m_rows == B.size ().rows && m_cols == B.size ().cols))
        {
            throw std::runtime_error ("Invalid Matrix for - operations. ");
        }

        Matrix<T> result (m_rows, m_cols);

        std::transform (
          m_data.begin (), m_data.end (), result.begin (), std::minus<T> ());

        return result;
    }

    // Multiply two Matrix with * operator
    // Returns: new result Matrix, This x B
    // Done using intrinsics, blocked for performance optimization
    Matrix<T>
    operator* (Matrix<T> const& B)
    {
        if (!(m_rows == B.cols () && m_cols == B.rows ()))
        {
            throw std::runtime_error ("Invalid Matrix for * operation. ");
        }

        Matrix<T> result (m_rows, B.cols ());
        int BLOCK_SIZE = 128;

        unsigned A_order = m_cols * m_rows;
        unsigned B_order = B.cols () * B.rows ();

        // Getting messy
        for (unsigned k = 0; k < A_order; k += BLOCK_SIZE)
        {
            for (unsigned i = 0; i < A_order; i += BLOCK_SIZE)
            {
                for (unsigned j = 0; j < A_order; j += BLOCK_SIZE)
                {
                    /// Same for Blocks + intrinsics
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