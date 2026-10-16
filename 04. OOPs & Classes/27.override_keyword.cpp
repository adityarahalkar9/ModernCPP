import std;
#include "class_header.h"

// Without override (dangerous!)
class Base {
public:
    virtual void process(int x) {
        std::cout << "  Base::process(int)" << std::endl;
    }

    virtual void handle(const std::string& s) const {
        std::cout << "  Base::handle(const string&)" << std::endl;
    }

    virtual ~Base() = default;
};

class WithoutOverride : public Base {
public:
    // INTENDED to override, but signature differs slightly
    void process(long x) {  // Different parameter type!
        std::cout << "  WithoutOverride::process(long) - NOT an override!" << std::endl;
    }

    void handle(std::string s) {  // Different: by value, not const ref
        std::cout << "  WithoutOverride::handle(string) - NOT an override!" << std::endl;
    }
};

void WithoutOverrideDemo() {
    std::cout << "=== Without override (DANGEROUS) ===" << std::endl;

    std::cout << "Problem: New virtual function created accidentally" << std::endl;

    WithoutOverride w;

    // Static type determines which is called
    w.process(42L);     // Calls derived version (hides base)
    w.handle("test");   // Calls derived version (hides base)

    // Through base pointer: calls BASE versions
    Base* b = &w;
    b->process(42);     // Calls Base::process (int)
    b->handle("test");  // Calls Base::handle (const string&)

    std::cout << "\nPolymorphism is broken!" << std::endl;
    std::cout << "Derived created NEW virtual functions," << std::endl;
    std::cout << "not overriding existing ones." << std::endl;
}

// With override (safe!)
class WithOverride : public Base {
public:
    void process(int x) override {  // Must match exactly
        std::cout << "  WithOverride::process(int) - proper override" << std::endl;
    }

    void handle(const std::string& s) const override {
        std::cout << "  WithOverride::handle(const string&) const" << std::endl;
    }
};

void WithOverrideDemo() {
    std::cout << "\n=== With override (SAFE) ===" << std::endl;

    std::cout << "'override' keyword:" << std::endl;
    std::cout << "  Compiler verifies it's actually overriding" << std::endl;
    std::cout << "  Catches signature mismatches" << std::endl;
    std::cout << "  Compile error if not truly virtual override" << std::endl;

    WithOverride w;
    w.process(42);
    w.handle("test");

    // Polymorphism works correctly
    Base* b = &w;
    b->process(42);     // Calls WithOverride::process
    b->handle("test");  // Calls WithOverride::handle
}

// Common mistakes caught by override
class MistakeBase {
public:
    virtual void method(int x) const = 0;
    virtual void another(int x) = 0;
    virtual ~MistakeBase() = default;
};

// These would all be caught with 'override':
class WouldBeCaught : public MistakeBase {
public:
    // void method(int x) override;        // ERROR: missing const
    // void method(long x) const override; // ERROR: wrong parameter type
    // int method(int x) const override;   // ERROR: wrong return type (unless covariant)
    // void another(int x, int y) override; // ERROR: wrong number of parameters

    // Correct:
    void method(int x) const override {
        std::cout << "  Correct override" << std::endl;
    }

    void another(int x) override {
        std::cout << "  Another correct override" << std::endl;
    }
};

void CommonMistakes() {
    std::cout << "\n=== Common Mistakes Caught ===" << std::endl;

    std::cout << "'override' catches:" << std::endl;
    std::cout << "  1. Missing const" << std::endl;
    std::cout << "  2. Wrong parameter types" << std::endl;
    std::cout << "  3. Wrong number of parameters" << std::endl;
    std::cout << "  4. Wrong return type" << std::endl;
    std::cout << "  5. Typos in function names" << std::endl;

    WouldBeCaught wbc;
    wbc.method(42);
    wbc.another(10);
}

// override vs virtual
class StyleBase {
public:
    virtual void style() = 0;
    virtual ~StyleBase() = default;
};

class StyleDerived : public StyleBase {
public:
    // Both work, but override is preferred
    void style() override {  // Preferred
        std::cout << "  With override (preferred)" << std::endl;
    }
};

class OldStyle : public StyleBase {
public:
    virtual void style() {  // Also works, but less safe
        std::cout << "  With virtual (works but risky)" << std::endl;
    }
};

void OverrideVsVirtual() {
    std::cout << "\n=== override vs virtual ===" std::endl;

    std::cout << "In derived classes:" << std::endl;
    std::cout << "  'virtual' is optional" << std::endl;
    std::cout << "  'override' is safer" << std::endl;

    std::cout << "\nRecommended:" << std::endl;
    std::cout << "  Base: virtual void func();" << std::endl;
    std::cout << "  Derived: void func() override;" << std::endl;

    std::cout << "\nNever:" << std::endl;
    std::cout << "  Derived: virtual void func() override;" << std::endl;
    std::cout << "  (redundant, confusing)" << std::endl;
}