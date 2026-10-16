import std;
#include "structs_header.h"

// Basic alignment with structs
struct DefaultAligned {
    char a;
    int b;
    double c;
};

struct Alignas16 {
    alignas(16) char data[16];
};

struct Alignas32 {
    alignas(32) double data[4];
};

struct Alignas64 {
    alignas(64) char cache_line[64];
};

void AlignmentBasics() {
    std::cout << "=== Struct Alignment ===" << std::endl;

    std::cout << "DefaultAligned:" << std::endl;
    std::cout << "  sizeof: " << sizeof(DefaultAligned) << std::endl;
    std::cout << "  alignof: " << alignof(DefaultAligned) << std::endl;
    std::cout << "  offsetof(a): " << offsetof(DefaultAligned, a) << std::endl;
    std::cout << "  offsetof(b): " << offsetof(DefaultAligned, b) << std::endl;
    std::cout << "  offsetof(c): " << offsetof(DefaultAligned, c) << std::endl;

    std::cout << "\nAlignas16:" << std::endl;
    std::cout << "  sizeof: " << sizeof(Alignas16) << std::endl;
    std::cout << "  alignof: " << alignof(Alignas16) << std::endl;

    std::cout << "\nAlignas32:" << std::endl;
    std::cout << "  sizeof: " << sizeof(Alignas32) << std::endl;
    std::cout << "  alignof: " << alignof(Alignas32) << std::endl;
}

// Stack alignment
void StackAlignment() {
    std::cout << "\n=== Stack Alignment ===" << std::endl;

    Alignas16 a16;
    Alignas32 a32;
    Alignas64 a64;

    std::cout << "Alignas16 address: " << &a16 << std::endl;
    std::cout << "  16-aligned: " << (reinterpret_cast<uintptr_t>(&a16) % 16 == 0) << std::endl;

    std::cout << "Alignas32 address: " << &a32 << std::endl;
    std::cout << "  32-aligned: " << (reinterpret_cast<uintptr_t>(&a32) % 32 == 0) << std::endl;

    std::cout << "Alignas64 address: " << &a64 << std::endl;
    std::cout << "  64-aligned: " << (reinterpret_cast<uintptr_t>(&a64) % 64 == 0) << std::endl;
}

// Heap alignment with new
void HeapAlignment() {
    std::cout << "\n=== Heap Alignment ===" << std::endl;

    // C++17: aligned new
    Alignas32* p32 = new Alignas32;
    std::cout << "new Alignas32 address: " << p32 << std::endl;
    std::cout << "  32-aligned: " << (reinterpret_cast<uintptr_t>(p32) % 32 == 0) << std::endl;
    delete p32;

    Alignas64* p64 = new Alignas64;
    std::cout << "new Alignas64 address: " << p64 << std::endl;
    std::cout << "  64-aligned: " << (reinterpret_cast<uintptr_t>(p64) % 64 == 0) << std::endl;
    delete p64;
}

// Alignment for SIMD
struct alignas(16) SIMD4 {
    float x, y, z, w;
};

void SIMDAlignment() {
    std::cout << "\n=== SIMD Alignment ===" << std::endl;

    std::cout << "SIMD4 (SSE aligned):" << std::endl;
    std::cout << "  sizeof: " << sizeof(SIMD4) << std::endl;
    std::cout << "  alignof: " << alignof(SIMD4) << std::endl;

    SIMD4 v = {1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "  Address: " << &v << std::endl;
    std::cout << "  16-aligned: " << (reinterpret_cast<uintptr_t>(&v) % 16 == 0) << std::endl;

    // Can be used with SSE intrinsics without unaligned loads
    std::cout << "\nUse for:" << std::endl;
    std::cout << "  - SSE (128-bit): alignas(16)" << std::endl;
    std::cout << "  - AVX (256-bit): alignas(32)" << std::endl;
    std::cout << "  - AVX-512 (512-bit): alignas(64)" << std::endl;
}

// Over-alignment and containers
struct OverAligned {
    alignas(64) double data[8];  // Cache line size
};

void OverAlignedContainers() {
    std::cout << "\n=== Over-Aligned in Containers ===" << std::endl;

    std::cout << "sizeof(OverAligned): " << sizeof(OverAligned) << std::endl;
    std::cout << "alignof(OverAligned): " << alignof(OverAligned) << std::endl;

    // std::vector with over-aligned types
    std::vector<OverAligned> vec;
    vec.reserve(4);

    std::cout << "\nVector data address: " << vec.data() << std::endl;
    std::cout << "  64-aligned: " << (reinterpret_cast<uintptr_t>(vec.data()) % 64 == 0) << std::endl;

    std::cout << "\nC++17 requires containers to support over-aligned types" << std::endl;
}

// Alignment trade-offs
void AlignmentTradeoffs() {
    std::cout << "\n=== Alignment Trade-offs ===" std::endl;

    std::cout << "Higher alignment:" << std::endl;
    std::cout << "  + Better for SIMD" << std::endl;
    std::cout << "  + Better cache utilization (sometimes)" << std::endl;
    std::cout << "  + Required for some hardware" << std::endl;
    std::cout << "  - More memory waste" << std::endl;
    std::cout << "  - Harder for allocator" << std::endl;
    std::cout << "  - May reduce number of objects per cache line" << std::endl;

    std::cout << "\nDefault alignment:" << std::endl;
    std::cout << "  + Memory efficient" << std::endl;
    std::cout << "  + Works with standard allocator" << std::endl;
    std::cout << "  - May not be optimal for SIMD" << std::endl;
    std::cout << "  - May cause misaligned access penalties" << std::endl;

    std::cout << "\nGuidelines:" << std::endl;
    std::cout << "  - Use default for most structs" << std::endl;
    std::cout << "  - Use 16/32/64 for SIMD data" << std::endl;
    std::cout << "  - Use 64 for cache-line alignment" << std::endl;
    std::cout << "  - Measure before optimizing" << std::endl;
}