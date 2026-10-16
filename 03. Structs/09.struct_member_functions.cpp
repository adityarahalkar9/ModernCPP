#include <iostream>
#include <string>
#include <cmath>
#include "structs_header.h"

// Struct with member functions
struct Vector2 {
    // Data members
    double x, y;

    // Member functions (methods)
    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    double dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    void scale(double factor) {
        x *= factor;
        y *= factor;
    }

    Vector2 operator+(const Vector2& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Vector2 operator-(const Vector2& rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Vector2 operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    bool operator==(const Vector2& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

void BasicMemberFunctions() {
    std::cout << "=== Struct Member Functions ===" << std::endl;

    Vector2 v1{3.0, 4.0};
    Vector2 v2{1.0, 2.0};

    std::cout << "v1 = (" << v1.x << ", " << v1.y << ")" << std::endl;
    std::cout << "v2 = (" << v2.x << ", " << v2.y << ")" << std::endl;

    std::cout << "\nv1.magnitude() = " << v1.magnitude() << std::endl;
    std::cout << "v1.dot(v2) = " << v1.dot(v2) << std::endl;

    v1.scale(2.0);
    std::cout << "\nAfter scaling v1 by 2: (" << v1.x << ", " << v1.y << ")" << std::endl;
}

void OperatorOverloading() {
    std::cout << "\n=== Operator Overloading ===" << std::endl;

    Vector2 a{1.0, 2.0};
    Vector2 b{3.0, 4.0};

    Vector2 sum = a + b;
    Vector2 diff = a - b;
    Vector2 scaled = a * 10.0;

    std::cout << "a + b = (" << sum.x << ", " << sum.y << ")" << std::endl;
    std::cout << "a - b = (" << diff.x << ", " << diff.y << ")" << std::endl;
    std::cout << "a * 10 = (" << scaled.x << ", " << scaled.y << ")" << std::endl;

    std::cout << "\na == b: " << (a == b) << std::endl;
    std::cout << "a == a: " << (a == a) << std::endl;
}

// const member functions
struct Counter {
    int count = 0;

    void increment() {  // Non-const: modifies state
        ++count;
    }

    int get() const {  // Const: doesn't modify state
        return count;
    }

    // ERROR: const function can't modify members
    // void bad_increment() const { ++count; }
};

void ConstMemberFunctions() {
    std::cout << "\n=== const Member Functions ===" << std::endl;

    Counter c;
    c.increment();
    c.increment();

    std::cout << "count = " << c.get() << std::endl;

    // Can call const functions on const objects
    const Counter const_c{42};
    std::cout << "const count = " << const_c.get() << std::endl;

    // Cannot call non-const functions on const objects
    // const_c.increment();  // ERROR!

    std::cout << "\nRules:" << std::endl;
    std::cout << "  - const functions can be called on any object" << std::endl;
    std::cout << "  - non-const functions can only be called on non-const objects" << std::endl;
    std::cout << "  - Mark functions const if they don't modify state" << std::endl;
}

// Static member functions
struct MathUtils {
    static double square(double x) {
        return x * x;
    }

    static double cube(double x) {
        return x * x * x;
    }

    static constexpr double pi = 3.14159265358979;
};

void StaticMemberFunctions() {
    std::cout << "\n=== Static Member Functions ===" << std::endl;

    // Call without object instance
    std::cout << "MathUtils::square(5) = " << MathUtils::square(5) << std::endl;
    std::cout << "MathUtils::cube(3) = " << MathUtils::cube(3) << std::endl;
    std::cout << "MathUtils::pi = " << MathUtils::pi << std::endl;

    // Can also call through instance (but not recommended)
    MathUtils mu;
    std::cout << "mu.square(4) = " << mu.square(4) << std::endl;
}

// Member functions with this pointer
struct ThisDemo {
    int value = 0;

    ThisDemo& add(int x) {
        value += x;
        return *this;  // Return reference for chaining
    }

    ThisDemo& multiply(int x) {
        value *= x;
        return *this;
    }

    void print_address() const {
        std::cout << "this = " << this << std::endl;
    }
};

void ThisPointer() {
    std::cout << "\n=== this Pointer ===" << std::endl;

    ThisDemo obj;
    std::cout << "Object address: " << &obj << std::endl;
    obj.print_address();

    // Method chaining via this
    obj.add(10).multiply(2).add(5);
    std::cout << "After chaining: value = " << obj.value << std::endl;

    std::cout << "\nthis pointer:" << std::endl;
    std::cout << "  - Implicit parameter to all non-static member functions" << std::endl;
    std::cout << "  - Points to the object the function was called on" << std::endl;
    std::cout << "  - Enables method chaining" << std::endl;
}