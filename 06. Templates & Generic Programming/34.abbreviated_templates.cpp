import std;
#include "templates_header.h"

// C++20 abbreviated function templates
// auto in function parameters makes it a template

// Old style
template <typename T>
void OldStyle(T value) {
    std::cout << "  Old style: " << value << std::endl;
}

// C++20 style with auto
void NewStyle(auto value) {
    std::cout << "  New style: " << value << std::endl;
}

void BasicAbbreviated() {
    std::cout << "=== Abbreviated Function Templates (C++20) ===" << std::endl;

    std::cout << "'auto' parameters create templates:" << std::endl;
    std::cout << "  void func(auto value);" << std::endl;
    std::cout << "  is equivalent to:" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  void func(T value);" << std::endl;

    OldStyle(42);
    NewStyle(42);
    NewStyle(3.14);
    NewStyle("hello");
}

// Multiple auto parameters
void MultiAuto(auto first, auto second) {
    std::cout << "  " << first << " and " << second << std::endl;
}

void MultipleAutoParams() {
    std::cout << "\n=== Multiple auto Parameters ===" << std::endl;

    std::cout << "Each 'auto' is independent template parameter:" << std::endl;
    std::cout << "  void func(auto a, auto b);" << std::endl;
    std::cout << "  ≡ template <typename T, typename U>" << std::endl;
    std::cout << "     void func(T a, U b);" << std::endl;

    MultiAuto(42, "hello");
    MultiAuto(3.14, 100);
}

// Auto with concepts
void ProcessIntegral(std::integral auto value) {
    std::cout << "  Integral: " << value << std::endl;
}

void ProcessFloating(std::floating_point auto value) {
    std::cout << "  Floating: " << value << std::endl;
}

void AutoWithConcepts() {
    std::cout << "\n=== auto with Concepts ===" << std::endl;

    std::cout << "Constrained abbreviated templates:" << std::endl;
    std::cout << "  void func(std::integral auto x);" << std::endl;

    ProcessIntegral(42);
    ProcessFloating(3.14);
}

// Auto return type (C++14)
auto DeduceReturn(auto value) {
    return value * 2;
}

void AutoReturnType() {
    std::cout << "\n=== auto Return Types ===" << std::endl;

    auto int_result = DeduceReturn(21);
    auto dbl_result = DeduceReturn(2.5);

    std::cout << "  DeduceReturn(21) = " << int_result << std::endl;
    std::cout << "  DeduceReturn(2.5) = " << dbl_result << std::endl;
}

// Generic lambdas (C++14) - same idea
void GenericLambdas() {
    std::cout << "\n=== Generic Lambdas ===" << std::endl;

    std::cout << "Lambdas had this before C++20:" << std::endl;
    std::cout << "  auto lambda = [](auto x) { return x * 2; };" << std::endl;

    auto generic = [](auto x) { return x * 2; };

    std::cout << "  generic(21) = " << generic(21) << std::endl;
    std::cout << "  generic(2.5) = " << generic(2.5) << std::endl;
}