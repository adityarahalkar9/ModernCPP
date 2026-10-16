import std;
#include "templates_header.h"

// Basic CRTP
template <typename Derived>
class Base5 {
public:
    void interface() {
        // Cast to derived and call implementation
        static_cast<Derived*>(this)->implementation();
    }

    // Common functionality
    void common() {
        std::cout << "  Common functionality" << std::endl;
    }

protected:
    Base5() = default;  // Prevent direct instantiation
    friend Derived;      // Allow derived to construct
};

class Impl1 : public Base5<Impl1> {
public:
    void implementation() {
        std::cout << "  Impl1::implementation" << std::endl;
    }
};

class Impl2 : public Base5<Impl2> {
public:
    void implementation() {
        std::cout << "  Impl2::implementation" << std::endl;
    }
};

void BasicCRTP() {
    std::cout << "=== CRTP (Curiously Recurring Template Pattern) ===" << std::endl;

    std::cout << "Pattern: class Derived : public Base<Derived>" << std::endl;

    Impl1 i1;
    Impl2 i2;

    i1.interface();  // Calls Impl1::implementation
    i2.interface();  // Calls Impl2::implementation

    std::cout << "\nStatic polymorphism (no virtual functions!)" << std::endl;
}

// CRTP for static polymorphism
template <typename Derived>
class Shape5 {
public:
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();
    }

    void describe() const {
        std::cout << "  Area: " << area() << std::endl;
    }
};

class Circle5 : public Shape5<Circle5> {
    double radius_;

public:
    explicit Circle5(double r) : radius_(r) {}

    double area_impl() const {
        return 3.14159265358979 * radius_ * radius_;
    }
};

class Square5 : public Shape5<Square5> {
    double side_;

public:
    explicit Square5(double s) : side_(s) {}

    double area_impl() const {
        return side_ * side_;
    }
};

void StaticPolymorphism() {
    std::cout << "\n=== Static Polymorphism ===" << std::endl;

    std::cout << "CRTP provides polymorphism without virtual functions:" << std::endl;

    Circle5 c(5.0);
    Square5 s(4.0);

    c.describe();
    s.describe();
}

// CRTP for mixins
template <typename Derived>
class Printable4 {
public:
    void print() const {
        std::cout << "  " << static_cast<const Derived*>(this)->to_string() << std::endl;
    }
};

template <typename Derived>
class Comparable4 {
public:
    bool operator<(const Derived& other) const {
        return static_cast<const Derived*>(this)->get_value()
            < other.get_value();
    }
};

class Value5 : public Printable4<Value5>, public Comparable4<Value5> {
    int value_;

public:
    explicit Value5(int v) : value_(v) {}

    int get_value() const { return value_; }
    std::string to_string() const { return "Value(" + std::to_string(value_) + ")"; }
};

void CRTPMixins() {
    std::cout << "\n=== CRTP Mixins ===" << std::endl;

    Value5 v1(42);
    Value5 v2(100);

    v1.print();  // From Printable4
    std::cout << "  v1 < v2: " << (v1 < v2) << std::endl;  // From Comparable4
}

// CRTP for performance (avoiding virtual calls)
template <typename Derived>
class OptimizedBase {
public:
    int process(int input) {
        return static_cast<Derived*>(this)->process_impl(input);
    }

    // Common function that uses the virtual-like call
    int process_many(const std::vector<int>& inputs) {
        int result = 0;
        for(int i : inputs) {
            result += process(i);  // Will be inlined
        }
        return result;
    }
};

class FastImpl : public OptimizedBase<FastImpl> {
public:
    int process_impl(int x) {
        return x * 2 + 1;
    }
};

void CRTPPerformance() {
    std::cout << "\n=== CRTP Performance ===" << std::endl;

    std::cout << "CRTP advantages:" << std::endl;
    std::cout << "  - No vtable indirection" << std::endl;
    std::cout << "  - Can be inlined" << std::endl;
    std::cout << "  - Better cache locality" << std::endl;

    FastImpl fast;
    std::vector<int> data = {1, 2, 3, 4, 5};

    int result = fast.process_many(data);
    std::cout << "  Result: " << result << std::endl;
}