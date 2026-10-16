import std;
#include "class_header.h"

// Basic CRTP
template <typename Derived>
class Base {
public:
    void interface() {
        // Cast 'this' to Derived* and call implementation
        static_cast<Derived*>(this)->implementation();
    }

    void common_method() {
        std::cout << "  Base::common (shared by all)" << std::endl;
        // Can also call derived-specific code
        derived_specific();
    }

    // Default implementation (can be overridden)
    virtual void derived_specific() {
        std::cout << "  Base default specific" << std::endl;
    }

protected:
    // Make Base impossible to instantiate directly
    Base() = default;

    friend Derived;  // Allow Derived to access protected members
};

class Impl1 : public Base<Impl1> {
public:
    void implementation() {
        std::cout << "  Impl1::implementation" << std::endl;
    }

    void derived_specific() override {
        std::cout << "  Impl1 specific" << std::endl;
    }
};

class Impl2 : public Base<Impl2> {
public:
    void implementation() {
        std::cout << "  Impl2::implementation" << std::endl;
    }
};

void CRTPBasics() {
    std::cout << "=== CRTP Basics ===" << std::endl;

    std::cout << "CRTP = Curiously Recurring Template Pattern" << std::endl;
    std::cout << "  class Derived : public Base<Derived>" << std::endl;

    std::cout << "\nStatic polymorphism:" << std::endl;
    Impl1 i1;
    Impl2 i2;

    i1.interface();  // Calls Impl1::implementation
    i2.interface();  // Calls Impl2::implementation

    std::cout << "\nNo virtual functions needed!" << std::endl;
    std::cout << "Resolved at compile time" << std::endl;
}

// CRTP for static polymorphism
template <typename Derived>
class ShapeBase {
public:
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();
    }

    void describe() const {
        std::cout << "  Area: " << area() << std::endl;
    }
};

class Circle3 : public ShapeBase<Circle3> {
    double radius;

public:
    Circle3(double r) : radius(r) {}

    double area_impl() const {
        return 3.14159265358979 * radius * radius;
    }
};

class Square3 : public ShapeBase<Square3> {
    double side;

public:
    Square3(double s) : side(s) {}

    double area_impl() const {
        return side * side;
    }
};

void StaticPolymorphism() {
    std::cout << "\n=== Static Polymorphism ===" << std::endl;

    std::cout << "Compare to virtual functions:" << std::endl;
    std::cout << "  Virtual: runtime dispatch via vtable" << std::endl;
    std::cout << "  CRTP: compile-time dispatch" << std::endl;

    Circle3 c(5.0);
    Square3 s(4.0);

    c.describe();  // Calls Circle3::area_impl
    s.describe();  // Calls Square3::area_impl

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  No vtable overhead" << std::endl;
    std::cout << "  Can be inlined" << std::endl;
    std::cout << "  Faster than virtual" << std::endl;
}

// CRTP for mixins
template <typename Derived>
class Printable3 {
public:
    void print() const {
        std::cout << "  " << static_cast<const Derived*>(this)->to_string() << std::endl;
    }
};

template <typename Derived>
class Comparable3 {
public:
    bool operator>(const Derived& other) const {
        return static_cast<const Derived*>(this)->get_value() > other.get_value();
    }

    bool operator<(const Derived& other) const {
        return static_cast<const Derived*>(this)->get_value() < other.get_value();
    }
};

class Value : public Printable3<Value>, public Comparable3<Value> {
    int v;

public:
    Value(int val) : v(val) {}

    int get_value() const { return v; }
    std::string to_string() const { return "Value(" + std::to_string(v) + ")"; }
};

void CRTPMixins() {
    std::cout << "\n=== CRTP Mixins ===" << std::endl;

    std::cout << "CRTP enables mixin composition:" << std::endl;
    std::cout << "  Add capabilities via inheritance" << std::endl;

    Value v1(42);
    Value v2(100);

    v1.print();  // From Printable3
    v2.print();

    std::cout << "  v1 > v2: " << (v1 > v2) << std::endl;  // From Comparable3
    std::cout << "  v1 < v2: " << (v1 < v2) << std::endl;
}

// CRTP to avoid virtual calls in hot path
template <typename Derived>
class OptimizedBase {
public:
    // This would normally be virtual
    int process(int input) {
        return static_cast<Derived*>(this)->process_impl(input);
    }

    // Common code that uses process()
    int process_many(const std::vector<int>& inputs) {
        int result = 0;
        for(int i : inputs) {
            result += process(i);
        }
        return result;
    }
};

class FastImpl : public OptimizedBase<FastImpl> {
public:
    int process_impl(int x) {
        return x * 2;
    }
};

void CRTPPerformance() {
    std::cout << "\n=== CRTP Performance ===" << std::endl;

    std::cout << "CRTP avoids:" << std::endl;
    std::cout << "  Virtual dispatch overhead" << std::endl;
    std::cout << "  vtable indirection" << std::endl;
    std::cout << "  Prevents inlining" << std::endl;

    FastImpl fast;
    std::vector<int> data = {1, 2, 3, 4, 5};

    int result = fast.process_many(data);
    std::cout << "  Result: " << result << std::endl;
}

// CRTP with object counters
template <typename Derived>
class Counted {
    static inline int count = 0;

public:
    Counted() { ++count; }
    ~Counted() { --count; }

    static int get_count() { return count; }
};

class Widget2 : public Counted<Widget2> {};
class Gadget : public Counted<Gadget> {};

void CRTPObjectCounting() {
    std::cout << "\n=== CRTP Object Counting ===" << std::endl;

    std::cout << "Each type gets its own counter:" << std::endl;

    {
        Widget2 w1, w2;
        Gadget g1;

        std::cout << "  Widgets: " << Widget2::get_count() << std::endl;
        std::cout << "  Gadgets: " << Gadget::get_count() << std::endl;
    }

    std::cout << "After scope:" << std::endl;
    std::cout << "  Widgets: " << Widget2::get_count() << std::endl;
    std::cout << "  Gadgets: " << Gadget::get_count() << std::endl;
}