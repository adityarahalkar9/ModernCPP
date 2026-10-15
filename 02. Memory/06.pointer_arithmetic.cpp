#include <iostream>
#include <cstring>
#include "memory_header.h"

// Basic pointer arithmetic
void BasicPointerArithmetic() {
    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;  // Points to first element

    std::cout << "=== Basic Pointer Arithmetic ===" << std::endl;
    std::cout << "p points to: " << *p << " (arr[0])" << std::endl;

    // Increment moves by sizeof(type)
    p++;  // Now points to arr[1]
    std::cout << "After p++: " << *p << " (arr[1])" << std::endl;

    p += 2;  // Now points to arr[3]
    std::cout << "After p+=2: " << *p << " (arr[3])" << std::endl;

    p--;  // Now points to arr[2]
    std::cout << "After p--: " << *p << " (arr[2])" << std::endl;

    // Difference between pointers
    int* start = arr;
    int* end = arr + 5;
    std::cout << "Pointer difference: " << (end - start) << " elements" << std::endl;
}

// Pointer arithmetic with different types
void TypedPointerArithmetic() {
    std::cout << "\n=== Typed Pointer Arithmetic ===" << std::endl;

    int int_arr[5] = {1, 2, 3, 4, 5};
    double dbl_arr[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    char char_arr[5] = {'a', 'b', 'c', 'd', 'e'};

    int* ip = int_arr;
    double* dp = dbl_arr;
    char* cp = char_arr;

    std::cout << "int pointer:    " << ip << " -> " << *ip << std::endl;
    std::cout << "double pointer: " << dp << " -> " << *dp << std::endl;
    std::cout << "char pointer:   " << cp << " -> " << *cp << std::endl;

    ip++;  // Moves 4 bytes (sizeof(int))
    dp++;  // Moves 8 bytes (sizeof(double))
    cp++;  // Moves 1 byte (sizeof(char))

    std::cout << "After ++:" << std::endl;
    std::cout << "int pointer:    " << ip << " -> " << *ip << " (moved " << sizeof(int) << " bytes)" << std::endl;
    std::cout << "double pointer: " << dp << " -> " << *dp << " (moved " << sizeof(double) << " bytes)" << std::endl;
    std::cout << "char pointer:   " << cp << " -> " << *cp << " (moved " << sizeof(char) << " byte)" << std::endl;
}

// Array-pointer duality
void ArrayPointerDuality() {
    std::cout << "\n=== Array-Pointer Duality ===" << std::endl;

    int arr[5] = {1, 2, 3, 4, 5};

    // arr decays to pointer to first element
    int* p1 = arr;         // Same as &arr[0]
    int* p2 = &arr[0];     // Explicit

    std::cout << "arr == &arr[0]: " << (p1 == p2) << std::endl;

    // But arr and &arr have different types!
    // arr  -> int[5], decays to int*
    // &arr -> int(*)[5] (pointer to entire array)

    std::cout << "sizeof(arr):  " << sizeof(arr) << " (entire array)" << std::endl;
    std::cout << "sizeof(&arr): " << sizeof(&arr) << " (just a pointer)" << std::endl;

    // Subscript is pointer arithmetic: arr[i] == *(arr + i)
    std::cout << "arr[2] = " << arr[2] << ", *(arr+2) = " << *(arr + 2) << std::endl;

    // This also works (though confusing):
    std::cout << "2[arr] = " << 2[arr] << " (same as arr[2])" << std::endl;
}

// Pointer comparisons
void PointerComparisons() {
    std::cout << "\n=== Pointer Comparisons ===" << std::endl;

    int arr[5] = {1, 2, 3, 4, 5};
    int* begin = arr;
    int* end = arr + 5;

    std::cout << "begin < end: " << (begin < end) << std::endl;
    std::cout << "begin == arr: " << (begin == arr) << std::endl;
    std::cout << "end == arr+5: " << (end == arr + 5) << std::endl;

    // Only compare pointers within the same array (or one-past-end)
    // Comparing unrelated pointers is unspecified/undefined

    // Null pointer comparisons
    int* np = nullptr;
    std::cout << "np == nullptr: " << (np == nullptr) << std::endl;
    std::cout << "!np: " << (!np) << std::endl;

    // C++20: three-way comparison for pointers
    auto cmp = begin <=> end;
    std::cout << "begin <=> end: " << (cmp < 0 ? "less" : "greater/equal") << std::endl;
}

// Pointer arithmetic with multi-dimensional arrays
void MultiDimensionalPointerArithmetic() {
    std::cout << "\n=== Multi-dimensional Arrays ===" << std::endl;

    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // matrix has type int[3][4]
    // matrix[i] has type int[4], decays to int*
    // matrix[i][j] is an int

    int* row0 = matrix[0];       // Points to first row
    int (*rowPtr)[4] = matrix;   // Pointer to array of 4 ints

    std::cout << "matrix[1][2] = " << matrix[1][2] << std::endl;
    std::cout << "*(*(matrix+1)+2) = " << *(*(matrix + 1) + 2) << " (same)" << std::endl;

    // Flattened access: treat as contiguous
    int* flat = &matrix[0][0];
    std::cout << "flat[6] = " << flat[6] << " (matrix[1][2])" << std::endl;

    // Row pointer arithmetic
    rowPtr++;  // Moves to next row (16 bytes for 4 ints)
    std::cout << "After rowPtr++, (*rowPtr)[0] = " << (*rowPtr)[0] << " (matrix[1][0])" << std::endl;
}

// Unsafe pointer tricks (educational - understand the risks!)
void UnsafePointerTricks() {
    std::cout << "\n=== Unsafe Pointer Tricks (Educational) ===" << std::endl;

    // Casting between pointer types
    double d = 3.14159;
    char* bytes = reinterpret_cast<char*>(&d);

    std::cout << "Bytes of " << d << ": ";
    for(size_t i = 0; i < sizeof(double); ++i) {
        std::cout << static_cast<int>(static_cast<unsigned char>(bytes[i])) << " ";
    }
    std::cout << std::endl;

    // WARNING: These are undefined behavior in many cases!
    // - Dereferencing misaligned pointers
    // - Accessing objects through wrong type (strict aliasing violations)
    // - Going out of bounds

    std::cout << "\nStrict aliasing rule:" << std::endl;
    std::cout << "  You may only access objects through:" << std::endl;
    std::cout << "  - Their own type" << std::endl;
    std::cout << "  - char/unsigned char (byte access)" << std::endl;
    std::cout << "  - std::byte" << std::endl;
    std::cout << "  Use std::bit_cast (C++20) for safe type punning" << std::endl;
}

// Pointer to one-past-the-end
void OnePastTheEnd() {
    std::cout << "\n=== One-Past-The-End Pointers ===" << std::endl;

    int arr[5] = {1, 2, 3, 4, 5};
    int* end = arr + 5;  // Legal: one-past-the-end

    std::cout << "end = arr + 5 is legal to compute" << std::endl;
    std::cout << "But you cannot dereference it!" << std::endl;
    // std::cout << *end;  // UNDEFINED BEHAVIOR

    // This is how standard algorithms work:
    for(int* p = arr; p != end; ++p) {
        std::cout << *p << " ";
    }
    std::cout << std::endl;

    // Can compare with one-past-the-end
    std::cout << "arr < end: " << (arr < end) << std::endl;
    std::cout << "end - arr: " << (end - arr) << " elements" << std::endl;
}

// Working with raw memory (memcpy, memmove)
void RawMemoryOperations() {
    std::cout << "\n=== Raw Memory Operations ===" << std::endl;

    int src[5] = {1, 2, 3, 4, 5};
    int dst[5];

    // memcpy: copy non-overlapping regions
    std::memcpy(dst, src, sizeof(src));
    std::cout << "After memcpy: ";
    for(int i : dst) std::cout << i << " ";
    std::cout << std::endl;

    // memmove: safe for overlapping regions
    int overlap[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::memmove(overlap + 2, overlap, 5 * sizeof(int));  // Shift right
    std::cout << "After memmove (shift right): ";
    for(int i : overlap) std::cout << i << " ";
    std::cout << std::endl;

    // memset: fill with a byte value
    char buffer[10];
    std::memset(buffer, 'A', sizeof(buffer));
    std::cout << "After memset: ";
    for(char c : buffer) std::cout << c;
    std::cout << std::endl;

    // memcmp: compare memory regions
    int a[3] = {1, 2, 3};
    int b[3] = {1, 2, 3};
    int c[3] = {1, 2, 4};
    std::cout << "memcmp(a,b): " << std::memcmp(a, b, sizeof(a)) << " (0 = equal)" << std::endl;
    std::cout << "memcmp(a,c): " << std::memcmp(a, c, sizeof(a)) << " (non-0 = different)" << std::endl;
}