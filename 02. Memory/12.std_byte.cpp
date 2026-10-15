#include <iostream>
#include <cstddef>
#include <string>
#include "memory_header.h"

// std::byte is a distinct byte type introduced in C++17
// It's an enum class that can only be used for byte-level operations

void StdByteBasics() {
    std::cout << "=== std::byte Basics ===" << std::endl;

    // Creation
    std::byte b1{42};
    std::byte b2 = std::byte{0xFF};

    std::cout << "sizeof(std::byte): " << sizeof(std::byte) << std::endl;

    // Must convert to integer for arithmetic/output
    std::cout << "b1 as int: " << std::to_integer<int>(b1) << std::endl;
    std::cout << "b2 as int: " << std::to_integer<int>(b2) << std::endl;
}

// std::byte operations
void ByteOperations() {
    std::cout << "\n=== Byte Operations ===" << std::endl;

    std::byte a{0x0F};
    std::byte b{0xF0};

    // Bitwise operations work directly
    std::byte or_result = a | b;
    std::byte and_result = a & b;
    std::byte xor_result = a ^ b;
    std::byte not_a = ~a;

    std::cout << "a | b = 0x" << std::hex << std::to_integer<int>(or_result) << std::endl;
    std::cout << "a & b = 0x" << std::hex << std::to_integer<int>(and_result) << std::endl;
    std::cout << "a ^ b = 0x" << std::hex << std::to_integer<int>(xor_result) << std::endl;
    std::cout << "~a    = 0x" << std::hex << std::to_integer<int>(not_a) << std::endl;

    // Shift operations
    std::byte shifted = a << 2;
    std::cout << "a << 2 = 0x" << std::hex << std::to_integer<int>(shifted) << std::endl;

    // Comparison
    std::cout << "a == b: " << (a == b) << std::endl;
}

// std::byte vs unsigned char
void ByteVsChar() {
    std::cout << "\n=== std::byte vs unsigned char ===" << std::endl;

    std::cout << "unsigned char:" << std::endl;
    std::cout << "  + Can do arithmetic directly" << std::endl;
    std::cout << "  + Implicitly converts to int" << std::endl;
    std::cout << "  + Can be used for both bytes and characters" << std::endl;
    std::cout << "  - No type safety (mixing bytes and chars)" << std::endl;

    std::cout << "\nstd::byte:" << std::endl;
    std::cout << "  + Type-safe: only for byte manipulation" << std::endl;
    std::cout << "  + Cannot accidentally use as character" << std::endl;
    std::cout << "  + Only bitwise operations allowed" << std::endl;
    std::cout << "  - Must use std::to_integer<> for conversion" << std::endl;
    std::cout << "  - No arithmetic (+, -, *, /)" << std::endl;

    // Example: memory buffer
    std::byte buffer[16];
    for(size_t i = 0; i < 16; ++i) {
        buffer[i] = std::byte{static_cast<unsigned char>(i * 16)};
    }

    std::cout << "\nBuffer contents:" << std::endl;
    for(size_t i = 0; i < 16; ++i) {
        std::cout << std::hex << std::to_integer<int>(buffer[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

// Using std::byte for memory manipulation
void MemoryManipulation() {
    std::cout << "\n=== Memory Manipulation ===" << std::endl;

    // Create a buffer
    std::byte memory[32];

    // Write a pattern
    for(size_t i = 0; i < 32; ++i) {
        memory[i] = std::byte{0xAB};
    }

    // Read back
    std::cout << "Memory filled with 0xAB:" << std::endl;
    for(size_t i = 0; i < 8; ++i) {
        std::cout << std::hex << std::to_integer<int>(memory[i]) << " ";
    }
    std::cout << std::dec << "..." << std::endl;

    // Copy memory
    std::byte dest[32];
    std::memcpy(dest, memory, sizeof(memory));

    std::cout << "After memcpy, dest[0] = 0x"
        << std::hex << std::to_integer<int>(dest[0]) << std::dec << std::endl;
}

// std::byte in APIs
void process_bytes(std::byte* data, size_t size) {
    std::cout << "Processing " << size << " bytes:" << std::endl;
    for(size_t i = 0; i < size && i < 8; ++i) {
        std::cout << std::hex << std::to_integer<int>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

void ByteInAPIs() {
    std::cout << "\n=== std::byte in APIs ===" << std::endl;

    std::byte data[] = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
    process_bytes(data, 3);

    // This is clearer than:
    // void process(unsigned char* data, size_t size);
    // Because unsigned char could be a string or actual bytes
}

// std::byte with structured bindings
void ByteWithModernFeatures() {
    std::cout << "\n=== std::byte with Modern Features ===" << std::endl;

    // C++20: std::byte in constexpr contexts
    constexpr std::byte magic = std::byte{0xDE};
    static_assert(std::to_integer<int>(magic) == 0xDE);

    // Using in containers
    std::vector<std::byte> byte_vector;
    for(int i = 0; i < 10; ++i) {
        byte_vector.push_back(std::byte{static_cast<unsigned char>(i)});
    }

    std::cout << "Vector of bytes: ";
    for(const auto& b : byte_vector) {
        std::cout << std::to_integer<int>(b) << " ";
    }
    std::cout << std::endl;

    // Iterating with range-for
    for(const auto& b : byte_vector) {
        if((b & std::byte{0x01}) == std::byte{0x01}) {
            // Odd byte
        }
    }
    std::cout << "Checked for odd bytes" << std::endl;
}