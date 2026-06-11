/*
    Simple Matrix Class, built for cassshh efficency.
*/

#pragma once

// System Includes
#include <format>
#include <memory>
#include <new>
#include <utility>

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

    std::pair<unsigned, unsigned>
    size () const
    {
        return std::make_pair (m_rows, m_cols);
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