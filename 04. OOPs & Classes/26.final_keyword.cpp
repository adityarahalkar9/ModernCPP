import std;
#include "class_header.h"

// final on virtual functions
class Base {
public:
    virtual void method() {
        std::cout << "  Base::method" << std::endl;
    }

    virtual void another_method() {
        std::cout << "  Base::another_method" << std::endl;
    }

    virtual ~Base() = default;
};

class Derived : public Base {
public:
    // This method cannot be overridden further
    void method() final {
        std::cout << "  Derived::method (FINAL)" << std::endl;
    }

    // This one can still be overridden
    void another_method() override {
        std::cout << "  Derived::another_method" << std::endl;
    }
};

// Attempting to override final: COMPILE ERROR
// class Further : public Derived {
// public:
//     void method() override {  // ERROR: method is final!
//         std::cout << "Further::method" << std::endl;
//     }
// };

void FinalOnVirtualFunction() {
    std::cout << "=== final on Virtual Functions ===" << std::endl;

    Derived d;
    d.method();          // Uses final version
    d.another_method();  // Can still be overridden further

    std::cout << "\n'final' on virtual function:" << std::endl;
    std::cout << "  Prevents further overriding" << std::endl;
    std::cout << "  Enables devirtualization optimization" << std::endl;
}

// final on classes
class Sealed final {
public:
    void method() {
        std::cout << "  Sealed::method" << std::endl;
    }
};

// Attempting to inherit: COMPILE ERROR
// class DerivedFromSealed : public Sealed {  // ERROR!
// };

void FinalOnClass() {
    std::cout << "\n=== final on Classes ===" << std::endl;

    std::cout << "'final' on class:" << std::endl;
    std::cout << "  Prevents any inheritance" << std::endl;
    std::cout << "  Class is sealed" << std::endl;

    Sealed s;
    s.method();

    std::cout << "\nUse cases:" << std::endl;
    std::cout << "  Value types not meant for inheritance" << std::endl;
    std::cout << "  Implementation classes" << std::endl;
    std::cout << "  Security-sensitive classes" << std::endl;
}

// final combined with override
class AnotherBase {
public:
    virtual void compute() = 0;
    virtual void optimize() = 0;
    virtual ~AnotherBase() = default;
};

class Implementation : public AnotherBase {
public:
    // Best practice: use both final and override
    void compute() final override {
        std::cout << "  Implementation::compute (final)" << std::endl;
    }

    void optimize() override {
        std::cout << "  Implementation::optimize (overridable)" << std::endl;
    }
};

void FinalWithOverride() {
    std::cout << "\n=== final + override ===" << std::endl;

    std::cout << "Best practice:" << std::endl;
    std::cout << "  Always use 'override'" << std::endl;
    std::cout << "  Add 'final' when appropriate" << std::endl;

    std::cout << "\n'override' catches errors:" << std::endl;
    std::cout << "  Typos in function name" << std::endl;
    std::cout << "  Signature mismatches" << std::endl;
    std::cout << "  const-qualification differences" << std::endl;

    Implementation impl;
    impl.compute();
    impl.optimize();
}

// Performance implications
class HotPath {
public:
    virtual int calculate(int x) {
        return x * 2;
    }
    virtual ~HotPath() = default;
};

class OptimizedHotPath : public HotPath {
public:
    int calculate(int x) final {  // final enables inlining
        return x * 2 + 1;
    }
};

void FinalPerformance() {
    std::cout << "\n=== final and Performance ===" std::endl;

    std::cout << "'final' enables optimizations:" << std::endl;
    std::cout << "  Compiler knows no further override" << std::endl;
    std::cout << "  Can devirtualize calls" << std::endl;
    std::cout << "  Can inline the function" << std::endl;

    std::cout << "\nWithout final:" << std::endl;
    std::cout << "  Must go through vtable" << std::endl;
    std::cout << "  Indirect call overhead" << std::endl;
    std::cout << "  Prevents inlining" << std::endl;
}

// When to use final
void WhenToUseFinal() {
    std::cout << "\n=== When to Use final ===" << std::endl;

    std::cout << "Use 'final' on functions when:" << std::endl;
    std::cout << "  Implementation is complete" << std::endl;
    std::cout << "  You want to prevent further override" << std::endl;
    std::cout << "  Performance-critical virtual calls" << std::endl;

    std::cout << "\nUse 'final' on classes when:" << std::endl;
    std::cout << "  Class not designed as base" << std::endl;
    std::cout << "  Value semantics types" << std::endl;
    std::cout << "  Utility classes" << std::endl;

    std::cout << "\nAvoid 'final' when:" << std::endl;
    std::cout << "  Framework expects extension" << std::endl;
    std::cout << "  Testing needs mocking" << std::endl;
    std::cout << "  Design might evolve" << std::endl;
}