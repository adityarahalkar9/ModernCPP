#include "structs_header.h"
import std;

// Standard layout types
struct StandardLayout {
    int a;
    double b;
};

struct NotStandardLayout1 {
private:
    int a;      // Private + public members
public:
    double b;
};

struct NotStandardLayout2 : StandardLayout {
    int c;  // Two classes with data members
};

struct NotStandardLayout3 {
    virtual void foo() {}  // Virtual function
    int a;
};

void StandardLayoutBasics() {
    std::cout << "=== Standard Layout Types ===" << std::endl;

    static_assert(std::is_standard_layout_v<StandardLayout>);
    static_assert(!std::is_standard_layout_v<NotStandardLayout1>);
    static_assert(!std::is_standard_layout_v<NotStandardLayout2>);
    static_assert(!std::is_standard_layout_v<NotStandardLayout3>);

    std::cout << "Standard layout requirements:" << std::endl;
    std::cout << "  1. No virtual functions or virtual base classes" << std::endl;
    std::cout << "  2. All non-static data members have same access control" << std::endl;
    std::cout << "  3. No non-standard-layout base classes" << std::endl;
    std::cout << "  4. Only one class in hierarchy has non-static data members" << std::endl;
}

// Why standard layout matters
void WhyStandardLayout() {
    std::cout << "\n=== Why Standard Layout Matters ===" << std::endl;

    std::cout << "Standard layout guarantees:" << std::endl;
    std::cout << "  - Predictable memory layout" << std::endl;
    std::cout << "  - C language compatibility" << std::endl;
    std::cout << "  - Can use offsetof() safely" << std::endl;
    std::cout << "  - reinterpret_cast between related types" << std::endl;

    std::cout << "\nUse cases:" << std::endl;
    std::cout << "  - Shared memory structures" << std::endl;
    std::cout << "  - Network protocols" << std::endl;
    std::cout << "  - File formats" << std::endl;
    std::cout << "  - Hardware registers" << std::endl;
    std::cout << "  - C library interop" << std::endl;
}

// offsetof with standard layout
struct WithOffsets {
    int first;
    double second;
    char third;
};

void OffsetofDemo() {
    std::cout << "\n=== offsetof() Demo ===" std::endl;

    static_assert(std::is_standard_layout_v<WithOffsets>);

    std::cout << "offsetof(WithOffsets, first) = " << offsetof(WithOffsets, first) << std::endl;
    std::cout << "offsetof(WithOffsets, second) = " << offsetof(WithOffsets, second) << std::endl;
    std::cout << "offsetof(WithOffsets, third) = " << offsetof(WithOffsets, third) << std::endl;

    std::cout << "\noffsetof only works with standard-layout types!" << std::endl;
    std::cout << "For non-standard-layout, use is undefined behavior" << std::endl;
}

// Accessing via byte offsets
void ByteOffsetAccess() {
    std::cout << "\n=== Access via Byte Offsets ===" << std::endl;

    WithOffsets wo{42, 3.14, 'A'};

    // Get address of each member
    void* base = &wo;
    void* first_addr = static_cast<char*>(base) + offsetof(WithOffsets, first);
    void* second_addr = static_cast<char*>(base) + offsetof(WithOffsets, second);
    void* third_addr = static_cast<char*>(base) + offsetof(WithOffsets, third);

    // Read through addresses
    std::cout << "first = " << *static_cast<int*>(first_addr) << std::endl;
    std::cout << "second = " << *static_cast<double*>(second_addr) << std::endl;
    std::cout << "third = " << *static_cast<char*>(third_addr) << std::endl;

    std::cout << "\nThis technique is used in:" << std::endl;
    std::cout << "  - Serialization frameworks" << std::endl;
    std::cout << "  - ORMs" << std::endl;
    std::cout << "  - Network protocols" << std::endl;
}

// Common patterns
void CommonPatterns() {
    std::cout << "\n=== Common Patterns ===" << std::endl;

    std::cout << "1. C-compatible structs:" << std::endl;
    std::cout << "   struct Message {" << std::endl;
    std::cout << "       uint32_t magic;" << std::endl;
    std::cout << "       uint16_t version;" << std::endl;
    std::cout << "       uint16_t length;" << std::endl;
    std::cout << "       char data[256];" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\n2. Tagged unions:" << std::endl;
    std::cout << "   struct Variant {" << std::endl;
    std::cout << "       enum Type { INT, DOUBLE, PTR } type;" << std::endl;
    std::cout << "       union {" << std::endl;
    std::cout << "           int i;" << std::endl;
    std::cout << "           double d;" << std::endl;
    std::cout << "           void* p;" << std::endl;
    std::cout << "       };" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "\n3. Hardware register maps:" << std::endl;
    std::cout << "   struct Registers {" << std::endl;
    std::cout << "       volatile uint32_t control;" << std::endl;
    std::cout << "       volatile uint32_t status;" << std::endl;
    std::cout << "       volatile uint32_t data;" << std::endl;
    std::cout << "   };" << std::endl;
}