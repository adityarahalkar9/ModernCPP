#include <iostream>
#include <bit>
#include <cstdint>
#include <string>
#include "memory_header.h"

// std::endian (C++20)
void BasicEndian() {
    std::cout << "=== std::endian (C++20) ===" << std::endl;

    if constexpr(std::endian::native == std::endian::little) {
        std::cout << "This machine is LITTLE-endian" << std::endl;
        std::cout << "Least significant byte first" << std::endl;
    }
    else if constexpr(std::endian::native == std::endian::big) {
        std::cout << "This machine is BIG-endian" << std::endl;
        std::cout << "Most significant byte first" << std::endl;
    }
    else {
        std::cout << "Mixed/other endianness" << std::endl;
    }

    std::cout << "\nstd::endian::native: " << static_cast<int>(std::endian::native) << std::endl;
    std::cout << "std::endian::little: " << static_cast<int>(std::endian::little) << std::endl;
    std::cout << "std::endian::big:    " << static_cast<int>(std::endian::big) << std::endl;
}

// Detecting byte order manually (pre-C++20 method)
void ManualEndianDetection() {
    std::cout << "\n=== Manual Endian Detection ===" << std::endl;

    uint32_t value = 0x12345678;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);

    std::cout << "Value 0x12345678 stored as bytes:" << std::endl;
    for(size_t i = 0; i < 4; ++i) {
        std::cout << "  byte[" << i << "] = 0x"
            << std::hex << static_cast<int>(bytes[i]) << std::dec << std::endl;
    }

    if(bytes[0] == 0x78) {
        std::cout << "-> Little-endian (LSB first)" << std::endl;
    }
    else if(bytes[0] == 0x12) {
        std::cout << "-> Big-endian (MSB first)" << std::endl;
    }
}

// Byte swapping
void ByteSwapping() {
    std::cout << "\n=== Byte Swapping ===" << std::endl;

    uint16_t val16 = 0x1234;
    uint32_t val32 = 0x12345678;

    // Manual swap (16-bit)
    uint16_t swap16 = static_cast<uint16_t>(
        (val16 << 8) | (val16 >> 8)
        );
    std::cout << std::hex;
    std::cout << "0x" << val16 << " swapped -> 0x" << swap16 << std::endl;

    // Manual swap (32-bit)
    uint32_t swap32 =
        ((val32 & 0xFF000000) >> 24) |
        ((val32 & 0x00FF0000) >> 8) |
        ((val32 & 0x0000FF00) << 8) |
        ((val32 & 0x000000FF) << 24);
    std::cout << "0x" << val32 << " swapped -> 0x" << swap32 << std::endl;

    std::cout << std::dec;
}

// Endianness in network communication
void NetworkByteOrder() {
    std::cout << "\n=== Network Byte Order ===" << std::endl;

    std::cout << "Network protocols use BIG-endian" << std::endl;
    std::cout << "x86/x64 CPUs are LITTLE-endian" << std::endl;

    // Simulate: host (little) to network (big)
    uint32_t host_value = 0x12345678;

    // Convert to network order
    uint32_t network_value;
    if constexpr(std::endian::native == std::endian::little) {
        // Swap bytes
        network_value =
            ((host_value & 0xFF000000) >> 24) |
            ((host_value & 0x00FF0000) >> 8) |
            ((host_value & 0x0000FF00) << 8) |
            ((host_value & 0x000000FF) << 24);
    }
    else {
        network_value = host_value;  // Already big-endian
    }

    std::cout << std::hex;
    std::cout << "Host value:    0x" << host_value << std::endl;
    std::cout << "Network value: 0x" << network_value << std::endl;
    std::cout << std::dec;

    // In real code: use htonl(), ntohl(), htons(), ntohs()
}

// Writing endian-independent code
void EndianIndependentCode() {
    std::cout << "\n=== Endian-Independent Code ===" << std::endl;

    // Method 1: Use arithmetic (always works)
    uint32_t value = 0xDEADBEEF;
    std::cout << std::hex << "Value: 0x" << value << std::dec << std::endl;

    // Method 2: Explicit byte serialization
    auto serialize_be = [](uint32_t v, uint8_t* out) {
        out[0] = static_cast<uint8_t>(v >> 24);
        out[1] = static_cast<uint8_t>(v >> 16);
        out[2] = static_cast<uint8_t>(v >> 8);
        out[3] = static_cast<uint8_t>(v);
        };

    auto deserialize_be = [](const uint8_t* in) -> uint32_t {
        return (static_cast<uint32_t>(in[0]) << 24) |
            (static_cast<uint32_t>(in[1]) << 16) |
            (static_cast<uint32_t>(in[2]) << 8) |
            static_cast<uint32_t>(in[3]);
        };

    uint8_t buffer[4];
    serialize_be(value, buffer);

    std::cout << "Serialized (big-endian): ";
    for(uint8_t b : buffer) {
        std::cout << std::hex << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;

    uint32_t recovered = deserialize_be(buffer);
    std::cout << "Deserialized: 0x" << std::hex << recovered << std::dec << std::endl;
}

// Endianness with std::bit_cast
void EndianWithBitCast() {
    std::cout << "\n=== Endianness + bit_cast ===" << std::endl;

    // Examine how an integer is stored in memory
    uint32_t num = 0x01020304;
    auto bits = std::bit_cast<std::array<uint8_t, 4>>(num);

    std::cout << "uint32_t 0x01020304 in memory:" << std::endl;
    std::cout << std::hex;
    for(size_t i = 0; i < 4; ++i) {
        std::cout << "  [" << i << "] = 0x" << static_cast<int>(bits[i]) << std::endl;
    }
    std::cout << std::dec;

    if(bits[0] == 0x04) {
        std::cout << "-> Little-endian (LSB at lowest address)" << std::endl;
    }
    else {
        std::cout << "-> Big-endian (MSB at lowest address)" << std::endl;
    }
}