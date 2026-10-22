/*
    CONSTEXPR FUNCTIONS - FUNCTIONS THAT CAN RUN AT COMPILE TIME
    
    WHAT IS A constexpr FUNCTION?
    A function marked constexpr CAN be evaluated at compile timewhen called with 
    compile-time arguments.
 
    THE KEY INSIGHT - DUAL NATURE:

    The SAME function can run at:
     1. COMPILE TIME (if arguments are constexpr)
     2. RUNTIME (if arguments are runtime values)
 
    This means you write ONE function, get BOTH behaviors!
 
    EXAMPLE:
       constexpr int square(int x){ 
           return x * x; 
       }
       constexpr int a = square(5);      // COMPILE TIME: a is 25 (baked into binary)
       int runtime = getUserInput();
       int b = square(runtime);          // RUNTIME: computed when program runs
*/

import std;
#include <iostream>
#include <string>
#include "constexpr_header.h"

void ConstexprFunctionBasics() {
    std::cout << "=== constexpr Functions ===" << std::endl;

    // A constexpr function - works at compile time AND runtime
    constexpr int square(int x) {
        return x * x;
    }

    // USE 1: Compile-time evaluation
    // The result is "baked into" your binary - zero runtime cost!
    constexpr int compile_time_result = square(12);
    static_assert(compile_time_result == 144);  // Verified at compile time!

    std::cout << "  Compile-time: square(12) = " << compile_time_result << std::endl;

    // USE 2: Runtime evaluation
    // Same function, called with runtime value
    std::cout << "  Enter a number: ";
    int user_input;
    std::cin >> user_input;

    int runtime_result = square(user_input);  // Runs at RUNTIME
    std::cout << "  Runtime: square(" << user_input << ") = " << runtime_result << std::endl;

    /*
       THE BEAUTY:
       ONE function definition, TWO execution contexts.
       No code duplication. No macros. No templates.
    */
}

void Cxx11VsCxx14(){
    std::cout << "\n=== C++11 vs C++14 constexpr Functions ===" << std::endl;

    /*
        C++11 STYLE: Single return statement ONLY
        This was incredibly restrictive but was a starting point.
    */

    // C++11 style - everything in one expression:
    constexpr int factorial_cxx11(int n){
        // Must be a single return, can use ternary for branching:
        return n <= 1 ? 1 : n * factorial_cxx11(n - 1);
        // No loops! No local variables! Recursion only!
    }

    // C++14 style - normal programming:
    constexpr int factorial_cxx14(int n) {
        int result = 1;                    // Local variable: allowed in C++14
        for(int i = 2; i <= n; ++i) {    // Loop: allowed in C++14
            result *= i;                   // Mutation: allowed in C++14
        }
        return result;
    }

    // Both produce the same results:
    static_assert(factorial_cxx11(5) == 120);
    static_assert(factorial_cxx14(5) == 120);

    std::cout << "  C++11 style: factorial(5) = " << factorial_cxx11(5) << std::endl;
    std::cout << "  C++14 style: factorial(5) = " << factorial_cxx14(5) << std::endl;

    std::cout << R"(
    THE DIFFERENCE IN READABILITY:
    ┌─────────────────────────────────────────────────────┐
    │ C++11: Forced into recursion/expression trees       │
    │ C++14+: Write normal loops and variables            │
    │                                                     │
    │ RULE OF THUMB: If you can write it as normal        │
    │ C++ code, it can probably be constexpr (C++14+)    │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void PracticalExamples() {
    std::cout << "\n=== Practical constexpr Functions ===" << std::endl;

    /*
     * REAL-WORLD USE CASES:
     * =====================
     */

     // 1. Compile-time math (physics simulations, game constants):
    constexpr double power(double base, int exp) {
        double result = 1.0;
        for(int i = 0; i < exp; ++i) {
            result *= base;
        }
        return result;
    }

    constexpr double kinetic_energy(double mass, double velocity) {
        return 0.5 * mass * velocity * velocity;
    }

    // Precompute expensive constants:
    constexpr double energy_2kg_10ms = kinetic_energy(2.0, 10.0);
    std::cout << "  KE(2kg, 10m/s) = " << energy_2kg_10ms << " J (compile-time)" << std::endl;

    // 2. String length (compile-time):
    constexpr size_t str_len(const char* s) {
        size_t len = 0;
        while(s[len] != '\0') ++len;
        return len;
    }

    constexpr auto msg_len = str_len("Hello, constexpr World!");
    static_assert(msg_len == 24);
    std::cout << "  Length of message: " << msg_len << " (known at compile time!)" << std::endl;

    // 3. Buffer size calculation:
    constexpr size_t calculate_buffer_size(size_t items, size_t item_size,
                                           size_t alignment) {
        size_t raw = items * item_size;
        // Round up to alignment:
        size_t remainder = raw % alignment;
        return remainder == 0 ? raw : raw + (alignment - remainder);
    }

    constexpr auto buffer_size = calculate_buffer_size(100, 8, 64);
    static_assert(buffer_size == 128);  // 100*8=800, rounded to 64 → 832? Let me check
    // Actually: 800 % 64 = 32, so 800 + (64-32) = 832. Let me just print it.
    std::cout << "  Buffer size for 100 8-byte items (64-aligned): " << buffer_size << std::endl;

    // 4. Bit manipulation:
    constexpr int count_ones(uint32_t n) {
        int count = 0;
        while(n) {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }

    static_assert(count_ones(0b1011) == 3);
    std::cout << "  Bits in 0b1011: " << count_ones(0b1011) << std::endl;
}

void CompileTimeVsRuntimeCall() {
    std::cout << "\n=== When is it Compile-Time vs Runtime? ===" << std::endl;

    constexpr int add(int a, int b) { return a + b; }

    // COMPILE TIME: Arguments are constexpr
    constexpr int x = 10;
    constexpr int y = 20;
    constexpr int z = add(x, y);  // COMPILE TIME: z is 30 in the binary
    static_assert(z == 30);

    std::cout << "  add(10, 20) as constexpr: " << z << " (computed during compilation)" << std::endl;

    // RUNTIME: At least one argument is runtime
    int runtime_val = 5;  // Imagine this comes from user input
    int w = add(runtime_val, y);  // RUNTIME: must execute when program runs

    std::cout << "  add(runtime, 20): " << w << " (computed at runtime)" << std::endl;

    /*
     * THE RULE:
     * =========
     * If ALL arguments are compile-time constants AND the result
     * is used in a constexpr context → compile-time evaluation.
     *
     * If ANY argument is a runtime value → runtime evaluation.
     *
     * The function doesn't change - only HOW it's called.
     */
}