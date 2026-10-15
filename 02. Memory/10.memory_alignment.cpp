#include <iostream>
#include <memory>
#include <new>
#include "memory_header.h"

// Basic alignment
struct AlignedStruct {
    alignas(16) double data[4];  // Force 16-byte alignment
};

struct DefaultStruct {
    double data[4];  // Natural alignment (8 bytes)
};

void BasicAlignment() {
    std::cout << "=== Basic Alignment ===" << std::endl;

    std::cout << "alignof(char):     " << alignof(char) << std::endl;
    std::cout << "alignof(int):      " << alignof(int) << std::endl;
    std::cout << "alignof(double):   " << alignof(double) << std::endl;
    std::cout << "alignof(void*):    " << alignof(void*) << std::endl;

    std::cout << "\nsizeof(DefaultStruct): " << sizeof(DefaultStruct) << std::endl;
    std::cout << "alignof(DefaultStruct): " << alignof(DefaultStruct) << std::endl;
    std::cout << "\nsizeof(AlignedStruct): " << sizeof(AlignedStruct) << std::endl;
    std::cout << "alignof(AlignedStruct): " << alignof(AlignedStruct) << std::endl;
}

// Over-aligned types
struct OverAligned {
    alignas(32) char data[64];  // Requires 32-byte alignment
};

void OverAlignment() {
    std::cout << "\n=== Over-Alignment ===" << std::endl;

    std::cout << "alignof(OverAligned): " << alignof(OverAligned) << std::endl;
    std::cout << "sizeof(OverAligned): " << sizeof(OverAligned) << std::endl;

    // Stack allocation respects alignment
    OverAligned oa;
    std::cout << "Address on stack: " << &oa << std::endl;
    std::cout << "Is 32-aligned: " << (reinterpret_cast<uintptr_t>(&oa) % 32 == 0) << std::endl;

    // Heap allocation: need aligned_new in C++17
    // auto* heap = new OverAligned;  // Might not be aligned properly pre-C++17
}

// Aligned dynamic allocation
void AlignedAllocation() {
    std::cout << "\n=== Aligned Dynamic Allocation ===" << std::endl;

    // C++17: aligned new
    OverAligned* ptr = new OverAligned;
    std::cout << "Heap address: " << ptr << std::endl;
    std::cout << "Is 32-aligned: " << (reinterpret_cast<uintptr_t>(ptr) % 32 == 0) << std::endl;

    delete ptr;

    // C++11: std::align
    alignas(32) char buffer[128];
    void* ptr2 = buffer;
    size_t space = sizeof(buffer);

    void* aligned = std::align(32, sizeof(double), ptr2, space);
    if(aligned) {
        std::cout << "std::align found aligned spot: " << aligned << std::endl;
        std::cout << "Is 32-aligned: " << (reinterpret_cast<uintptr_t>(aligned) % 32 == 0) << std::endl;
    }
}

// Alignment in structures with padding
struct BadAlignment {
    char a;      // 1 byte + 7 padding
    double b;    // 8 bytes
    char c;      // 1 byte + 7 padding
    double d;    // 8 bytes
    // Total: 32 bytes
};

struct GoodAlignment {
    double b;    // 8 bytes
    double d;    // 8 bytes
    char a;      // 1 byte
    char c;      // 1 byte + 6 padding
    // Total: 24 bytes (8 bytes wasted vs 16)
    // Wait: 8+8+1+1 = 18, padded to 24? No: 8+8+1+1 = 18, align to 8 = 24
    // Actually: 8+8+1+1 = 18 bytes, padded to multiple of 8 = 24 bytes
};

void StructureAlignment() {
    std::cout << "\n=== Structure Alignment ===" << std::endl;

    std::cout << "sizeof(BadAlignment): " << sizeof(BadAlignment) << std::endl;
    std::cout << "sizeof(GoodAlignment): " << sizeof(GoodAlignment) << std::endl;

    std::cout << "\nBadAlignment offsets:" << std::endl;
    std::cout << "  a: " << offsetof(BadAlignment, a) << std::endl;
    std::cout << "  b: " << offsetof(BadAlignment, b) << std::endl;
    std::cout << "  c: " << offsetof(BadAlignment, c) << std::endl;
    std::cout << "  d: " << offsetof(BadAlignment, d) << std::endl;

    std::cout << "\nGoodAlignment offsets:" << std::endl;
    std::cout << "  b: " << offsetof(GoodAlignment, b) << std::endl;
    std::cout << "  d: " << offsetof(GoodAlignment, d) << std::endl;
    std::cout << "  a: " << offsetof(GoodAlignment, a) << std::endl;
    std::cout << "  c: " << offsetof(GoodAlignment, c) << std::endl;
}

// Hardware-specific alignment requirements
void HardwareAlignment() {
    std::cout << "\n=== Hardware Alignment ===" << std::endl;

    std::cout << "CPU typically requires aligned access for:" << std::endl;
    std::cout << "  - SIMD operations (SSE: 16-byte, AVX: 32-byte)" << std::endl;
    std::cout << "  - Atomic operations" << std::endl;
    std::cout << "  - Some architectures crash on misaligned access" << std::endl;

    std::cout << "\nCommon alignments:" << std::endl;
    std::cout << "  char:   1 (no requirement)" << std::endl;
    std::cout << "  short:  2" << std::endl;
    std::cout << "  int:    4" << std::endl;
    std::cout << "  double: 8" << std::endl;
    std::cout << "  SIMD:   16 or 32" << std::endl;
}

// Custom aligned types for SIMD
struct alignas(16) Vector4 {
    float x, y, z, w;
};

void SIMDAlignment() {
    std::cout << "\n=== SIMD Alignment ===" << std::endl;

    std::cout << "alignof(Vector4): " << alignof(Vector4) << std::endl;
    std::cout << "sizeof(Vector4): " << sizeof(Vector4) << std::endl;

    // Aligned for SSE operations
    Vector4 v{1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "Vector4 address: " << &v << std::endl;
    std::cout << "Is 16-aligned: " << (reinterpret_cast<uintptr_t>(&v) % 16 == 0) << std::endl;

    // Can use with SIMD intrinsics (conceptually)
    std::cout << "Ready for SSE/AVX operations" << std::endl;
}

// Alignment and arrays
void ArrayAlignment() {
    std::cout << "\n=== Array Alignment ===" << std::endl;

    // Array elements are aligned to element alignment
    double arr[4];
    std::cout << "Array address: " << arr << std::endl;
    std::cout << "Is 8-aligned: " << (reinterpret_cast<uintptr_t>(arr) % 8 == 0) << std::endl;

    // Array of over-aligned structs
    alignas(32) OverAligned aligned_arr[2];
    std::cout << "\nOverAligned array address: " << aligned_arr << std::endl;
    std::cout << "Is 32-aligned: " << (reinterpret_cast<uintptr_t>(aligned_arr) % 32 == 0) << std::endl;

    // Note: sizeof(OverAligned) might be padded so each element stays aligned
    std::cout << "sizeof(OverAligned): " << sizeof(OverAligned) << std::endl;
    std::cout << "Element 1 address: " << &aligned_arr[1] << std::endl;
    std::cout << "Element 1 is 32-aligned: " << (reinterpret_cast<uintptr_t>(&aligned_arr[1]) % 32 == 0) << std::endl;
}

// Packed structures (avoiding padding)
#pragma pack(push, 1)
struct PackedStruct {
    char a;      // 1 byte
    int b;       // 4 bytes (no padding!)
    char c;      // 1 byte
    double d;    // 8 bytes (no padding!)
    // Total: 14 bytes
};
#pragma pack(pop)

void PackedStructures() {
    std::cout << "\n=== Packed Structures ===" std::endl;

    std::cout << "sizeof(PackedStruct): " << sizeof(PackedStruct) << std::endl;

    // WARNING: Packed structs can be slower to access
    // Misaligned access may fault on some architectures
    std::cout << "\nUse cases:" << std::endl;
    std::cout << "  - Network protocols (exact byte layout)" << std::endl;
    std::cout << "  - File formats (reading binary data)" << std::endl;
    std::cout << "  - Hardware registers" << std::endl;
}