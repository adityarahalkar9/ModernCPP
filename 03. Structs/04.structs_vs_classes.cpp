#include <iostream>
#include <string>
#include "structs_header.h"

// Same struct/class with different default access
struct StructVersion {
    int x;               // Public by default
    void method() {      // Public by default
        std::cout << "Struct method: " << x << std::endl;
    }
};

class ClassVersion {
public:  // Must explicitly make public
    int x;
    void method() {
        std::cout << "Class method: " << x << std::endl;
    }
};

void DefaultAccessDifference() {
    std::cout << "=== Default Access ===" << std::endl;

    StructVersion s{42};
    s.method();
    std::cout << "s.x (public): " << s.x << std::endl;

    ClassVersion c;
    c.x = 24;
    c.method();
    std::cout << "c.x (public): " << c.x << std::endl;

    std::cout << "\nKey difference:" << std::endl;
    std::cout << "  struct: members are PUBLIC by default" << std::endl;
    std::cout << "  class:  members are PRIVATE by default" << std::endl;
}

// Default inheritance access
struct DerivedStruct : StructVersion {};      // Public inheritance
class DerivedClass : public ClassVersion {};  // Must specify public

void InheritanceAccessDifference() {
    std::cout << "\n=== Default Inheritance ===" << std::endl;

    std::cout << "struct inheritance defaults to PUBLIC" << std::endl;
    std::cout << "class inheritance defaults to PRIVATE" << std::endl;

    std::cout << "\nExample:" << std::endl;
    std::cout << "  struct S : Base {};     // public inheritance" << std::endl;
    std::cout << "  class C : public Base {}; // must write 'public'" << std::endl;
}

// When to use struct vs class
void WhenToUse() {
    std::cout << "\n=== When to Use struct vs class ===" << std::endl;

    std::cout << "Use STRUCT for:" << std::endl;
    std::cout << "  - Passive data objects (no invariants)" << std::endl;
    std::cout << "  - Plain data carriers" << std::endl;
    std::cout << "  - Interfacing with C" << std::endl;
    std::cout << "  - Aggregate initialization" << std::endl;
    std::cout << "  - POD-like types" << std::endl;

    std::cout << "\nUse CLASS for:" << std::endl;
    std::cout << "  - Types with invariants" << std::endl;
    std::cout << "  - Types that manage resources" << std::endl;
    std::cout << "  - Types with complex behavior" << std::endl;
    std::cout << "  - Types with private state" << std::endl;
    std::cout << "  - Types with virtual functions" << std::endl;

    std::cout << "\nRule of thumb:" << std::endl;
    std::cout << "  If it's 'just data' -> struct" << std::endl;
    std::cout << "  If it 'does things' or has invariants -> class" << std::endl;
}

// Example: appropriate usage
struct Point3D {  // Appropriate: just data
    double x, y, z;
};

class Vector3D {  // Appropriate: has operations and invariant (normalized)
private:
    double x_, y_, z_;
    bool normalized_ = false;

public:
    Vector3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    double magnitude() const {
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    void normalize() {
        double mag = magnitude();
        if(mag > 0) {
            x_ /= mag; y_ /= mag; z_ /= mag;
            normalized_ = true;
        }
    }

    bool is_normalized() const { return normalized_; }
};

void AppropriateUsage() {
    std::cout << "\n=== Appropriate Usage Examples ===" << std::endl;

    // Struct: just holds data
    Point3D p{1.0, 2.0, 3.0};
    std::cout << "Point3D: (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

    // Class: has behavior and invariants
    Vector3D v(3.0, 4.0, 0.0);
    std::cout << "\nVector3D before normalize: magnitude = " << v.magnitude() << std::endl;
    std::cout << "Is normalized? " << v.is_normalized() << std::endl;

    v.normalize();
    std::cout << "After normalize: magnitude = " << v.magnitude() << std::endl;
    std::cout << "Is normalized? " << v.is_normalized() << std::endl;

    // Can't access private members directly
    // std::cout << v.x_;  // Error: x_ is private
}

// Technically identical
struct TechnicallyStruct {
    int x;
    void foo() { std::cout << "foo: " << x << std::endl; }
    ~TechnicallyStruct() { std::cout << "~TechnicallyStruct" << std::endl; }
};

class TechnicallyClass {
public:
    int x;
    void foo() { std::cout << "foo: " << x << std::endl; }
    ~TechnicallyClass() { std::cout << "~TechnicallyClass" << std::endl; }
};

void TechnicalEquivalence() {
    std::cout << "\n=== Technical Equivalence ===" << std::endl;

    std::cout << "With explicit access specifiers, struct and class are identical:" << std::endl;
    std::cout << "  Same memory layout" << std::endl;
    std::cout << "  Same behavior" << std::endl;
    std::cout << "  Same performance" << std::endl;

    TechnicallyStruct s{42};
    TechnicallyClass c{24};

    s.foo();
    c.foo();

    std::cout << "\nsizeof(TechnicallyStruct) = " << sizeof(TechnicallyStruct) << std::endl;
    std::cout << "sizeof(TechnicallyClass) = " << sizeof(TechnicallyClass) << std::endl;
}