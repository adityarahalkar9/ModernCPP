#include <iostream>
#include <string>
#include <cstring>
#include "structs_header.h"

// Basic bit fields
struct Flags {
    unsigned int flag1 : 1;   // 1 bit
    unsigned int flag2 : 1;   // 1 bit
    unsigned int mode : 2;   // 2 bits (0-3)
    unsigned int count : 4;   // 4 bits (0-15)
    // Total: 8 bits, but struct will be sizeof(unsigned int) = 4 bytes
};

void BasicBitFields() {
    std::cout << "=== Basic Bit Fields ===" << std::endl;

    std::cout << "sizeof(Flags) = " << sizeof(Flags) << " bytes" << std::endl;

    Flags f{};
    f.flag1 = 1;
    f.flag2 = 0;
    f.mode = 2;      // Max value: 3 (2 bits)
    f.count = 15;    // Max value: 15 (4 bits)

    std::cout << "f.flag1 = " << f.flag1 << std::endl;
    std::cout << "f.flag2 = " << f.flag2 << std::endl;
    std::cout << "f.mode = " << f.mode << std::endl;
    std::cout << "f.count = " << f.count << std::endl;

    // Overflow wraps around (implementation-defined)
    f.count = 20;  // 20 doesn't fit in 4 bits
    std::cout << "f.count (overflowed) = " << f.count << std::endl;
}

// Bit fields for flags
struct CPUFlags {
    bool zero : 1;
    bool sign : 1;
    bool carry : 1;
    bool overflow : 1;
    // 4 bits used, but may take 4 bytes
};

void FlagBitFields() {
    std::cout << "\n=== Flags with Bit Fields ===" << std::endl;

    std::cout << "sizeof(CPUFlags) = " << sizeof(CPUFlags) << " bytes" << std::endl;

    CPUFlags flags{};
    flags.zero = true;
    flags.sign = false;
    flags.carry = true;
    flags.overflow = false;

    std::cout << "Zero:     " << flags.zero << std::endl;
    std::cout << "Sign:     " << flags.sign << std::endl;
    std::cout << "Carry:    " << flags.carry << std::endl;
    std::cout << "Overflow: " << flags.overflow << std::endl;
}

// Packing multiple values
struct PackedDate {
    unsigned int day : 5;    // 1-31 (needs 5 bits)
    unsigned int month : 4;    // 1-12 (needs 4 bits)
    unsigned int year : 12;   // 0-4095 (needs 12 bits)
    // Total: 21 bits
};

void PackedValues() {
    std::cout << "\n=== Packing Multiple Values ===" << std::endl;

    std::cout << "sizeof(PackedDate) = " << sizeof(PackedDate) << " bytes" << std::endl;

    PackedDate date{};
    date.day = 25;
    date.month = 12;
    date.year = 2024;

    std::cout << "Date: " << date.day << "/" << date.month << "/" << date.year << std::endl;

    // Maximum values
    std::cout << "\nMaximum values:" << std::endl;
    std::cout << "  day: " << ((1 << 5) - 1) << " (5 bits)" << std::endl;
    std::cout << "  month: " << ((1 << 4) - 1) << " (4 bits)" << std::endl;
    std::cout << "  year: " << ((1 << 12) - 1) << " (12 bits)" << std::endl;
}

// Bit fields for hardware registers
struct HardwareRegister {
    volatile unsigned int enable : 1;
    volatile unsigned int interrupt : 1;
    volatile unsigned int mode : 2;
    volatile unsigned int reserved : 4;
    volatile unsigned int address : 24;
};

void HardwareRegisters() {
    std::cout << "\n=== Hardware Register Bit Fields ===" << std::endl;

    std::cout << "Hardware registers often use bit fields:" << std::endl;
    std::cout << "sizeof(HardwareRegister) = " << sizeof(HardwareRegister) << std::endl;

    std::cout << "\nLayout:" << std::endl;
    std::cout << "  enable:    bit 0" << std::endl;
    std::cout << "  interrupt: bit 1" << std::endl;
    std::cout << "  mode:      bits 2-3" << std::endl;
    std::cout << "  reserved:  bits 4-7" << std::endl;
    std::cout << "  address:   bits 8-31" << std::endl;
}

// Limitations and pitfalls
void BitFieldPitfalls() {
    std::cout << "\n=== Bit Field Pitfalls ===" << std::endl;

    std::cout << "1. Implementation-defined bit order:" << std::endl;
    std::cout << "   - Bits may be allocated from low or high end" << std::endl;
    std::cout << "   - Varies between compilers/platforms" << std::endl;

    std::cout << "\n2. No pointer to bit field:" << std::endl;
    std::cout << "   unsigned int Flags::*ptr = &Flags::flag1;  // ERROR!" << std::endl;

    std::cout << "\n3. Can't take address:" << std::endl;
    std::cout << "   // auto* addr = &f.flag1;  // ERROR!" << std::endl;

    std::cout << "\n4. sizeof may be larger than expected:" << std::endl;
    struct Small {
        unsigned int a : 1;
    };
    std::cout << "   sizeof(struct with 1 bit) = " << sizeof(Small)
        << " (at least sizeof(unsigned int))" << std::endl;

    std::cout << "\n5. Signed bit fields are tricky:" << std::endl;
    std::cout << "   int signed_field : 3;  // Can hold -4 to 3" << std::endl;

    std::cout << "\n6. Thread safety:" << std::endl;
    std::cout << "   - Adjacent bit fields may be in same word" << std::endl;
    std::cout << "   - No atomic operations on individual bits" << std::endl;
}

// Alternative: manual bit manipulation
struct ManualBits {
    uint32_t value;

    bool get_bit(int n) const {
        return (value >> n) & 1;
    }

    void set_bit(int n) {
        value |= (1u << n);
    }

    void clear_bit(int n) {
        value &= ~(1u << n);
    }

    uint32_t get_field(int shift, int width) const {
        uint32_t mask = (1u << width) - 1;
        return (value >> shift) & mask;
    }

    void set_field(int shift, int width, uint32_t val) {
        uint32_t mask = (1u << width) - 1;
        value = (value & ~(mask << shift)) | ((val & mask) << shift);
    }
};

void ManualBitManipulation() {
    std::cout << "\n=== Manual Bit Manipulation ===" << std::endl;

    ManualBits mb{0};

    mb.set_bit(0);
    mb.set_bit(3);
    std::cout << "After setting bits 0 and 3: 0x" << std::hex << mb.value << std::dec << std::endl;

    mb.set_field(4, 4, 0xA);  // Set 4-bit field at position 4 to 0xA
    std::cout << "After setting field: 0x" << std::hex << mb.value << std::dec << std::endl;

    std::cout << "\nManual approach:" << std::endl;
    std::cout << "  + Predictable layout" << std::endl;
    std::cout << "  + Portable across compilers" << std::endl;
    std::cout << "  + Can take addresses of words" << std::endl;
    std::cout << "  - More verbose" << std::endl;
}