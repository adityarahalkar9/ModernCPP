#include <iostream>
#include <string>
#include "structs_header.h"

// Demonstrating struct memory layout
struct BadLayout {
    char a;      // 1 byte + 7 padding
    double b;    // 8 bytes
    char c;      // 1 byte + 7 padding
    double d;    // 8 bytes
    // Total: 32 bytes
};

struct GoodLayout {
    double b;    // 8 bytes
    double d;    // 8 bytes
    char a;      // 1 byte
    char c;      // 1 byte + 6 padding
    // Total: 24 bytes
};

struct MixedTypes {
    char c;           // 1 + 3 padding
    int i;           // 4 bytes
    short s;         // 2 + 2 padding
    double d;        // 8 bytes
    // Total: 20 bytes
};

void MemoryLayoutBasics() {
    std::cout << "=== Struct Memory Layout ===" << std::endl;

    std::cout << "BadLayout (32 bytes):" << std::endl;
    std::cout << "  a: offset " << offsetof(BadLayout, a) << std::endl;
    std::cout << "  b: offset " << offsetof(BadLayout, b) << std::endl;
    std::cout << "  c: offset " << offsetof(BadLayout, c) << std::endl;
    std::cout << "  d: offset " << offsetof(BadLayout, d) << std::endl;
    std::cout << "  sizeof: " << sizeof(BadLayout) << std::endl;

    std::cout << "\nGoodLayout (24 bytes):" << std::endl;
    std::cout << "  b: offset " << offsetof(GoodLayout, b) << std::endl;
    std::cout << "  d: offset " << offsetof(GoodLayout, d) << std::endl;
    std::cout << "  a: offset " << offsetof(GoodLayout, a) << std::endl;
    std::cout << "  c: offset " << offsetof(GoodLayout, c) << std::endl;
    std::cout << "  sizeof: " << sizeof(GoodLayout) << std::endl;

    std::cout << "\nMixedTypes:" << std::endl;
    std::cout << "  c: offset " << offsetof(MixedTypes, c) << std::endl;
    std::cout << "  i: offset " << offsetof(MixedTypes, i) << std::endl;
    std::cout << "  s: offset " << offsetof(MixedTypes, s) << std::endl;
    std::cout << "  d: offset " << offsetof(MixedTypes, d) << std::endl;
    std::cout << "  sizeof: " << sizeof(MixedTypes) << std::endl;
}

void AlignmentRules() {
    std::cout << "\n=== Alignment Rules ===" << std::endl;

    std::cout << "Alignment requirements:" << std::endl;
    std::cout << "  char:   " << alignof(char) << " byte" << std::endl;
    std::cout << "  short:  " << alignof(short) << " bytes" << std::endl;
    std::cout << "  int:    " << alignof(int) << " bytes" << std::endl;
    std::cout << "  double: " << alignof(double) << " bytes" << std::endl;

    std::cout << "\nStruct alignment = max alignment of members" << std::endl;
    std::cout << "alignof(BadLayout):  " << alignof(BadLayout) << std::endl;
    std::cout << "alignof(GoodLayout): " << alignof(GoodLayout) << std::endl;
    std::cout << "alignof(MixedTypes): " << alignof(MixedTypes) << std::endl;

    std::cout << "\nRules:" << std::endl;
    std::cout << "  1. Each member is aligned to its alignment requirement" << std::endl;
    std::cout << "  2. Struct size is rounded up to multiple of struct alignment" << std::endl;
    std::cout << "  3. Order members largest to smallest for minimal padding" << std::endl;
}

// Empty structs
struct Empty {};

struct WithEmpty {
    Empty e;      // Takes at least 1 byte!
    int i;
};

void EmptyStructSize() {
    std::cout << "\n=== Empty Struct Size ===" << std::endl;

    std::cout << "sizeof(Empty): " << sizeof(Empty) << " (must be at least 1)" << std::endl;
    std::cout << "sizeof(WithEmpty): " << sizeof(WithEmpty) << std::endl;

    std::cout << "\nC++ requires distinct objects have distinct addresses" << std::endl;
    std::cout << "So even 'empty' structs take at least 1 byte" << std::endl;
    std::cout << "Base class optimization (C++20 [[no_unique_address]]) can help" << std::endl;
}

// Structs with inheritance
struct Base { int a; };
struct Derived : Base { int b; };
struct DeepDerived : Derived { int c; };

void InheritanceLayout() {
    std::cout << "\n=== Inheritance Memory Layout ===" << std::endl;

    std::cout << "sizeof(Base): " << sizeof(Base) << std::endl;
    std::cout << "sizeof(Derived): " << sizeof(Derived) << std::endl;
    std::cout << "sizeof(DeepDerived): " << sizeof(DeepDerived) << std::endl;

    std::cout << "\nOffsets in DeepDerived:" << std::endl;
    std::cout << "  a (from Base): " << offsetof(DeepDerived, a) << std::endl;
    std::cout << "  b (from Derived): " << offsetof(DeepDerived, b) << std::endl;
    std::cout << "  c (own): " << offsetof(DeepDerived, c) << std::endl;

    std::cout << "\nBase class members come first in memory" << std::endl;
}

// Virtual functions change layout
struct NoVirtual { int a; };
struct WithVirtual {
    int a;
    virtual void foo() {}
};

void VirtualFunctionLayout() {
    std::cout << "\n=== Virtual Functions and Layout ===" << std::endl;

    std::cout << "sizeof(NoVirtual): " << sizeof(NoVirtual) << std::endl;
    std::cout << "sizeof(WithVirtual): " << sizeof(WithVirtual) << std::endl;

    std::cout << "\nVirtual functions add vtable pointer:" << std::endl;
    std::cout << "  - Hidden pointer at start of struct" << std::endl;
    std::cout << "  - Increases size by sizeof(void*)" << std::endl;
    std::cout << "  - Changes struct to non-standard-layout" << std::endl;
}

// Array of structs - contiguity
struct Vec2 { float x, y; };

void ArrayContiguity() {
    std::cout << "\n=== Array Contiguity ===" << std::endl;

    Vec2 arr[5] = {{1,2}, {3,4}, {5,6}, {7,8}, {9,10}};

    std::cout << "Array addresses:" << std::endl;
    for(int i = 0; i < 5; ++i) {
        std::cout << "  arr[" << i << "] at " << &arr[i] << std::endl;
    }

    std::cout << "\nElements are contiguous (stride = sizeof(Vec2) = "
        << sizeof(Vec2) << ")" << std::endl;
    std::cout << "Address difference: " << (&arr[1] - &arr[0]) << " elements" << std::endl;

    // Can treat as flat array
    float* flat = reinterpret_cast<float*>(arr);
    std::cout << "\nAs flat array: ";
    for(int i = 0; i < 10; ++i) {
        std::cout << flat[i] << " ";
    }
    std::cout << std::endl;
}