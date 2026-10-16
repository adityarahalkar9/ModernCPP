import std;
#include "class_header.h"

// Basic constructor inheritance
class Base {
public:
    Base(int x) {
        std::cout << "  Base(" << x << ")" << std::endl;
    }

    Base(std::string s) {
        std::cout << "  Base(\"" << s << "\")" << std::endl;
    }

    Base(int x, int y) {
        std::cout << "  Base(" << x << ", " << y << ")" << std::endl;
    }

    virtual ~Base() = default;
};

class Derived : public Base {
public:
    // Inherit all Base constructors
    using Base::Base;
};

void InheritingConstructorsBasics() {
    std::cout << "=== Inheriting Constructors ===" << std::endl;

    std::cout << "'using Base::Base' inherits constructors:" << std::endl;
    std::cout << "  No need to write them all in Derived" << std::endl;

    std::cout << "\nConstructing derived:" << std::endl;
    Derived d1(42);           // Calls Base(int)
    Derived d2("hello");      // Calls Base(string)
    Derived d3(10, 20);       // Calls Base(int, int)
}

// Without inheriting constructors
class WithoutInherit : public Base {
public:
    // Must manually forward each constructor
    WithoutInherit(int x) : Base(x) {
        std::cout << "  WithoutInherit forwarding int" << std::endl;
    }

    WithoutInherit(std::string s) : Base(std::move(s)) {
        std::cout << "  WithoutInherit forwarding string" << std::endl;
    }

    // Tedious if Base has many constructors!
};

void WithoutInheritance() {
    std::cout << "\n=== Without Constructor Inheritance ===" << std::endl;

    std::cout << "Without 'using':" << std::endl;
    std::cout << "  Must forward each constructor manually" << std::endl;
    std::cout << "  Error-prone" << std::endl;
    std::cout << "  Tedious for many constructors" << std::endl;

    WithoutInherit w1(42);
    WithoutInherit w2("test");
}

// Constructor inheritance with additional initialization
class Enhanced : public Base {
    int extra_data;

public:
    using Base::Base;  // Inherit constructors

    // But we can still add our own
    Enhanced(int x, int y, int extra) : Base(x, y), extra_data(extra) {
        std::cout << "  Enhanced with extra=" << extra_data << std::endl;
    }

    void set_extra(int e) { extra_data = e; }
};

void InheritanceWithExtra() {
    std::cout << "\n=== Inheritance with Additional Init ===" << std::endl;

    std::cout << "Can inherit AND add own constructors:" << std::endl;

    Enhanced e1(1, 2);          // Uses inherited
    Enhanced e2(1, 2, 100);     // Uses own constructor
}

// When constructors are NOT inherited
class Special : public Base {
public:
    // Defining ANY constructor stops inheritance
    Special() : Base(0) {
        std::cout << "  Special default (stops inheritance)" << std::endl;
    }

    // Must define all others manually now
    // Special(int) doesn't exist unless defined
};

void WhenNotInherited() {
    std::cout << "\n=== When Not Inherited ===" << std::endl;

    std::cout << "Constructors NOT inherited when:" << std::endl;
    std::cout << "  1. You define any constructor in Derived" << std::endl;
    std::cout << "  2. Default constructor" << std::endl;
    std::cout << "  3. Copy constructor" << std::endl;
    std::cout << "  4. Move constructor" << std::endl;

    std::cout << "\nInherited constructors are:" << std::endl;
    std::cout << "  All other user-defined constructors" << std::endl;
}

// Practical use case
class Exception2 : public std::runtime_error {
public:
    // Inherit runtime_error's constructors
    using std::runtime_error::runtime_error;
};

void PracticalUse() {
    std::cout << "\n=== Practical Use Case ===" << std::endl;

    std::cout << "Common pattern:" << std::endl;
    std::cout << "  Subclass exceptions without boilerplate" << std::endl;

    try {
        throw Exception2("Custom exception message");
    }
    catch(const Exception2& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
    }

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  Less code" << std::endl;
    std::cout << "  Fewer bugs" << std::endl;
    std::cout << "  Automatically stays in sync with Base" << std::endl;
}