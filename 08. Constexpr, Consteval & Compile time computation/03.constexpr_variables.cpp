/*
    CONSTEXPR VARIABLES - VALUES BAKED INTO YOUR BINARY

    WHAT IS A constexpr VARIABLE?
    A variable whose value is computed during COMPILATION and stored directly in your 
    program's binary.
 
    THE METAPHOR - "BAKING" VALUES:
    Think of it like baking a cake:
     - Baking (compilation) happens ONCE
     - The finished cake (the value) is stored
     - Eating (running the program) just uses the stored result
     - You don't re-bake the cake every time you want a slice!
  
    RUNTIME COMPUTATION:                     CONSTEXPR COMPUTATION:
    Program starts                           Compiler runs
      → Compute value                          → Compute value
      → Use value                              → Store in binary
                                               → Program starts
                                               → Use stored value (instant!)
*/

import std;
#include <iostream>
#include <string>
#include <array>
#include "constexpr_header.h"

void ConstexprVariableBasics() {
    std::cout << "=== constexpr Variables ===" << std::endl;

    /*
     * THREE WAYS TO GET A CONSTANT:
     * ================================================================
     */

     // Method 1: Literal (always worked)
    constexpr int literal = 42;

    // Method 2: Constant expression (always worked)
    constexpr int expression = 6 * 7;

    // Method 3: Constexpr function call (the game-changer!)
    constexpr int computed = []() {
        int result = 0;
        for(int i = 1; i <= 10; ++i) {
            result += i;
        }
        return result;
        }();  // IIFE - Immediately Invoked

    static_assert(literal == 42);
    static_assert(expression == 42);
    static_assert(computed == 55);  // 1+2+3+...+10

    std::cout << "  literal: " << literal << std::endl;
    std::cout << "  expression: " << expression << std::endl;
    std::cout << "  computed: " << computed << " (loop at compile time!)" << std::endl;
}

void WhereConstexprRequired() {
    std::cout << "\n=== Where constexpr is REQUIRED ===" << std::endl;

    /*
     * CONTEXTS THAT DEMAND COMPILE-TIME CONSTANTS:
     * ============================================
     * If it needs to be known before the program runs, it needs constexpr.
     */

    constexpr int size = 10;
    constexpr int value = 99;

    // 1. ARRAY SIZES (C-style arrays):
    int array[size];  // Must know size at compile time
    std::cout << "  1. Array size: " << size << " OK" << std::endl;

    // 2. TEMPLATE ARGUMENTS:
    std::array<int, size> std_array;  // std::array needs compile-time size
    std::cout << "  2. Template argument (std::array): OK" << std::endl;

    // 3. static_assert CONDITIONS:
    static_assert(size > 5);  // Compile-time assertion
    std::cout << "  3. static_assert: OK" << std::endl;

    // 4. CASE LABELS IN SWITCH:
    switch(value) {
        case 99:  // Must be compile-time constant
            std::cout << "  4. Case label: OK" << std::endl;
            break;
        default:
            break;
    }

    // 5. BIT-FIELD WIDTHS:
    struct Flags {
        unsigned int a : 4;   // 4 must be compile-time
        unsigned int b : 8;
    };
    std::cout << "  5. Bit-field widths: OK" << std::endl;

    // 6. ENUM VALUES (implicitly constexpr):
    enum Color { Red = 0, Green = 100, Blue = 200 };
    std::cout << "  6. Enum values: implicitly constexpr" << std::endl;

    std::cout << R"(
    THE PATTERN:
    ┌─────────────────────────────────────────────────────┐
    │ If the compiler needs to know the value BEFORE      │
    │ your program runs, it MUST be constexpr.            │
    │                                                     │
    │ This includes: array sizes, template parameters,    │
    │ static_assert conditions, case labels, etc.         │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void CostComparison() {
    std::cout << "\n=== The Cost Savings ===" << std::endl;

    /*
     * WHAT YOU SAVE BY USING constexpr:
     * ================================================================
     */

     // EXPENSIVE COMPUTATION: pretend this takes milliseconds
    constexpr int expensive_computation(int n) {
        // Pretend this is a complex simulation
        int result = 0;
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                result += i * j;
            }
        }
        return result;
    }

    // RUNTIME VERSION: computes EVERY time program runs
    // (commented out because it's slow - just to show the contrast)
    // int runtime_result = expensive_computation(1000);  // Every program start!

    // CONSTEXPR VERSION: computed ONCE during compilation
    constexpr int compile_time_result = expensive_computation(100);

    std::cout << "  Compile-time result: " << compile_time_result << std::endl;
    std::cout << "  (Computed during compilation - FREE at runtime!)" << std::endl;

    std::cout << R"(
    THE SAVINGS:
    ┌─────────────────────────────────────────────────────┐
    │ WITHOUT constexpr:                                  │
    │   Every program start:                              │
    │     → Run computation (milliseconds?)              │
    │     → Waste CPU cycles                              │
    │     → Slower startup                                │
    │                                                     │
    │ WITH constexpr:                                     │
    │   During compilation (ONE TIME):                   │
    │     → Run computation                               │
    │     → Store result in binary                        │
    │   Every program start:                              │
    │     → Read stored value (nanoseconds)              │
    │     → Zero CPU cost                                 │
    │     → Instant access                                │
    └─────────────────────────────────────────────────────┘

    USE THIS FOR:
    • Mathematical constants (tables, coefficients)
    • Configuration that's known at compile time
    • Lookup tables (sine tables, CRC tables)
    • Anything computed once and read many times
)" << std::endl;
}