import std;
#include "templates_header.h"

// Basic template argument deduction
template <typename T>
void Deduce(T value) {
    std::cout << "  T = " << typeid(T).name() << std::endl;
}

template <typename T>
void DeduceRef(T& value) {
    std::cout << "  T& - lvalue reference" << std::endl;
}

template <typename T>
void DeduceRRef(T&& value) {
    std::cout << "  T&& - forwarding reference" << std::endl;
}

void BasicDeduction() {
    std::cout << "=== Template Argument Deduction ===" << std::endl;

    int x = 42;
    const int cx = 10;

    std::cout << "Deduce(x):" << std::endl;
    Deduce(x);   // T = int

    std::cout << "Deduce(cx):" << std::endl;
    Deduce(cx);  // T = int (const dropped for by-value)

    std::cout << "Deduce(3.14):" << std::endl;
    Deduce(3.14);  // T = double

    std::cout << "\nDeduceRef(x):" << std::endl;
    DeduceRef(x);   // T = int, param is int&

    std::cout << "DeduceRRef(x):" << std::endl;
    DeduceRRef(x);   // T = int& (lvalue → lvalue ref)

    std::cout << "DeduceRRef(42):" << std::endl;
    DeduceRRef(42);  // T = int (rvalue → rvalue ref)
}

// Deduction with containers
template <typename T>
void ProcessContainer(const std::vector<T>& vec) {
    std::cout << "  Vector of " << typeid(T).name()
        << ", size: " << vec.size() << std::endl;
}

void ContainerDeduction() {
    std::cout << "\n=== Container Deduction ===" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    ProcessContainer(vec);  // T deduced as int
}

// Deduction failure cases
template <typename T>
void Compare(T a, T b) {
    std::cout << "  Comparing same types" << std::endl;
}

void DeductionFailures() {
    std::cout << "\n=== Deduction Failures ===" << std::endl;

    std::cout << "When deduction fails:" << std::endl;
    std::cout << "  1. Conflicting deductions" << std::endl;
    std::cout << "  2. No viable function" << std::endl;
    std::cout << std::endl;

    std::cout << "Compare(1, 2) - OK:" << std::endl;
    Compare(1, 2);  // T=int

    std::cout << "Compare(1, 2.5) - ERROR:" << std::endl;
    std::cout << "  // T can't be both int and double" << std::endl;
    std::cout << "  // Must specify explicitly:" << std::endl;
    std::cout << "  Compare<double>(1, 2.5);" << std::endl;
}

// auto as template parameter (C++17)
template <auto Value>
void AutoParameter() {
    std::cout << "  Value = " << Value << std::endl;
}

void AutoDeduction() {
    std::cout << "\n=== auto Template Parameters (C++17) ===" << std::endl;

    std::cout << "C++17 allows auto for non-type parameters:" << std::endl;
    std::cout << "  template <auto Value>" << std::endl;
    std::cout << "  Deduces type from the value" << std::endl;

    AutoParameter<42>();     // int
    AutoParameter<3.14>();   // double
    AutoParameter<'c'>();    // char
}

// Deduction with initialization lists
template <typename T>
void ProcessInitList(std::initializer_list<T> list) {
    std::cout << "  List of " << typeid(T).name()
        << " with " << list.size() << " elements" << std::endl;
}

void InitListDeduction() {
    std::cout << "\n=== Initializer List Deduction ===" << std::endl;

    ProcessInitList({1, 2, 3});        // T=int
    ProcessInitList({1.5, 2.5});       // T=double
    ProcessInitList({"a", "b"});       // T=const char*
}

// Deduction rules summary
void DeductionRules() {
    std::cout << "\n=== Deduction Rules Summary ===" << std::endl;

    std::cout << "For by-value (T param):" << std::endl;
    std::cout << "  - Drops const, volatile" << std::endl;
    std::cout << "  - Drops references" << std::endl;
    std::cout << "  - Arrays decay to pointers" << std::endl;
    std::cout << "  - Functions decay to function pointers" << std::endl;

    std::cout << "For reference (T& param):" << std::endl;
    std::cout << "  - Preserves const" << std::endl;
    std::cout << "  - Binds to lvalues" << std::endl;

    std::cout << "For forwarding (T&& param):" << std::endl;
    std::cout << "  - Universal reference" << std::endl;
    std::cout << "  - Lvalue → T&" << std::endl;
    std::cout << "  - Rvalue → T&&" << std::endl;
}