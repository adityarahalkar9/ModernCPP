import std;
#include "structs_header.h"

// Suboptimal layouts
struct BadLayout1 {
    char a;      // 1 + 7 padding
    double b;    // 8 bytes
    char c;      // 1 + 7 padding
    double d;    // 8 bytes
    // Total: 32 bytes
};

struct BadLayout2 {
    int a;       // 4 bytes
    char b;      // 1 + 3 padding
    int c;       // 4 bytes
    char d;      // 1 + 3 padding
    // Total: 16 bytes
};

struct BadLayout3 {
    char a;
    short b;     // 1 + 1 padding
    int c;
    char d;      // 1 + 3 padding
    double e;
    char f;      // 1 + 7 padding
    // Total: 32 bytes
};

void BadLayouts() {
    std::cout << "=== Suboptimal Struct Layouts ===" << std::endl;

    std::cout << "BadLayout1 (32 bytes):" << std::endl;
    std::cout << "  a: offset " << offsetof(BadLayout1, a) << std::endl;
    std::cout << "  b: offset " << offsetof(BadLayout1, b) << std::endl;
    std::cout << "  c: offset " << offsetof(BadLayout1, c) << std::endl;
    std::cout << "  d: offset " << offsetof(BadLayout1, d) << std::endl;
    std::cout << "  Total: " << sizeof(BadLayout1) << " bytes" << std::endl;

    std::cout << "\nBadLayout2 (16 bytes):" << std::endl;
    std::cout << "  a: offset " << offsetof(BadLayout2, a) << std::endl;
    std::cout << "  b: offset " << offsetof(BadLayout2, b) << std::endl;
    std::cout << "  c: offset " << offsetof(BadLayout2, c) << std::endl;
    std::cout << "  d: offset " << offsetof(BadLayout2, d) << std::endl;
    std::cout << "  Total: " << sizeof(BadLayout2) << " bytes" << std::endl;
}

// Optimized layouts
struct GoodLayout1 {
    double b;    // 8 bytes
    double d;    // 8 bytes
    char a;      // 1 byte
    char c;      // 1 + 6 padding
    // Total: 24 bytes (25% smaller!)
};

struct GoodLayout2 {
    int a;       // 4 bytes
    int c;       // 4 bytes
    char b;      // 1 byte
    char d;      // 1 + 2 padding
    // Total: 12 bytes (25% smaller!)
};

struct GoodLayout3 {
    double e;    // 8 bytes
    int c;       // 4 bytes
    short b;     // 2 bytes
    char a;      // 1 byte
    char d;      // 1 byte
    // Total: 16 bytes (50% smaller!)
};

void OptimizedLayouts() {
    std::cout << "\n=== Optimized Struct Layouts ===" << std::endl;

    std::cout << "GoodLayout1 (24 bytes vs 32):" << std::endl;
    std::cout << "  b: offset " << offsetof(GoodLayout1, b) << std::endl;
    std::cout << "  d: offset " << offsetof(GoodLayout1, d) << std::endl;
    std::cout << "  a: offset " << offsetof(GoodLayout1, a) << std::endl;
    std::cout << "  c: offset " << offsetof(GoodLayout1, c) << std::endl;
    std::cout << "  Total: " << sizeof(GoodLayout1) << " bytes" << std::endl;

    std::cout << "\nSavings:" << std::endl;
    std::cout << "  BadLayout1: " << sizeof(BadLayout1) << " bytes" << std::endl;
    std::cout << "  GoodLayout1: " << sizeof(GoodLayout1) << " bytes" << std::endl;
    std::cout << "  Saved: " << sizeof(BadLayout1) - sizeof(GoodLayout1) << " bytes ("
        << (100.0 * (sizeof(BadLayout1) - sizeof(GoodLayout1)) / sizeof(BadLayout1)) << "%)" << std::endl;
}

// Rule: Sort by size (largest to smallest)
struct BySize {
    // Correct order: 8, 4, 2, 1
    double d;    // 8 bytes
    int i;       // 4 bytes
    short s;     // 2 bytes
    char c;      // 1 + 1 padding
    // Total: 16 bytes
};

void SortBySize() {
    std::cout << "\n=== Sort by Size Rule ===" << std::endl;

    std::cout << "Optimal ordering (largest to smallest):" << std::endl;
    std::cout << "  1. double/long long (8 bytes)" << std::endl;
    std::cout << "  2. pointers (8 bytes on 64-bit)" << std::endl;
    std::cout << "  3. int/float (4 bytes)" << std::endl;
    std::cout << "  4. short (2 bytes)" << std::endl;
    std::cout << "  5. char/bool (1 byte)" << std::endl;

    std::cout << "\nBySize layout:" << std::endl;
    std::cout << "  d: offset " << offsetof(BySize, d) << std::endl;
    std::cout << "  i: offset " << offsetof(BySize, i) << std::endl;
    std::cout << "  s: offset " << offsetof(BySize, s) << std::endl;
    std::cout << "  c: offset " << offsetof(BySize, c) << std::endl;
    std::cout << "  Total: " << sizeof(BySize) << " bytes" << std::endl;
}

// Using #pragma pack
#pragma pack(push, 1)
struct Packed {
    char a;      // 1 byte (no padding!)
    int b;       // 4 bytes (no padding!)
    char c;      // 1 byte (no padding!)
    double d;    // 8 bytes (no padding!)
    // Total: 14 bytes (no padding at all!)
};
#pragma pack(pop)

void PackedStructs() {
    std::cout << "\n=== Packed Structs ===" std::endl;

    std::cout << "sizeof(Packed) = " << sizeof(Packed) << " bytes" << std::endl;
    std::cout << "offsetof(Packed, a) = " << offsetof(Packed, a) << std::endl;
    std::cout << "offsetof(Packed, b) = " << offsetof(Packed, b) << std::endl;
    std::cout << "offsetof(Packed, c) = " << offsetof(Packed, c) << std::endl;
    std::cout << "offsetof(Packed, d) = " << offsetof(Packed, d) << std::endl;

    std::cout << "\n#pragma pack(1) removes all padding" << std::endl;
    std::cout << "Trade-offs:" << std::endl;
    std::cout << "  + Smaller struct size" << std::endl;
    std::cout << "  - Slower access (misaligned loads)" << std::endl;
    std::cout << "  - May crash on some architectures" << std::endl;
    std::cout << "  - Non-standard, compiler-specific" << std::endl;
}

// Alignas for custom alignment
struct alignas(16) Aligned {
    double a, b;
    // Forces 16-byte alignment
};

void CustomAlignment() {
    std::cout << "\n=== Custom Alignment ===" << std::endl;

    std::cout << "alignof(Aligned) = " << alignof(Aligned) << std::endl;
    std::cout << "sizeof(Aligned) = " << sizeof(Aligned) << std::endl;

    Aligned a;
    std::cout << "Address of a: " << &a << std::endl;
    std::cout << "Is 16-aligned: " << (reinterpret_cast<uintptr_t>(&a) % 16 == 0) << std::endl;

    std::cout << "\nUse cases for custom alignment:" << std::endl;
    std::cout << "  - SIMD operations (16/32/64 byte alignment)" << std::endl;
    std::cout << "  - Cache line alignment (64 bytes)" << std::endl;
    std::cout << "  - DMA requirements" << std::endl;
}