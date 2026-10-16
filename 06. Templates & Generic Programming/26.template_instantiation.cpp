import std;
#include "templates_header.h"

// Implicit instantiation
template <typename T>
class Instantiated {
public:
    explicit Instantiated(T value) : value_(value) {
        std::cout << "  Instantiated<" << typeid(T).name()
            << "> created with " << value_ << std::endl;
    }

    T get() const { return value_; }

private:
    T value_;
};

void ImplicitInstantiation() {
    std::cout << "=== Implicit Instantiation ===" << std::endl;

    std::cout << "Compiler generates code when you USE the template:" << std::endl;
    std::cout << "  Instantiated<int> a(42);" << std::endl;
    std::cout << "  → Compiler generates Instantiated<int> class" << std::endl;

    Instantiated<int> int_obj(42);
    Instantiated<double> dbl_obj(3.14);
    Instantiated<std::string> str_obj("hello");

    std::cout << "\nEach type creates a SEPARATE class:" << std::endl;
    std::cout << "  &int_obj type: " << typeid(int_obj).name() << std::endl;
    std::cout << "  &dbl_obj type: " << typeid(dbl_obj).name() << std::endl;
}

// Function template instantiation
template <typename T>
T Identity(T value) {
    return value;
}

// Explicit instantiation
template int Identity<int>(int);
template double Identity<double>(double);

void ExplicitInstantiationDemo() {
    std::cout << "\n=== Explicit Instantiation ===" << std::endl;

    std::cout << "Force instantiation even without use:" << std::endl;
    std::cout << "  template int Identity<int>(int);" << std::endl;
    std::cout << "  (in a .cpp file)" << std::endl;

    std::cout << "\nUse cases:" << std::endl;
    std::cout << "  - Library provides pre-instantiated versions" << std::endl;
    std::cout << "  - Reduces compile times for users" << std::endl;
}

// Instantiation points
template <typename T>
class Deferred {
public:
    void method() {
        std::cout << "  Deferred::method" << std::endl;
    }

    // This method is only instantiated if called
    void unused_method() {
        // Even if this has errors for some T, it's OK
        // if never called for that T
        std::cout << "  Deferred::unused_method" << std::endl;
    }
};

void SelectiveInstantiation() {
    std::cout << "\n=== Selective Instantiation ===" << std::endl;

    std::cout << "Methods are instantiated only when used:" << std::endl;
    std::cout << "  Errors in unused methods are ignored" << std::endl;

    Deferred<int> d;
    d.method();
    // d.unused_method();  // Only instantiated if called
}

// Two-phase lookup
template <typename T>
class TwoPhase {
    // Phase 1: Non-dependent names looked up at definition
    int member = 42;

public:
    void method() {
        // Phase 2: Dependent names looked up at instantiation
        std::cout << "  member = " << member << std::endl;
    }
};

void TwoPhaseLookup() {
    std::cout << "\n=== Two-Phase Lookup ===" << std::endl;

    std::cout << "Phase 1 (definition):" << std::endl;
    std::cout << "  Non-dependent names checked" << std::endl;
    std::cout << std::endl;
    std::cout << "Phase 2 (instantiation):" << std::endl;
    std::cout << "  Dependent names checked" << std::endl;
    std::cout << "  T-specific code generated" << std::endl;

    TwoPhase<int> tp;
    tp.method();
}

// Code bloat considerations
template <typename T>
void HeavyFunction(T value) {
    // Large amount of code
    std::cout << "  Heavy processing of: " << value << std::endl;
}

void CodeBloatConsiderations() {
    std::cout << "\n=== Code Bloat ===" << std::endl;

    std::cout << "Each instantiation creates separate code:" << std::endl;
    std::cout << "  HeavyFunction<int>" << std::endl;
    std::cout << "  HeavyFunction<double>" << std::endl;
    std::cout << "  HeavyFunction<std::string>" << std::endl;
    std::cout << std::endl;
    std::cout << "Mitigation strategies:" << std::endl;
    std::cout << "  1. Common non-template base" << std::endl;
    std::cout << "  2. Thin template wrapper over non-template code" << std::endl;
    std::cout << "  3. Explicit instantiation in one place" << std::endl;
}