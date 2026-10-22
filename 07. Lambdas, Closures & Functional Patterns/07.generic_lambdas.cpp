import std;
/*
 * ============================================================================
 * GENERIC LAMBDAS (C++14) - THE 'auto' REVOLUTION
 * ============================================================================
 *
 * WHAT ARE GENERIC LAMBDAS?
 * =========================
 * Before C++14, lambda parameters had CONCRETE types:
 *   auto add = [](int a, int b) { return a + b; };
 *   // Can ONLY add ints. Need a different lambda for doubles.
 *
 * C++14 introduced 'auto' parameters, making lambdas GENERIC:
 *   auto add = [](auto a, auto b) { return a + b; };
 *   // Works with ints, doubles, strings, ANY type that supports +
 *
 * HOW IT WORKS:
 * =============
 * Each 'auto' in the parameter list is secretly a TEMPLATE parameter.
 * The compiler generates a special version for each type combination.
 *
 *   auto add = [](auto a, auto b) { return a + b; };
 *
 * Is roughly equivalent to:
 *   struct Lambda {
 *       template <typename T, typename U>
 *       auto operator()(T a, U b) const { return a + b; }
 *   };
 *
 * This is why they're called "generic lambdas" - they're
 * function templates in disguise!
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "functional_header.h"

void GenericLambdaBasics() {
    std::cout << "=== Generic Lambda Basics ===" << std::endl;

    // ONE lambda works with MANY types
    auto add = [](auto a, auto b) {
        return a + b;
        };

    // Each call instantiates a different version:
    std::cout << "  add(1, 2) = " << add(1, 2) << "          (int + int)" << std::endl;
    std::cout << "  add(1.5, 2.5) = " << add(1.5, 2.5) << "    (double + double)" << std::endl;
    std::cout << "  add(1, 2.5) = " << add(1, 2.5) << "       (int + double -> double)" << std::endl;
    std::cout << "  add(\"a\", \"b\") = " << add("a", "b") << "    (string concat!)" << std::endl;

    // Even works with std::string:
    std::string hello = "Hello, ";
    std::string world = "World!";
    std::cout << "  add(string, string) = " << add(hello, world) << std::endl;

    /*
     * THE KEY INSIGHT:
     * ================
     * Each 'auto' is an INDEPENDENT template parameter.
     * So (auto a, auto b) means a and b can be DIFFERENT types.
     * That's why add(1, 2.5) works - a is int, b is double.
     */
}

void DifferentTypesPerParameter() {
    std::cout << "\n=== Independent Type Deduction ===" << std::endl;

    // Each auto parameter gets its own deduced type
    // This is different from template <typename T> void f(T a, T b)
    // which would require BOTH to be the same type!

    auto print_pair = [](auto first, auto second) {
        std::cout << "  (" << first << ", " << second << ")" << std::endl;
        };

    print_pair(42, "answer");        // int, const char*
    print_pair(3.14, 'c');           // double, char
    print_pair("str", std::vector<int>{1, 2, 3}.size());  // const char*, size_t

    // Contrast: if this were template <typename T> (T a, T b),
    // then print_pair(42, "answer") would FAIL because
    // T can't be both int and const char* simultaneously.
}

void GenericLambdasWithSTL() {
    std::cout << "\n=== Generic Lambdas with STL ===" << std::endl;

    // GENERIC PRINT - works with any printable type
    std::vector<int> ints = {1, 2, 3, 4, 5};
    std::vector<std::string> strings = {"one", "two", "three"};

    auto print_all = [](const auto& container) {
        // 'auto' here works with vector<int>, vector<string>, etc.
        std::cout << "  [";
        for(const auto& elem : container) {
            std::cout << " " << elem;
        }
        std::cout << " ]" << std::endl;
        };

    print_all(ints);
    print_all(strings);

    // GENERIC FIND - works with any container
    auto contains = [](const auto& container, const auto& value) {
        // Uses begin/end which works for most containers
        return std::find(container.begin(), container.end(), value)
            != container.end();
        };

    std::cout << "  ints contains 3: " << contains(ints, 3) << std::endl;
    std::cout << "  ints contains 99: " << contains(ints, 99) << std::endl;
    std::cout << "  strings contains \"two\": " << contains(strings, std::string("two")) << std::endl;
}

void Cxx20Improvements() {
    std::cout << "\n=== C++20: Template Syntax for Lambdas ===" << std::endl;

    /*
     * C++20 ADDITION: EXPLICIT TEMPLATE PARAMETER LISTS
     * =================================================
     * Sometimes 'auto' isn't enough - you need to express
     * constraints or reference the deduced types.
     *
     * C++20 allows:
     *   []<typename T>(T value) { ... }
     *
     * This gives you a NAME for the deduced type.
     */

     // C++14: Can do this, but can't reference the type itself
    auto old_style = [](const auto& container) {
        // Can use the container, but can't say "its element type"
        return container.size();
        };

    // C++20: Can name and use the type
    auto new_style = []<typename Container>(const Container & c) {
        // Now we can use Container as a type!
        // typename Container::value_type is accessible
        using Element = typename Container::value_type;

        Element total{};
        for(const auto& elem : c) {
            total += elem;
        }
        return total;
    };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "  old_style size: " << old_style(vec) << std::endl;
    std::cout << "  new_style sum: " << new_style(vec) << std::endl;

    // C++20 also allows template parameter packs in lambdas:
    auto count_types = []<typename... Ts>(Ts... args) {
        return sizeof...(args);
    };
    std::cout << "  count_types(1, 2.5, 'x') = " << count_types(1, 2.5, 'x') << std::endl;
}

void WhenToUseGeneric(){
    std::cout << "\n=== When to Use Generic Lambdas ===" << std::endl;

    std::cout << R"(
        USE GENERIC LAMBDAS WHEN:
        ┌─────────────────────────────────────────────────────┐
        │ • Writing helpers used with multiple types          │
        │ • Working with STL containers generically           │
        │ • Creating utility functions (print, compare, etc.) │
        │ • The operation makes sense for many types          │
        └─────────────────────────────────────────────────────┘

        PREFER SPECIFIC TYPES WHEN:
        ┌─────────────────────────────────────────────────────┐
        │ • The logic only makes sense for one type           │
        │ • You want to PREVENT other types (type safety)     │
        │ • Performance is critical (avoids template bloat)   │
        │ • The API contract needs to be clear                │
        └─────────────────────────────────────────────────────┘

        NOTE: Generic lambdas generate code for EACH type used.
        If you call a generic lambda with 10 different types,
        you get 10 different function bodies in your binary.
        This is "template bloat" - usually fine, but be aware.
    )" << std::endl;
}