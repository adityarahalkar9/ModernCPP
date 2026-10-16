import std;
#include "templates_header.h"

// Pitfall 1: Not using typename
template <typename T>
void Pitfall1() {
    // ERROR: T::iterator is treated as value
    // typename T::iterator it = ...;  // Need typename
    std::cout << "  Must use 'typename' for dependent types" << std::endl;
}

void TypenamePitfall() {
    std::cout << "=== Pitfall: Missing typename ===" << std::endl;

    std::cout << "Wrong:" << std::endl;
    std::cout << "  T::iterator it;" << std::endl;
    std::cout << "  (compiler thinks it's a value)" << std::endl;
    std::cout << std::endl;
    std::cout << "Right:" << std::endl;
    std::cout << "  typename T::iterator it;" << std::endl;
}

// Pitfall 2: Not using template
struct Outer2 {
    template <typename T>
    struct Inner {};
};

template <typename T>
void Pitfall2() {
    // ERROR: Outer::Inner<T> parsed as (Outer::Inner) < T
    // typename Outer::template Inner<T> inner;  // Need template
    std::cout << "  Must use 'template' for dependent templates" << std::endl;
}

void TemplatePitfall() {
    std::cout << "\n=== Pitfall: Missing template ===" << std::endl;

    std::cout << "Wrong:" << std::endl;
    std::cout << "  Outer::Inner<T>" << std::endl;
    std::cout << "  (parsed as less-than!)" << std::endl;
    std::cout << std::endl;
    std::cout << "Right:" << std::endl;
    std::cout << "  Outer::template Inner<T>" << std::endl;
}

// Pitfall 3: Dependent base class
template <typename T>
class DependentBase {
protected:
    void method() {}
    int value = 42;
};

template <typename T>
class DerivedFromBase : public DependentBase<T> {
public:
    void call() {
        // ERROR: 'method' not found in dependent base
        // this->method();  // Need this->
        // std::cout << value;  // Need this->value
    }
};

void DependentBasePitfall() {
    std::cout << "\n=== Pitfall: Dependent Base Class ===" << std::endl;

    std::cout << "In dependent base, need 'this->':" << std::endl;
    std::cout << "  this->method();" << std::endl;
    std::cout << "  this->value;" << std::endl;
    std::cout << std::endl;
    std::cout << "Why: compiler doesn't look in dependent bases" << std::endl;
    std::cout << "  without explicit qualification" << std::endl;
}

// Pitfall 4: Template code bloat
template <typename T>
class BloatedClass {
    // Lots of code generated for each T
public:
    void heavy_method1() { /* ... */ }
    void heavy_method2() { /* ... */ }
    void heavy_method3() { /* ... */ }
};

void CodeBloatPitfall() {
    std::cout << "\n=== Pitfall: Code Bloat ===" << std::endl;

    std::cout << "Each instantiation duplicates code:" << std::endl;
    std::cout << "  BloatedClass<int>" << std::endl;
    std::cout << "  BloatedClass<double>" << std::endl;
    std::cout << "  BloatedClass<std::string>" << std::endl;
    std::cout << std::endl;
    std::cout << "Solutions:" << std::endl;
    std::cout << "  1. Common non-template base" << std::endl;
    std::cout << "  2. Hoist invariant code out" << std::endl;
}

// Pitfall 5: Specialization in wrong namespace
template <typename T>
struct SpecializeMe {};

void SpecializationPitfall() {
    std::cout << "\n=== Pitfall: Specialization Rules ===" << std::endl;

    std::cout << "Specialization must be in:" << std::endl;
    std::cout << "  Same namespace as primary template" << std::endl;
    std::cout << "  Or the template's enclosing namespace" << std::endl;
}

// Pitfall 6: Unexpected conversions
template <typename T>
void Process(T value) {
    std::cout << "  Processing: " << value << std::endl;
}

void ConversionPitfall() {
    std::cout << "\n=== Pitfall: Unexpected Behavior ===" << std::endl;

    std::cout << "Templates can surprise you:" << std::endl;

    // Array decay
    int arr[] = {1, 2, 3};
    Process(arr);   // T = int* (not int[3]!)

    // String literals
    Process("hello");  // T = const char* (not std::string)
}