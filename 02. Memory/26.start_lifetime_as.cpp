#include <iostream>
#include <string>
#include <type_traits>
#include "memory_header.h"

// C++23: std::start_lifetime_as
// Begins lifetime of object in existing memory without initialization
void StartLifetimeAsBasics() {
    std::cout << "=== std::start_lifetime_as (C++23) ===" << std::endl;

    std::cout << "Purpose: Begin object lifetime in existing storage" << std::endl;
    std::cout << "Without: construction/initialization" << std::endl;

    std::cout << "\nSignature:" << std::endl;
    std::cout << "  template<typename T>" << std::endl;
    std::cout << "  T* start_lifetime_as(void* p) noexcept;" << std::endl;

    std::cout << "\nRequirements:" << std::endl;
    std::cout << "  - T must be an implicit-lifetime type" << std::endl;
    std::cout << "  - Properly aligned for T" << std::endl;
    std::cout << "  - No initialization performed" << std::endl;
}

// When this is useful
void UseCases() {
    std::cout << "\n=== Use Cases ===" << std::endl;

    std::cout << "1. Reading binary data:" << std::endl;
    std::cout << "   // Old (UB):" << std::endl;
    std::cout << "   // auto* obj = reinterpret_cast<MyStruct*>(buffer);" << std::endl;
    std::cout << std::endl;
    std::cout << "   // C++23:" << std::endl;
    std::cout << "   // auto* obj = std::start_lifetime_as<MyStruct>(buffer);" << std::endl;
    std::cout << "   // Lifetime officially started!" << std::endl;

    std::cout << "\n2. Memory-mapped files:" << std::endl;
    std::cout << "   void* mapped = mmap(nullptr, size, PROT_READ, ...);" << std::endl;
    std::cout << "   auto* data = std::start_lifetime_as<Header>(mapped);" << std::endl;

    std::cout << "\n3. Network packets:" << std::endl;
    std::cout << "   auto* packet = std::start_lifetime_as<Packet>(raw_bytes);" << std::endl;
    std::cout << "   // Safe to read without memcpy" << std::endl;

    std::cout << "\n4. Deserialization without copy:" << std::endl;
    std::cout << "   // Zero-copy deserialization" << std::endl;
}

// Implicit-lifetime types
void ImplicitLifetimeTypes() {
    std::cout << "\n=== Implicit-Lifetime Types ===" std::endl;

    std::cout << "Implicit-lifetime types include:" << std::endl;
    std::cout << "  - Scalar types (int, float, pointers)" << std::endl;
    std::cout << "  - Arrays of implicit-lifetime types" << std::endl;
    std::cout << "  - Standard layout structs with no user-provided" << std::endl;
    std::cout << "    - constructors" << std::endl;
    std::cout << "    - destructors" << std::endl;
    std::cout << "    - virtual functions" << std::endl;

    struct POD {
        int a;
        double b;
        char c[10];
    };

    std::cout << "\nIs POD implicit-lifetime? "
        << std::is_implicit_lifetime_v<POD> << std::endl;
    std::cout << "Is int implicit-lifetime? "
        << std::is_implicit_lifetime_v<int> << std::endl;
}

// Demonstration (simulated since C++23 support varies)
void Demonstration() {
    std::cout << "\n=== Demonstration ===" << std::endl;

    // Allocate raw memory
    alignas(16) unsigned char buffer[64];

    // Old way (potentially UB):
    // struct Header { uint32_t magic; uint32_t size; };
    // auto* header = reinterpret_cast<Header*>(buffer);
    // Reading header->magic might be UB (no object lifetime started)

    // C++23 way:
    // auto* header = std::start_lifetime_as<Header>(buffer);
    // Now there IS a Header object at that address
    // Reading it is well-defined

    std::cout << "Buffer allocated at: " << static_cast<void*>(buffer) << std::endl;
    std::cout << "With C++23 start_lifetime_as:" << std::endl;
    std::cout << "  Lifetime of object officially begins" << std::endl;
    std::cout << "  Reading through pointer is well-defined" << std::endl;
    std::cout << "  No initialization - bits are as-is" << std::endl;
}

// Comparison with alternatives
void Comparison() {
    std::cout << "\n=== Comparison ===" << std::endl;

    std::cout << "Option 1: memcpy (safe but slow):" << std::endl;
    std::cout << "  Header h;" << std::endl;
    std::cout << "  std::memcpy(&h, buffer, sizeof(Header));" << std::endl;
    std::cout << "  // Well-defined, but copies data" << std::endl;

    std::cout << "\nOption 2: reinterpret_cast (potentially UB):" << std::endl;
    std::cout << "  auto* h = reinterpret_cast<Header*>(buffer);" << std::endl;
    std::cout << "  // Might work, might be UB" << std::endl;
    std::cout << "  // Depends on compiler/optimizations" << std::endl;

    std::cout << "\nOption 3: start_lifetime_as (C++23):" << std::endl;
    std::cout << "  auto* h = std::start_lifetime_as<Header>(buffer);" << std::endl;
    std::cout << "  // Well-defined by standard" << std::endl;
    std::cout << "  // Zero-copy, zero-initialization" << std::endl;
}