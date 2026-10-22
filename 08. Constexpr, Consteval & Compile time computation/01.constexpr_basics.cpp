/*
    CONSTEXPR BASICS - COMPUTATION AT COMPILE TIME
    
    WHAT IS constexpr?
    constexpr is a keyword that tells the compiler: "This CAN be evaluated at compile time."
 
    - The key word is CAN - it's a PERMISSION, not a command.
    - The compiler MAY evaluate it at compile time if the inputs are compile-time constants, 
      but it can also run at runtime if needed.
  
    WHY DO WE CARE?
    Before constexpr, if you wanted a compile-time value (like a lookup table, a computed 
    constant, array sizes), you had to:
    1. Hard-code the value (error-prone)
    2. Use template metaprogramming (unreadable)
    3. Compute it at runtime (wasteful, can't use for array sizes)
 
    constexpr lets you write NORMAL C++ code that executes during compilation, giving you:
    - Zero runtime cost
    - Values usable as array sizes, template arguments
    - Compile-time error checking
    - No template metaprogramming headaches
  
   THE THREE LEVELS OF "CONST":
    1. const: "I won't change this at runtime" (value might still be computed at runtime)
    2. constexpr (variable): "Initialize this at compile time" (MUST be computable at compile time)
    3. constexpr (function): "This function CAN run at compile time" (if called with compile-time arguments)
 */

import std;
#include <iostream>
#include <string>
#include "constexpr_header.h"

void WhatIsConstexpr() {
    std::cout << "=== constexpr: The Foundation ===" << std::endl;

    /*
     * LEVEL 1: const (weakest guarantee)
     * =================================
     * const means "read-only after initialization."
     * The initialization CAN happen at runtime!
     */
    int runtime_input = 10;  // Imagine this comes from user input
    const int const_value = runtime_input * 2;  // Computed at RUNTIME
    // ^ This is legal! const just means we can't MODIFY it later.

    std::cout << "  const value: " << const_value << " (computed at runtime)" << std::endl;

    /*
     * LEVEL 2: constexpr variable (stronger guarantee)
     * ================================================================
     * constexpr means "computed at COMPILE time."
     * The initializer MUST be a compile-time expression.
     */
    constexpr int compile_time_value = 10 * 2;  // MUST be computable at compile time
    // constexpr int bad = runtime_input * 2;  // ERROR: runtime_input not known at compile time

    std::cout << "  constexpr value: " << compile_time_value << " (computed at compile time)" << std::endl;

    /*
     * WHY constexpr MATTERS - THE ARRAY SIZE EXAMPLE:
     * ===============================================
     */
     // Array sizes must be compile-time constants!
    int array[compile_time_value];  // OK: constexpr works as array size
    // int bad_array[const_value];  // ERROR (in C++): const_value not compile-time

    std::cout << "  Array of size " << compile_time_value << " created on stack" << std::endl;
}

void ConstexprVsConst() {
    std::cout << "\n=== const vs constexpr ===" << std::endl;

    /*
     * THE CRITICAL DIFFERENCE:
     * =======================
     *
     * const int x = get_input();     // OK: const, but runtime init
     * constexpr int y = get_input(); // ERROR: must be compile-time
     * constexpr int z = 5 + 3;       // OK: literal expression
     */

     // Both can be used for reading:
    const int c1 = 100;
    constexpr int c2 = 100;

    std::cout << "  const c1 = " << c1 << std::endl;
    std::cout << "  constexpr c2 = " << c2 << std::endl;

    // Only constexpr works in contexts requiring compile-time constants:
    // 1. Array sizes:
    int arr1[c2];        // OK: c2 is constexpr
    // int arr2[c1];      // ERROR (in standard C++): c1 is just const

    // 2. Template arguments:
    // std::array<int, c2> a1;  // OK
    // std::array<int, c1> a2;  // ERROR

    // 3. static_assert:
    static_assert(c2 == 100);      // OK: compile-time check
    // static_assert(c1 == 100);   // ERROR (in C++98/03 style)

    std::cout << "  Only constexpr works for: array sizes, template args, static_assert" << std::endl;
}

void TheEvolution() {
    std::cout << "\n=== Evolution of constexpr (Why it's so important) ===" << std::endl;

    std::cout << R"(
    THE HISTORY LESSON:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ C++98/03:                                           │
    │   • Only integral constants, enum values             │
    │   • Complex compile-time computation = templates    │
    │   • Template metaprogramming was painful             │
    │                                                     │
    │ C++11: constexpr introduced                          │
    │   • Single return statement: return expr;            │
    │   • Very restrictive but a start!                   │
    │                                                     │
    │ C++14: RELAXED!                                     │
    │   • Loops allowed!                                   │
    │   • Local variables!                                │
    │   • Mutation!                                        │
    │   • Now it's "normal programming"                   │
    │                                                     │
    │ C++17: if constexpr!                                 │
    │   • Compile-time branching in normal code            │
    │                                                     │
    │ C++20: REVOLUTION!                                  │
    │   • constexpr can use: try/catch, new/delete         │
    │   • constexpr virtual functions                      │
    │   • constexpr std::vector, std::string!              │
    │   • consteval (must be compile-time)                 │
    │                                                     │
    │ C++23:                                              │
    │   • if consteval (cleaner compile-time detection)    │
    │   • constexpr in more places                         │
    │                                                     │
    └─────────────────────────────────────────────────────┘

    THE PRACTICAL IMPACT:
    Before constexpr, compile-time computation looked like:
    ┌─────────────────────────────────────────────────────┐
    │ template<int N> struct Factorial {                  │
    │     static const int value = N * Factorial<N-1>::value; │
    │ };                                                  │
    │ template<> struct Factorial<0> {                    │
    │     static const int value = 1;                     │
    │ };                                                  │
    │ // Unreadable, hard to debug, template errors       │
    └─────────────────────────────────────────────────────┘

    With C++14 constexpr:
    ┌─────────────────────────────────────────────────────┐
    │ constexpr int factorial(int n) {                     │
    │     int result = 1;                                 │
    │     for (int i = 2; i <= n; ++i)                    │
    │         result *= i;                                │
    │     return result;                                  │
    │ }                                                   │
    │ // Readable, debuggable, normal code!               │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}