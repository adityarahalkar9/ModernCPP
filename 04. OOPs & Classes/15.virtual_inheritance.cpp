import std;
#include "class_header.h"

// Virtual inheritance to solve diamond problem
class VirtualBase {
public:
    VirtualBase() {
        std::cout << "  VirtualBase constructed" << std::endl;
    }

    VirtualBase(int v) : value(v) {
        std::cout << "  VirtualBase(" << v << ") constructed" << std::endl;
    }

    virtual ~VirtualBase() = default;

    int value = 0;

    void common_method() {
        std::cout << "  VirtualBase::common_method, value = " << value << std::endl;
    }
};

// Virtual inheritance: only ONE VirtualBase in final object
class VirtualDerived1 : virtual public VirtualBase {
public:
    VirtualDerived1() {
        std::cout << "  VirtualDerived1 constructed" << std::endl;
    }

    void method1() {
        std::cout << "  VirtualDerived1::method1" << std::endl;
    }
};

class VirtualDerived2 : virtual public VirtualBase {
public:
    VirtualDerived2() {
        std::cout << "  VirtualDerived2 constructed" << std::endl;
    }

    void method2() {
        std::cout << "  VirtualDerived2::method2" << std::endl;
    }
};

// Final class: only one VirtualBase
class FinalClass : public VirtualDerived1, public VirtualDerived2 {
public:
    // In virtual inheritance, most-derived class calls base constructor
    FinalClass() : VirtualBase(42) {
        std::cout << "  FinalClass constructed" << std::endl;
    }
};

void VirtualInheritanceBasics() {
    std::cout << "=== Virtual Inheritance ===" << std::endl;

    std::cout << "Virtual inheritance ensures:" << std::endl;
    std::cout << "  Only ONE instance of common base" << std::endl;

    std::cout << "\nWith virtual inheritance:" << std::endl;
    std::cout << "      VirtualBase" << std::endl;
    std::cout << "      /       \\" << std::endl;
    std::cout << "  VirtualD1   VirtualD2" << std::endl;
    std::cout << "      \\       /" << std::endl;
    std::cout << "      FinalClass" << std::endl;

    std::cout << "\n(Single VirtualBase shared)" << std::endl;

    FinalClass fc;
    fc.common_method();   // No ambiguity!
    fc.method1();
    fc.method2();

    // Single 'value', no ambiguity
    std::cout << "\nvalue = " << fc.value << std::endl;
    fc.value = 100;
    std::cout << "Modified value = " << fc.value << std::endl;
}

// Without virtual inheritance (for comparison)
class NonVirtualBase {
public:
    int value = 0;
    virtual ~NonVirtualBase() = default;
};

class NVDerived1 : public NonVirtualBase {};
class NVDerived2 : public NonVirtualBase {};

class NVFinal : public NVDerived1, public NVDerived2 {};

void VirtualVsNonVirtual() {
    std::cout << "\n=== Virtual vs Non-Virtual Inheritance ===" << std::endl;

    std::cout << "WITHOUT virtual inheritance:" << std::endl;
    NVFinal nvf;
    // nvf.value = 42;  // AMBIGUOUS!
    nvf.NVDerived1::value = 1;
    nvf.NVDerived2::value = 2;
    std::cout << "  Via D1: " << nvf.NVDerived1::value << std::endl;
    std::cout << "  Via D2: " << nvf.NVDerived2::value << std::endl;
    std::cout << "  TWO separate values!" << std::endl;

    std::cout << "\nWITH virtual inheritance:" << std::endl;
    FinalClass vf;
    vf.value = 42;  // No ambiguity
    std::cout << "  Single value: " << vf.value << std::endl;
}

// Virtual inheritance with constructors
class VB {
public:
    VB(int val) : value(val) {
        std::cout << "  VB(" << val << ")" << std::endl;
    }
    int value;
    virtual ~VB() = default;
};

class VD1 : virtual public VB {
public:
    VD1() : VB(1) {  // Ignored when not most-derived
        std::cout << "  VD1()" << std::endl;
    }
};

class VD2 : virtual public VB {
public:
    VD2() : VB(2) {  // Ignored when not most-derived
        std::cout << "  VD2()" << std::endl;
    }
};

class VFinal : public VD1, public VD2 {
public:
    VFinal() : VB(42) {  // This one is used!
        std::cout << "  VFinal()" << std::endl;
    }
};

void VirtualInheritanceConstructors() {
    std::cout << "\n=== Virtual Inheritance Constructors ===" << std::endl;

    std::cout << "RULE: Most-derived class calls virtual base constructor" << std::endl;
    std::cout << "  VD1's and VD2's calls to VB are IGNORED" << std::endl;
    std::cout << "  VFinal's call to VB is used" << std::endl;

    VFinal vf;
    std::cout << "\nvalue = " << vf.value << std::endl;
}

// Memory layout with virtual inheritance
void VirtualInheritanceLayout() {
    std::cout << "\n=== Virtual Inheritance Memory Layout ===" << std::endl;

    std::cout << "Virtual inheritance uses:" << std::endl;
    std::cout << "  - Virtual base pointers (vbptr)" << std::endl;
    std::cout << "  - Offset tables" << std::endl;
    std::cout << "  - More complex layout" << std::endl;

    std::cout << "\nCosts:" << std::endl;
    std::cout << "  + Solves diamond problem" << std::endl;
    std::cout << "  - More memory overhead" << std::endl;
    std::cout << "  - Slower access to base members" << std::endl;
    std::cout << "  - More complex construction order" << std::endl;

    std::cout << "\nsizeof(VirtualBase) = " << sizeof(VirtualBase) << std::endl;
    std::cout << "sizeof(FinalClass) = " << sizeof(FinalClass) << std::endl;
}

// Iostream hierarchy uses virtual inheritance
void IostreamExample() {
    std::cout << "\n=== Real-World Example: iostream ===" << std::endl;

    std::cout << "std::iostream uses virtual inheritance:" << std::endl;
    std::cout << std::endl;
    std::cout << "        istream    ostream" << std::endl;
    std::cout << "           \\      /" << std::endl;
    std::cout << "          iostream" << std::endl;
    std::endl;
    std::cout << "Both inherit from virtual ios base" << std::endl;

    std::cout << "\nstd::cin is an istream" << std::endl;
    std::cout << "std::cout is an ostream" << std::endl;
    std::cout << "std::stringstream is an iostream" << std::endl;
}