import std;
#include "smart_pointers_header.h"

class Tracked {
public:
    Tracked() { std::cout << "  Tracked default constructed" << std::endl; }
    explicit Tracked(int v) : value_(v) {
        std::cout << "  Tracked(" << v << ") constructed" << std::endl;
    }
    ~Tracked() { std::cout << "  Tracked(" << value_ << ") destroyed" << std::endl; }

    int value() const { return value_; }
    void set_value(int v) { value_ = v; }

private:
    int value_ = 0;
};

void UniquePtrArrayBasics() {
    std::cout << "=== unique_ptr Arrays ===" << std::endl;

    // C++11: Raw array with new[]
    std::unique_ptr<Tracked[]> arr1(new Tracked[3]);

    // C++14: make_unique for arrays
    auto arr2 = std::make_unique<Tracked[]>(3);  // Default constructs

    std::cout << "\nArray access:" << std::endl;
    arr1[0] = Tracked(10);  // Assignment (creates temp, moves)
    arr2[0].set_value(20);

    std::cout << "  arr1[0].value(): " << arr1[0].value() << std::endl;
    std::cout << "  arr2[0].value(): " << arr2[0].value() << std::endl;
}

void UniquePtrArrayWithValues() {
    std::cout << "\n=== Array with Initial Values ===" << std::endl;

    // Can't do this with make_unique directly
    // auto arr = std::make_unique<Tracked[]>({1, 2, 3});  // Not supported

    // Workaround: create then initialize
    auto arr = std::make_unique<int[]>(5);
    for(size_t i = 0; i < 5; ++i) {
        arr[i] = static_cast<int>(i * 10);
    }

    std::cout << "Array contents: ";
    for(size_t i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void UniquePtr2DArray() {
    std::cout << "\n=== 2D Arrays with unique_ptr ===" << std::endl;

    const size_t rows = 3;
    const size_t cols = 4;

    // Method 1: Array of unique_ptr arrays
    auto matrix = std::make_unique<std::unique_ptr<int[]>[]>(rows);

    // Allocate each row
    for(size_t i = 0; i < rows; ++i) {
        matrix[i] = std::make_unique<int[]>(cols);
    }

    // Fill and access
    for(size_t i = 0; i < rows; ++i) {
        for(size_t j = 0; j < cols; ++j) {
            matrix[i][j] = static_cast<int>(i * cols + j);
        }
    }

    // Print
    std::cout << "Matrix contents:" << std::endl;
    for(size_t i = 0; i < rows; ++i) {
        for(size_t j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void UniquePtrFlattened2D() {
    std::cout << "\n=== Flattened 2D Array ===" << std::endl;

    const size_t rows = 3;
    const size_t cols = 4;

    // Single allocation, manual indexing
    auto flat = std::make_unique<int[]>(rows * cols);

    for(size_t i = 0; i < rows; ++i) {
        for(size_t j = 0; j < cols; ++j) {
            flat[i * cols + j] = static_cast<int>(i * 10 + j);
        }
    }

    std::cout << "Using [i*cols + j] indexing:" << std::endl;
    std::cout << "  flat[1*4 + 2] = " << flat[1 * cols + 2] << std::endl;

    std::cout << "\nBetter: use std::vector or std::array for multi-dimensional" << std::endl;
}

void UniquePtrArrayPitfalls() {
    std::cout << "\n=== Array Pitfalls ===" std::endl;

    std::cout << "MUST use correct form:" << std::endl;
    std::cout << "  unique_ptr<T[]> for arrays (calls delete[])" << std::endl;
    std::cout << "  unique_ptr<T> for single (calls delete)" << std::endl;

    std::cout << "\nWRONG:" << std::endl;
    std::cout << "  std::unique_ptr<Tracked> bad(new Tracked[3]);" << std::endl;
    std::cout << "  // Uses delete, not delete[] - UNDEFINED BEHAVIOR!" << std::endl;

    std::cout << "\nCORRECT:" << std::endl;
    std::cout << "  std::unique_ptr<Tracked[]> good(new Tracked[3]);" << std::endl;
    std::cout << "  auto better = std::make_unique<Tracked[]>(3);" << std::endl;
}

void ArrayVsVector() {
    std::cout << "\n=== unique_ptr Array vs std::vector ===" << std::endl;

    std::cout << "Prefer std::vector for dynamic arrays:" << std::endl;
    std::cout << "  + Knows its size" << std::endl;
    std::cout << "  + Growable" << std::endl;
    std::cout << "  + Standard algorithms work" << std::endl;
    std::cout << "  + Exception safe" << std::endl;

    std::cout << "\nunique_ptr<T[]> use cases:" << std::endl;
    std::cout << "  - C API interop" << std::endl;
    std::cout << "  - Fixed size, no resize needed" << std::endl;
    std::cout << "  - Custom allocation" << std::endl;

    // vector is easier
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "\nVector size: " << vec.size() << std::endl;
    // Array size is NOT stored - you must track it separately!
}