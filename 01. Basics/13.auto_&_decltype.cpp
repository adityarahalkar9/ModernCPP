#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <type_traits>
#include "basic_header.h"

// Deep dive into auto type deduction rules
void AutoDeductionRules(){
    // Rule 1: auto drops reference and top-level const from value initialization
    int x = 10;
    const int& cref = x;
    auto a = cref;      // int (const and & dropped)
                        // a = 20; 

    // Rule 2: auto& preserves reference
    auto& b = cref;     // const int& (const preserved)
    // b = 30;          // Error: cannot modify const reference

    // Rule 3: auto* requires pointer initializer
    int* ptr = &x;
    auto* c = ptr;      // int*
    auto d = ptr;       // int* (same as auto*)
    std::cout << "Value through pointer: " << *c << std::endl;

    // Rule 4: auto&& is universal (forwarding) reference
    auto&& e = x;       // int& (lvalue -> lvalue reference)
    auto&& f = 42;      // int&& (rvalue -> rvalue reference)

    std::cout << "auto deduction rules demonstrated" << std::endl;
}


// auto with containers
void AutoWithContainers(){
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};

    // Without auto (verbose)
    for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // With auto (clean)
    for(auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Range-based for with auto
    for(const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // Auto with map
    for(const auto& [key, value] : m) {
        std::cout << key << " -> " << value << std::endl;
    }
}


// auto in lambda parameters (C++14)
void AutoInLambdas(){
    auto generic_lambda = [](auto x, auto y) {
        return x + y;
        };

    std::cout << "Generic lambda (int): " << generic_lambda(3, 4) << std::endl;
    std::cout << "Generic lambda (double): " << generic_lambda(2.5, 3.7) << std::endl;
    std::cout << "Generic lambda (string): "
        << generic_lambda(std::string("Hello"), std::string(" World")) << std::endl;
}


// trailing return type
template <typename T, typename U>
auto AddValues(T t, U u) -> decltype(t + u) {
    return t + u;
}
void TrailingReturnType() {
    auto result1 = AddValues(3, 4.5);       // double
    auto result2 = AddValues(2.5f, 3);      // float

    std::cout << "3 + 4.5 = " << result1 << std::endl;
    std::cout << "2.5f + 3 = " << result2 << std::endl;
    std::cout << "Type of result1: " << typeid(result1).name() << std::endl;
}


// decltype in type traits
void DecltypeInTypeTraits() {
    static_assert(std::is_same_v<decltype(42), int>);
    static_assert(std::is_same_v<decltype(3.14), double>);
    static_assert(std::is_same_v<decltype('c'), char>);

    int x = 10;
    static_assert(std::is_same_v<decltype(x), int>);
    static_assert(std::is_same_v<decltype(&x), int*>);

    std::cout << "All type trait static_asserts passed!" << std::endl;
}


// Common use case: decltype in template metaprogramming
template <typename Container>
auto GetFirstElement(const Container& c) -> decltype(*c.begin()) {
    return *c.begin();
}
void DecltypeInTemplates() {
    std::vector<int> vec = {10, 20, 30};
    std::string str = "Hello";

    auto first_vec = GetFirstElement(vec);   // int
    auto first_str = GetFirstElement(str);   // char

    std::cout << "First element of vector: " << first_vec << std::endl;
    std::cout << "First character of string: " << first_str << std::endl;
}


// decltype(auto) for perfect forwarding return types
template <typename T>
decltype(auto) ForwardReturn(T&& t) {
    return std::forward<T>(t);
}
void DecltypeAutoForwarding() {
    int x = 42;
    auto& ref = ForwardReturn(x);  // int&
    ref = 100;
    std::cout << "x = " << x << " (modified through forwarded reference)" << std::endl;

    auto val = ForwardReturn(42);  // int (rvalue -> moved/copied)
    std::cout << "val = " << val << std::endl;
}


// C++20: constrained auto with concepts
#include <concepts>
template <std::integral T>
void ProcessInteger(T value) {
    std::cout << "Processing integer: " << value << std::endl;
}
template <std::floating_point T>
void ProcessFloatingPoint(T value) {
    std::cout << "Processing floating point: " << value << std::endl;
}
void AutoWithConcepts() {
    ProcessInteger(42);       // int
    ProcessInteger(100L);     // long
    ProcessFloatingPoint(3.14);   // double
    ProcessFloatingPoint(2.5f);   // float
}


// auto as non-type template parameter (C++20)
template <auto Value>
void PrintAutoNTTP() {
    std::cout << "Non-type template parameter value: " << Value << std::endl;
}
void AutoAsTemplateParameter() {
    PrintAutoNTTP<42>();       // int
    PrintAutoNTTP<'A'>();      // char
    PrintAutoNTTP<true>();     // bool
    PrintAutoNTTP<3.14>();     // double (C++20)
}