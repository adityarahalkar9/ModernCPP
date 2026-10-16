#include <iostream>
#include <string>
#include <type_traits>
#include <cstring>
#include "structs_header.h"

// POD (Plain Old Data) types
struct POD {
    int a;
    double b;
    char c[10];
};

struct NotPOD {
    std::string str;  // Has non-trivial members
    int a;
};

struct HasConstructor {
    HasConstructor() : x(0) {}  // User-provided constructor
    int x;
};

struct HasDestructor {
    ~HasDestructor() {}  // User-provided destructor
    int x;
};

void PODBasics() {
    std::cout << "=== POD Types ===" << std::endl;

    static_assert(std::is_pod_v<POD>);
    static_assert(!std::is_pod_v<NotPOD>);
    static_assert(!std::is_pod_v<HasConstructor>);
    static_assert(!std::is_pod_v<HasDestructor>);

    std::cout << "POD requirements:" << std::endl;
    std::cout << "  1. Trivial type" << std::endl;
    std::cout << "  2. Standard-layout type" << std::endl;

    std::cout << "\nPOD benefits:" << std::endl;
    std::cout << "  - Can be memcpy'd" << std::endl;
    std::cout << "  - Compatible with C" << std::endl;
    std::cout << "  - Can be in static memory" << std::endl;
    std::cout << "  - Guaranteed byte-for-byte copy" << std::endl;
}

// Trivial types
struct Trivial {
    int a;
    double b;
};

struct NotTrivial1 {
    NotTrivial1() {}  // User-provided constructor (not defaulted)
    int a;
};

struct NotTrivial2 {
    std::string s;  // Has non-trivial member
};

void TrivialTypes() {
    std::cout << "\n=== Trivial Types ===" << std::endl;

    static_assert(std::is_trivial_v<Trivial>);
    static_assert(!std::is_trivial_v<NotTrivial1>);
    static_assert(!std::is_trivial_v<NotTrivial2>);

    std::cout << "Trivial requirements:" << std::endl;
    std::cout << "  1. Trivial default constructor" << std::endl;
    std::cout << "  2. Trivial copy constructor" << std::endl;
    std::cout << "  3. Trivial move constructor" << std::endl;
    std::cout << "  4. Trivial copy assignment" << std::endl;
    std::cout << "  5. Trivial move assignment" << std::endl;
    std::cout << "  6. Trivial destructor" << std::endl;

    std::cout << "\nTrivial means:" << std::endl;
    std::cout << "  - Compiler-generated special functions" << std::endl;
    std::cout << "  - No custom logic in construction/destruction" << std::endl;
    std::cout << "  - Can be treated as raw memory" << std::endl;
}

// Trivially copyable (slightly different from trivial)
struct TriviallyCopyable {
    TriviallyCopyable() {}  // Non-trivial constructor is OK!
    int a;
    double b;
};

void TriviallyCopyableTypes() {
    std::cout << "\n=== Trivially Copyable Types ===" << std::endl;

    static_assert(std::is_trivially_copyable_v<TriviallyCopyable>);
    static_assert(!std::is_trivial_v<TriviallyCopyable>);

    std::cout << "Trivially copyable requirements:" << std::endl;
    std::cout << "  - Trivial copy/move constructors" << std::endl;
    std::cout << "  - Trivial copy/move assignments" << std::endl;
    std::cout << "  - Trivial destructor" << std::endl;
    std::cout << "  (Constructor can be non-trivial!)" << std::endl;

    std::cout << "\nWhat this enables:" << std::endl;
    std::cout << "  - memcpy/memmove operations" << std::endl;
    std::cout << "  - Serialization to bytes" << std::endl;
    std::cout << "  - Binary I/O" << std::endl;
}

// Practical usage
void PracticalUsage() {
    std::cout << "\n=== Practical Usage ===" << std::endl;

    POD p1{42, 3.14, "hello"};
    POD p2;

    // Safe to memcpy (POD is trivially copyable)
    std::memcpy(&p2, &p1, sizeof(POD));

    std::cout << "After memcpy:" << std::endl;
    std::cout << "  p2.a = " << p2.a << std::endl;
    std::cout << "  p2.b = " << p2.b << std::endl;
    std::cout << "  p2.c = " << p2.c << std::endl;

    // Serialization to bytes
    unsigned char bytes[sizeof(POD)];
    std::memcpy(bytes, &p1, sizeof(POD));

    std::cout << "\nSerialized bytes (first 8): ";
    for(int i = 0; i < 8; ++i) {
        std::cout << std::hex << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    // Deserialization
    POD p3;
    std::memcpy(&p3, bytes, sizeof(POD));
    std::cout << "\nDeserialized: a=" << p3.a << ", b=" << p3.b << std::endl;
}

// C compatibility
extern "C" {
    struct CStruct {  // C-compatible struct
        int x;
        double y;
    };
}

void CCompatibility() {
    std::cout << "\n=== C Compatibility ===" << std::endl;

    static_assert(std::is_pod_v<CStruct>);

    std::cout << "POD structs can be passed to C code:" << std::endl;
    std::cout << "  - Same memory layout" << std::endl;
    std::cout << "  - No name mangling issues" << std::endl;
    std::cout << "  - No construction/destruction semantics needed" << std::endl;

    std::cout << "\nFor C interop, ensure:" << std::endl;
    std::cout << "  - Use POD types" << std::endl;
    std::cout << "  - Avoid virtual functions" << std::endl;
    std::cout << "  - Avoid inheritance" << std::endl;
    std::cout << "  - Use standard layout" << std::endl;
}