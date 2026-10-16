import std;
#include "structs_header.h"

// C99 style flexible array member
struct FlexHeader {
    uint32_t length;
    char data[];  // Flexible array member (C99, GCC/Clang extension in C++)
};

// C++ approach using single member
struct FlexHeaderCpp {
    uint32_t length;
    char data[1];  // Hack: allocate more than sizeof
};

void FlexibleArrayBasics() {
    std::cout << "=== Flexible Array Members ===" << std::endl;

    std::cout << "sizeof(FlexHeader) = " << sizeof(FlexHeader) << std::endl;
    std::cout << "sizeof(FlexHeaderCpp) = " << sizeof(FlexHeaderCpp) << std::endl;

    std::cout << "\nFlexible array member:" << std::endl;
    std::cout << "  data[] has zero size in struct" << std::endl;
    std::cout << "  Allocate extra memory after struct" << std::endl;
    std::cout << "  Access as array of arbitrary length" << std::endl;
}

// Using flexible array members
void UsingFlexibleArray() {
    std::cout << "\n=== Using Flexible Array Member ===" << std::endl;

    // Allocate memory for header + data
    size_t data_length = 100;
    size_t total_size = sizeof(FlexHeader) + data_length;

    FlexHeader* header = static_cast<FlexHeader*>(std::malloc(total_size));
    header->length = data_length;

    // Fill data
    for(size_t i = 0; i < data_length; ++i) {
        header->data[i] = static_cast<char>('A' + i % 26);
    }

    std::cout << "Length: " << header->length << std::endl;
    std::cout << "First 10 chars: ";
    for(size_t i = 0; i < 10; ++i) {
        std::cout << header->data[i];
    }
    std::cout << std::endl;

    std::free(header);
}

// C++ style with vector
struct DynamicData {
    std::vector<char> data;

    DynamicData(size_t size) : data(size) {}

    char& operator[](size_t index) { return data[index]; }
    const char& operator[](size_t index) const { return data[index]; }

    size_t size() const { return data.size(); }
};

void ModernAlternative() {
    std::cout << "\n=== Modern C++ Alternative ===" << std::endl;

    DynamicData dd(50);
    for(size_t i = 0; i < dd.size(); ++i) {
        dd[i] = static_cast<char>('a' + i % 26);
    }

    std::cout << "Size: " << dd.size() << std::endl;
    std::cout << "Contents: ";
    for(size_t i = 0; i < 10; ++i) {
        std::cout << dd[i];
    }
    std::cout << "..." << std::endl;

    std::cout << "\nPrefer std::vector in modern C++:" << std::endl;
    std::cout << "  + Automatic memory management" << std::endl;
    std::cout << "  + Bounds checking with .at()" << std::endl;
    std::cout << "  + Exception safety" << std::endl;
    std::cout << "  + Standard container interface" << std::endl;
}

// When flexible arrays are still used
struct Packet {
    uint16_t type;
    uint16_t length;
    uint8_t payload[];
};

void UseCases() {
    std::cout << "\n=== Use Cases for Flexible Arrays ===" << std::endl;

    std::cout << "1. Network packets:" << std::endl;
    std::cout << "   struct Packet {" << std::endl;
    std::cout << "       uint16_t type;" << std::endl;
    std::cout << "       uint16_t length;" << std::endl;
    std::cout << "       uint8_t payload[];" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\n2. Variable-length strings:" << std::endl;
    std::cout << "   struct String {" << std::endl;
    std::cout << "       size_t length;" << std::endl;
    std::cout << "       char data[];" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\n3. Memory-mapped files:" << std::endl;
    std::cout << "   struct FileHeader {" << std::endl;
    std::cout << "       uint32_t magic;" << std::endl;
    std::cout << "       uint32_t count;" << std::endl;
    std::cout << "       Entry entries[];" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\n4. Kernel/OS structures:" << std::endl;
    std::cout << "   struct SyscallMessage {" << std::endl;
    std::cout << "       int syscall_number;" << std::endl;
    std::cout << "       long args[];" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\nNote: This is a C technique" << std::endl;
    std::cout << "In C++, prefer vectors or std::string" << std::endl;
}

// Dangers and alternatives
void DangersAndAlternatives() {
    std::cout << "\n=== Dangers and Alternatives ===" << std::endl;

    std::cout << "Dangers of flexible arrays:" << std::endl;
    std::cout << "  1. Manual memory management required" << std::endl;
    std::cout << "  2. No bounds checking" << std::endl;
    std::cout << "  3. Easy to have buffer overflows" << std::endl;
    std::cout << "  4. Not standard C++ (compiler extension)" << std::endl;
    std::cout << "  5. Can't use with std::vector directly" << std::endl;

    std::cout << "\nSafer alternatives:" << std::endl;
    std::cout << "  1. std::vector<T>" << std::endl;
    std::cout << "  2. std::string for char data" << std::endl;
    std::cout << "  3. Unique_ptr with custom deleter" << std::endl;
    std::cout << "  4. std::array for fixed size" << std::endl;

    std::cout << "\nIf you must use flexible arrays:" << std::endl;
    std::cout << "  - Wrap in RAII class" << std::endl;
    std::cout << "  - Validate sizes carefully" << std::endl;
    std::cout << "  - Use static analysis tools" << std::endl;
}