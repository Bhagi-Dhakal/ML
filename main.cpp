/*
    Main file
*/

// Includes System
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>

// Local
#include "Math/Dense.hpp"
#include "Math/Matrix.hpp"
#include "Math/Network.hpp"
#include "Math/activation_functions.hpp"

// Prototypes

// Constants
unsigned EPOCHS = 1;
unsigned BATCH_SIZE = 1;
float LEARN_RATE = 0.01;

// Data functions
template<typename T>
void
draw_digit (Matrix<T>& data);

template<typename T>
Matrix<T>
load_data (unsigned rows, unsigned cols, std::string file_path);

template<typename T>
Matrix<T>
get_digit (Matrix<T>& data_set, unsigned index);

template<typename T>
Matrix<T>
get_row (Matrix<T>& data_set,
         unsigned index,
         unsigned row_size,
         unsigned col_size);

int
main ()
{

    // Each row contains an image total of (60K)
    // Each image is 28 X 28 = 784
    Matrix<float> train_images =
      load_data<float> (60000, 784, "data/train_images.mat");
    Matrix<float> test_images =
      load_data<float> (10000, 784, "data/test_images.mat");

    Matrix<float> train_labels (60000, 10);
    Matrix<float> test_labels (10000, 10);

    Matrix<float> train_labels_data =
      load_data<float> (60000, 1, "data/train_labels.mat");
    Matrix<float> test_labels_data =
      load_data<float> (10000, 1, "data/test_labels.mat");

    for (unsigned i : std::views::iota (0u, train_labels_data.rows ()))
    {
        unsigned number = train_labels_data[i, 0];
        train_labels[i, number] = { 1 };
    }

    for (unsigned i : std::views::iota (0u, test_labels_data.rows ()))
    {
        unsigned number = test_labels_data[i, 0];
        test_labels[i, number] = { 1 };
    }

    // unsigned random_ind = 67;
    // Matrix<float> random_digit = get_digit (train_images, random_ind);
    // draw_digit (random_digit);
    // for (unsigned i : std::views::iota (0u, train_labels.cols ()))
    // {
    //     std::print ("{} ", train_labels[random_ind, i]);
    // }

    Network<float> network;

    network.add (784, 128);
    network.add (128, 64);
    network.add (64, 10, ActivationFun::identity<float> ());

    for (unsigned epoch : std::views::iota (0u, EPOCHS))
    {
        //
        unsigned train_images_total = train_images.rows ();

        // should suffel both x and y..?

        for (unsigned i : std::views::iota (0u, train_images_total))
        {
            // network input must be a 784 x 1 column vector
            Matrix<float> x = get_row (train_images, i, 784, 1);
            // one-hot target as a 10 x 1 column vector
            Matrix<float> y = get_row (train_labels, i, 10, 1);

            float loss = network.train_step (x, y, LEARN_RATE);

            // every 3000 th loss printed
            if (i % 3000 == 0)
            {
                std::println (
                  "Epoch: {}, Iteration: {}, Loss: {}", epoch, i, loss);
            }
        }
    }

    return 0;
}

// loads the data and returns a matrix
template<typename T>
Matrix<T>
load_data (unsigned rows, unsigned cols, std::string file_path)
{
    Matrix<T> A (rows, cols);
    std::ifstream in (file_path, std::ios::binary);
    std::span<const T> out_view (A.begin (), A.size ());
    in.read (reinterpret_cast<char*> (A.begin ()), A.size () * sizeof (T));

    if (!in)
    {
        throw std::runtime_error ("Could not open " + file_path);
    }
    return A;
}

// Draws the digit on the terminal
template<typename T>
void
draw_digit (Matrix<T>& data)
{
    for (auto i : std::views::iota (0u, data.rows ()))
    {
        for (auto j : std::views::iota (0u, data.cols ()))
        {
            T num = data[i, j];

            unsigned color = 232 * static_cast<unsigned> (num * 24);

            color = std::clamp (color, 232u, 255u);
            std::print ("\x1b[48;5;{}m  ", color);
        }
        // Reset color and move to next line
        std::println ("\x1b[0m");
    }
}

// retrives one digit from a batch
template<typename T>
Matrix<T>
get_digit (Matrix<T>& data_set, unsigned index)
{
    // Each image is 784 pixels (28 * 28)
    Matrix<T> digit (28, 28);

    unsigned offset = index * 28 * 28;

    std::copy (data_set.begin () + offset,
               data_set.begin () + offset + 784,
               digit.begin ());

    return digit;
}

// Marker comment kept distinct from get_digit
template<typename T>
Matrix<T>
get_row (Matrix<T>& data_set,
         unsigned index,
         unsigned row_size,
         unsigned col_size)
{
    //
    Matrix<T> digit (row_size, col_size);

    unsigned offset = index * row_size * col_size;

    std::copy (data_set.begin () + offset,
               data_set.begin () + offset + row_size * col_size,
               digit.begin ());

    return digit;
}