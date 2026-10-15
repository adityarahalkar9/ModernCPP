#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "basic_header.h"

// C++23: Multidimensional subscript operator
// Before C++23, operator[] could only take a single argument
// C++23 allows operator[] with multiple parameters: arr[i, j, k]


template <typename T>
class Matrix2D{
public:
    Matrix2D(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(rows* cols){
    }
    // C++23: Multidimensional subscript
    T& operator[](size_t row, size_t col){
        return data_[row * cols_ + col];
    }
    const T& operator[](size_t row, size_t col) const{
        return data_[row * cols_ + col];
    }

    // Traditional operator[] returning a proxy
    // (pre-C++23 approach)
    struct RowProxy {
        Matrix2D& matrix;
        size_t row;

        T& operator[](size_t col) {
            return matrix.data_[row * matrix.cols_ + col];
        }
    };

    RowProxy operator[](size_t row) {
        return RowProxy{*this, row};
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

private:
    size_t rows_;
    size_t cols_;
    std::vector<T> data_;
};

void MultidimensionalSubscript() {
    Matrix2D<int> matrix(3, 4);

    // C++23: Direct multidimensional access
    for(size_t i = 0; i < matrix.rows(); ++i) {
        for(size_t j = 0; j < matrix.cols(); ++j) {
            matrix[i, j] = static_cast<int>(i * matrix.cols() + j);
        }
    }

    // Print using C++23 syntax
    std::cout << "Matrix (C++23 syntax):" << std::endl;
    for(size_t i = 0; i < matrix.rows(); ++i) {
        for(size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix[i, j] << "\t";
        }
        std::cout << std::endl;
    }

    // Pre-C++23: Proxy pattern
    std::cout << "Matrix (pre-C++23 syntax):" << std::endl;
    for(size_t i = 0; i < matrix.rows(); ++i) {
        for(size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix[i][j] << "\t";  // Two separate [] calls
        }
        std::cout << std::endl;
    }
}

// 3D array with multidimensional subscript
template <typename T>
class Array3D {
public:
    Array3D(size_t x, size_t y, size_t z)
        : x_(x), y_(y), z_(z), data_(x* y* z) {
    }

    // C++23: Three-dimensional subscript
    T& operator[](size_t x, size_t y, size_t z) {
        return data_[(x * y_ + y) * z_ + z];
    }

    const T& operator[](size_t x, size_t y, size_t z) const {
        return data_[(x * y_ + y) * z_ + z];
    }

private:
    size_t x_, y_, z_;
    std::vector<T> data_;
};

void ThreeDimensionalArray() {
    Array3D<int> arr(2, 3, 4);

    // C++23: Natural 3D access syntax
    for(size_t i = 0; i < 2; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            for(size_t k = 0; k < 4; ++k) {
                arr[i, j, k] = static_cast<int>(i * 100 + j * 10 + k);
            }
        }
    }

    std::cout << "3D array values:" << std::endl;
    for(size_t i = 0; i < 2; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            for(size_t k = 0; k < 4; ++k) {
                std::cout << arr[i, j, k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---" << std::endl;
    }
}

// static operator[] (C++23)
template <typename T>
class StaticMatrix {
public:
    static constexpr size_t Size = 3;

    // C++23: static operator[]
    // Can be called without an object instance
    static T& at(size_t i, size_t j) {
        static std::array<T, Size* Size> data = {};
        return data[i * Size + j];
    }

    // Alternative: static operator[] (C++23)
    static T& operator[](size_t i, size_t j) {
        static std::array<T, Size* Size> data = {};
        return data[i * Size + j];
    }
};

void StaticSubscriptOperator() {
    // C++23: static operator[] allows calling without instance
    // StaticMatrix<int>::at(0, 0) = 42;
    // StaticMatrix<int>::operator[](0, 0) = 42;  // If using static operator[]

    // Note: static operator[] syntax may vary by compiler support
    std::cout << "static operator[] concept demonstrated" << std::endl;
}

// Multidimensional std::mdspan (C++23)
void MdspanDemo() {
#ifdef __cpp_lib_mdspan
#include <mdspan>

    // C++23: std::mdspan provides multidimensional array view
    std::vector<int> data = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12
    };

    // Create a 3x4 mdspan view
    std::mdspan md(data.data(), 3, 4);

    // Access with multidimensional subscript
    std::cout << "mdspan[1, 2]: " << md[1, 2] << std::endl;
    std::cout << "mdspan[0, 0]: " << md[0, 0] << std::endl;

    // Print all elements
    for(size_t i = 0; i < md.extent(0); ++i) {
        for(size_t j = 0; j < md.extent(1); ++j) {
            std::cout << md[i, j] << " ";
        }
        std::cout << std::endl;
    }
#else
    std::cout << "std::mdspan not available (need C++23 with library support)" << std::endl;
#endif
}