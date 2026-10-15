#include <iostream>
#include <bit>
#include <cstring>
#include <string>
#include "memory_header.h"

// std::bit_cast (C++20): safe type punning
void BasicBitCast() {
    std::cout << "=== Basic std::bit_cast ===" << std::endl;

    // Convert float to int (see the bits)
    float f = 3.14f;
    auto bits = std::bit_cast<uint32_t>(f);

    std::cout << "float " << f << " as bits: 0x"
        << std::hex << bits << std::dec << std::endl;

    // Convert back
    float back = std::bit_cast<float>(bits);
    std::cout << "Back to float: " << back << std::endl;

    // Requirements:
    // 1. Same size (sizeof(To) == sizeof(From))
    // 2. Both must be trivially copyable
    // 3. Must be constexpr-usable

    static_assert(sizeof(float) == sizeof(uint32_t));
}

// Why bit_cast is safer than reinterpret_cast
void WhyBitCastIsSafer() {
    std::cout << "\n=== Why bit_cast is Safer ===" << std::endl;

    std::cout << "Old way (undefined behavior):" << std::endl;
    std::cout << "  float f = 3.14f;" << std::endl;
    std::cout << "  uint32_t bits;" << std::endl;
    std::cout << "  std::memcpy(&bits, &f, 4);  // OK but not constexpr" << std::endl;
    std::cout << "  // OR" << std::endl;
    std::cout << "  bits = *reinterpret_cast<uint32_t*>(&f);  // UB (strict aliasing)" << std::endl;

    std::cout << "\nC++20 way:" << std::endl;
    std::cout << "  auto bits = std::bit_cast<uint32_t>(f);  // Safe, constexpr!" << std::endl;

    // Demonstrate with double
    double d = 2.718281828;
    auto d_bits = std::bit_cast<uint64_t>(d);
    std::cout << "\ndouble " << d << " as bits: 0x"
        << std::hex << d_bits << std::dec << std::endl;

    double d_back = std::bit_cast<double>(d_bits);
    std::cout << "Back to double: " << d_back << std::endl;
}

// bit_cast in constexpr contexts
constexpr uint32_t FloatBits(float f) {
    return std::bit_cast<uint32_t>(f);
}

void ConstexprBitCast() {
    std::cout << "\n=== constexpr bit_cast ===" << std::endl;

    // Can be used at compile time!
    constexpr float pi = 3.14159f;
    constexpr uint32_t pi_bits = FloatBits(pi);

    static_assert(pi_bits != 0);  // Compile-time verification

    std::cout << "PI bits (constexpr): 0x" << std::hex << pi_bits << std::dec << std::endl;
}

// Practical examples
void PracticalExamples() {
    std::cout << "\n=== Practical Examples ===" << std::endl;

    // 1. Inspecting floating point representation
    float value = 1.0f;
    uint32_t bits = std::bit_cast<uint32_t>(value);
    std::cout << "1.0f bits: 0x" << std::hex << bits << std::dec << std::endl;
    // IEEE 754: 1.0 = 0x3F800000

    // 2. Fast inverse square root (Quake III style)
    auto fast_inv_sqrt = [](float x) -> float {
        auto i = std::bit_cast<uint32_t>(x);
        i = 0x5F3759DF - (i >> 1);
        return std::bit_cast<float>(i);
        };

    std::cout << "fast_inv_sqrt(4.0) ≈ " << fast_inv_sqrt(4.0f)
        << " (actual: " << 1.0f / std::sqrt(4.0f) << ")" << std::endl;

    // 3. Hashing by bit pattern
    auto float_hash = [](float f) -> size_t {
        return std::hash<uint32_t>{}(std::bit_cast<uint32_t>(f));
        };

    std::cout << "Hash of 3.14: " << float_hash(3.14f) << std::endl;

    // 4. NaN detection
    float nan = std::numeric_limits<float>::quiet_NaN();
    uint32_t nan_bits = std::bit_cast<uint32_t>(nan);
    std::cout << "NaN bits: 0x" << std::hex << nan_bits << std::dec << std::endl;
    std::cout << "Is NaN pattern: " << ((nan_bits & 0x7F800000) == 0x7F800000
                                        && (nan_bits & 0x007FFFFF) != 0) << std::endl;
}

// bit_cast with custom types
struct Pod {  // Must be trivially copyable
    int a;
    float b;
    char c;
};

void BitCastWithCustomTypes() {
    std::cout << "\n=== bit_cast with Custom Types ===" << std::endl;

    static_assert(std::is_trivially_copyable_v<Pod>);
    static_assert(sizeof(Pod) == 8);  // With padding

    Pod p{42, 3.14f, 'A'};
    auto bytes = std::bit_cast<uint64_t>(p);

    std::cout << "Pod as bits: 0x" << std::hex << bytes << std::dec << std::endl;

    // Back
    Pod p2 = std::bit_cast<Pod>(bytes);
    std::cout << "Recovered: a=" << p2.a << ", b=" << p2.b << ", c=" << p2.c << std::endl;
}

// Comparing bit patterns
void ComparingBitPatterns() {
    std::cout << "\n=== Comparing Bit Patterns ===" << std::endl;

    // -0.0 and 0.0 are equal as floats, but different bit patterns!
    float pos_zero = 0.0f;
    float neg_zero = -0.0f;

    std::cout << "pos_zero == neg_zero: " << (pos_zero == neg_zero) << std::endl;

    auto pos_bits = std::bit_cast<uint32_t>(pos_zero);
    auto neg_bits = std::bit_cast<uint32_t>(neg_zero);

    std::cout << "pos_zero bits: 0x" << std::hex << pos_bits << std::dec << std::endl;
    std::cout << "neg_zero bits: 0x" << std::hex << neg_bits << std::dec << std::endl;
    std::cout << "Bits are different: " << (pos_bits != neg_bits) << std::endl;
}