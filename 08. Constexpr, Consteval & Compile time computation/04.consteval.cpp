/*
 * CONSTEVAL - IMMEDIATE FUNCTIONS (C++20)
 *
 * WHAT IS consteval?
 * ==================
 * consteval declares an "immediate function" - a function that
 * MUST be evaluated at compile time. No exceptions.
 *
 * constexpr vs consteval:
 * =======================
 * constexpr: "CAN be compile-time" (might be runtime)
 * consteval: "MUST be compile-time" (runtime is an ERROR)
 *
 * THE PRACTICAL DIFFERENCE:
 * =========================
 *   constexpr int f(int x) { return x*2; }
 *   consteval int g(int x) { return x*2; }
 *
 *   int runtime = 5;
 *   int a = f(runtime);  // OK: f runs at runtime
 *   int b = g(runtime);  // COMPILE ERROR: g must be compile-time!
 *   int c = g(10);       // OK: 10 is a literal, computed at compile time
 *
 * WHY DOES consteval EXIST?
 * =========================
 * 1. Some things ONLY make sense at compile time:
 *    - Template argument computation
 *    - Array sizes
 *    - static_assert conditions
 *
 * 2. Catching bugs: If you THINK it's compile-time but it's not,
 *    consteval turns a silent performance issue into a compile error.
 */

import std;
#include <iostream>
#include <string>
#include "constexpr_header.h"

void ConstevalBasics() {
    std::cout << "=== consteval: Must Be Compile-Time ===" << std::endl;

    /*
     * THE STRICT VERSION OF constexpr
     * ================================================================
     */

     // A regular constexpr function (flexible):
    constexpr int flexible_square(int x) {
        return x * x;
    }

    // A consteval function (strict):
    consteval int strict_square(int x) {
        return x * x;
    }

    // BOTH work with compile-time arguments:
    constexpr int a = flexible_square(12);  // OK: compile-time
consteval_check_a:
    constexpr int b = strict_square(12);    // OK: compile-time (required!)

    std::cout << "  flexible_square(12) = " << a << std::endl;
    std::cout << "  strict_square(12) = " << b << std::endl;

    // THE DIFFERENCE appears with RUNTIME arguments:
    int runtime_input = 5;  // Pretend this comes from user/std::cin

    int c = flexible_square(runtime_input);  // OK: runs at runtime
    // int d = strict_square(runtime_input);  // COMPILE ERROR!
    // ^ Error: "call to immediate function is not a constant expression"

    std::cout << "  flexible with runtime input: " << c << " (computed at runtime)" << std::endl;
    std::cout << "  strict with runtime input: COMPILE ERROR (by design!)" << std::endl;
}

void WhenToUseConsteval() {
    std::cout << "\n=== When to Use consteval ===" << std::endl;

    /*
     * USE CASES FOR consteval:
     * ========================
     */

     // USE CASE 1: Compile-time validation
     // You want to ensure something is checked AT COMPILE TIME.
    consteval int validate_positive(int x) {
        if(x <= 0) {
            // In a consteval function, this effectively becomes a
            // compile-time error when reached at compile time
            return -1;  // Signal invalid (in real code, might want stronger error)
        }
        return x;
    }

    constexpr int valid = validate_positive(100);  // OK
    // constexpr int invalid = validate_positive(-5);  // ERROR at compile time!

    std::cout << "  Valid value: " << valid << " (validated at compile time!)" << std::endl;

    // USE CASE 2: Ensuring array sizes are truly compile-time
    consteval size_t array_size(size_t elements, size_t safety_margin = 1.2) {
        return static_cast<size_t>(elements * safety_margin);
    }

    // This MUST be compile-time, so it works for array declaration:
    int buffer[array_size(100)];  // OK: guaranteed compile-time
    std::cout << "  Buffer size: " << array_size(100) << " (guaranteed compile-time)" << std::endl;

    // USE CASE 3: Template argument computation
    consteval int power_of_2(int n) {
        int result = 1;
        for(int i = 0; i < n; ++i) {
            result *= 2;
        }
        return result;
    }

    // Template arguments MUST be compile-time:
    std::array<int, power_of_2(5)> power_array;  // std::array<int, 32>
    std::cout << "  Array with 2^5 = " << power_of_2(5) << " elements" << std::endl;

    std::cout << R"(
    DECISION GUIDE:
    ┌─────────────────────────────────────────────────────┐
    │ Use constexpr when:                                 │
    │   • Function might be called at runtime too          │
    │   • You want flexibility                            │
    │   • Performance benefit either way                  │
    │                                                     │
    │ Use consteval when:                                 │
    │   • Compile-time is REQUIRED (not optional)          │
    │   • Result used for array sizes, templates          │
    │   • You want to CATCH accidental runtime use        │
    │   • Zero runtime cost is mandatory                  │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ConstevalAsApiDesign() {
    std::cout << "\n=== consteval as API Design ===" << std::endl;

    /*
     * CONSTEVAL AS DOCUMENTATION AND ENFORCEMENT:
     * ==========================================
     * When you write a consteval function, you're telling users:
     * "This is ONLY for compile-time use. Don't even try to call it at runtime."
     *
     * This is better than a comment because the COMPILER enforces it!
     */

     // A library author writes this:
    consteval uint32_t compile_time_hash(const char* s) {
        uint32_t hash = 5381;
        while(*s) {
            hash = ((hash << 5) + hash) + *s++;
        }
        return hash;
    }

    // Users CANNOT accidentally use it at runtime:
    // The following would be a compile error:
    // std::string user_input = getUserInput();
    // uint32_t h = compile_time_hash(user_input.c_str());  // ERROR!

    // But this works perfectly:
    constexpr uint32_t known_hash = compile_time_hash("hello");
    std::cout << "  Hash of 'hello': " << known_hash << " (compile-time)" << std::endl;

    /*
     * THE PATTERN - Compile-Time String Hashing:
     * ==========================================
     * This is used in game engines, serialization libraries, etc.
     * The hash is computed during compilation, so at runtime
     * you're just comparing integers - extremely fast!
     */
}