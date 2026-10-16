import std;
#include "structs_header.h"

// Anonymous struct (unnamed)
struct WithAnonymous {
    int named;
    struct {           // Anonymous struct
        int x;
        int y;
    };                  // No name!
    // Access x and y directly as members of WithAnonymous
};

void BasicAnonymousStruct() {
    std::cout << "=== Anonymous Structs ===" << std::endl;

    WithAnonymous obj{};
    obj.named = 10;
    obj.x = 20;  // Direct access, no intermediate name
    obj.y = 30;

    std::cout << "named = " << obj.named << std::endl;
    std::cout << "x = " << obj.x << std::endl;
    std::cout << "y = " << obj.y << std::endl;

    std::cout << "\nsizeof(WithAnonymous) = " << sizeof(WithAnonymous) << std::endl;
}

// Anonymous unions
struct WithAnonymousUnion {
    enum Type { INT, DOUBLE, PTR } type;

    union {  // Anonymous union
        int i;
        double d;
        void* p;
    };
};

void AnonymousUnion() {
    std::cout << "\n=== Anonymous Unions ===" << std::endl;

    WithAnonymousUnion obj{};
    obj.type = WithAnonymousUnion::INT;
    obj.i = 42;  // Direct access

    std::cout << "As int: " << obj.i << std::endl;

    obj.type = WithAnonymousUnion::DOUBLE;
    obj.d = 3.14;
    std::cout << "As double: " << obj.d << std::endl;

    std::cout << "\nsizeof(WithAnonymousUnion) = " << sizeof(WithAnonymousUnion) << std::endl;
    std::cout << "(union members share same memory)" << std::endl;
}

// Tagged unions with anonymous
struct Value {
    enum Kind { Number, Text, Boolean } kind;

    union {
        double number;
        // Can't have std::string in anonymous union (non-trivial destructor)
        // std::string text;  // ERROR in anonymous union
        bool boolean;
    };

    static Value make_number(double d) {
        Value v{};
        v.kind = Number;
        v.number = d;
        return v;
    }

    static Value make_boolean(bool b) {
        Value v{};
        v.kind = Boolean;
        v.boolean = b;
        return v;
    }
};

void TaggedUnion() {
    std::cout << "\n=== Tagged Unions ===" << std::endl;

    auto v1 = Value::make_number(3.14);
    auto v2 = Value::make_boolean(true);

    std::cout << "v1 is number: " << v1.number << std::endl;
    std::cout << "v2 is boolean: " << v2.boolean << std::endl;

    // Type-safe access
    auto print_value = [](const Value& v) {
        switch(v.kind) {
            case Value::Number:
                std::cout << "Number: " << v.number << std::endl;
                break;
            case Value::Boolean:
                std::cout << "Boolean: " << v.boolean << std::endl;
                break;
            default:
                std::cout << "Unknown kind" << std::endl;
        }
        };

    print_value(v1);
    print_value(v2);
}

// Nested anonymous structs
struct Nested {
    struct {  // First anonymous
        struct {  // Nested anonymous
            int deep;
        };
        int middle;
    };
    int outer;
};

void NestedAnonymous() {
    std::cout << "\n=== Nested Anonymous Structs ===" << std::endl;

    Nested n{};
    n.outer = 1;
    n.middle = 2;
    n.deep = 3;

    std::cout << "outer = " << n.outer << std::endl;
    std::cout << "middle = " << n.middle << std::endl;
    std::cout << "deep = " << n.deep << std::endl;

    std::cout << "\nNote: Deeply nested anonymous structs" << std::endl;
    std::cout << "can be confusing - use sparingly!" << std::endl;
}

// Anonymous structs in unions
union WithStruct {
    struct {
        short low;
        short high;
    };
    int full;

    WithStruct(int v = 0) : full(v) {}
};

void AnonymousStructInUnion() {
    std::cout << "\n=== Anonymous Struct in Union ===" << std::endl;

    WithStruct u(0x12345678);

    std::cout << std::hex;
    std::cout << "full = 0x" << u.full << std::endl;
    std::cout << "high = 0x" << u.high << std::endl;
    std::cout << "low = 0x" << u.low << std::endl;
    std::cout << std::dec;

    // Modify parts
    u.low = 0xABCD;
    std::cout << "\nAfter changing low:" << std::endl;
    std::cout << std::hex << "full = 0x" << u.full << std::dec << std::endl;
}

// Limitations and warnings
void LimitationsAndWarnings() {
    std::cout << "\n=== Limitations and Warnings ===" << std::endl;

    std::cout << "1. Anonymous structs are NOT standard C++:" << std::endl;
    std::cout << "   - They're a compiler extension" << std::endl;
    std::cout << "   - Work in GCC, Clang, MSVC" << std::endl;
    std::cout << "   - May not work on other compilers" << std::endl;

    std::cout << "\n2. Anonymous unions ARE standard:" << std::endl;
    std::cout << "   - Part of C++ since C++11" << std::endl;
    std::cout << "   - Members must be trivial (no constructors/destructors)" << std::endl;

    std::cout << "\n3. Use sparingly:" << std::endl;
    std::cout << "   - Can make code harder to understand" << std::endl;
    std::cout << "   - Debugging can be tricky" << std::endl;
    std::cout << "   - Prefer named members for clarity" << std::endl;
}