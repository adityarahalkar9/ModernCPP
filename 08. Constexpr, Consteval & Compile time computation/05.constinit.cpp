import std;
/*
    CONSTINIT - GUARANTEED STATIC INITIALIZATION
    
    WHAT IS constinit?
    constinit ensures a variable with static storage duration is initialized at 
    COMPILE TIME (static initialization), not at program startup (dynamic initialization).
 
    THE PROBLEM IT SOLVES:

    "Static initialization order fiasco" - a legendary C++ bug source:
 
    // File A.cpp
       int global_a = compute_something();  // When does this run?
 
    // File B.cpp
       int global_b = global_a + 1;     // What if this runs BEFORE A is initialized?
 
    If global_b initializes first, it reads an UNINITIALIZED global_a → garbage!
 
    THE SOLUTION:
    constinit guarantees the value is available at COMPILE TIME, so there's no ordering 
    issue. The value is baked into the binary.
 
    constinit vs constexpr:
    - constexpr: Must be compile-time AND the type must be a literal type.
    - constinit: Must be compile-time initialization, but the variable itself can be 
                 MUTABLE at runtime!
*/

#include <iostream>
#include <string>
#include "constexpr_header.h"

 // Global variables demonstrating the problem and solution
int compute_expensive() {
    // Pretend this is expensive
    return 42;
}

// PROBLEM: This might initialize at RUNTIME (dynamic initialization)
int runtime_global = compute_expensive();

// SOLUTION: This is guaranteed compile-time (static initialization)
constinit int compile_time_global = 100 + 200;  // Must be constant expression

// Note: constinit variable is NOT const - it can be modified at runtime!
// But its INITIAL value is compile-time.

void ConstinitBasics() {
    std::cout << "=== constinit: Guaranteed Static Initialization ===" << std::endl;

    std::cout << "  runtime_global = " << runtime_global << std::endl;
    std::cout << "  compile_time_global = " << compile_time_global << std::endl;

    // constinit variables CAN be modified (unlike constexpr):
    compile_time_global = 500;  // OK: it's not const!
    std::cout << "  After modification: " << compile_time_global << std::endl;

    std::cout << R"(
    THE THREE INITIALIZATION GUARANTEES:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ int x = f();        (no specifier)                  │
    │   → Might be runtime init (dynamic initialization)  │
    │   → Order across files is UNDEFINED!               │
    │                                                     │
    │ constexpr int x = f();                              │
    │   → Compile-time init                               │
    │   → x is CONST (can't modify later)                 │
    │                                                     │
    │ constinit int x = f();                              │
    │   → Compile-time init (guaranteed!)                 │
    │   → x is MUTABLE (can modify at runtime)            │
    │   → Solves static init order fiasco!                │
    └─────────────────────────────────────────────────────┘

    WHEN TO USE constinit:
    • Global variables that need a compile-time initial value
    • But that will be MODIFIED during the program
    • Eliminates static initialization order bugs
)" << std::endl;
}

void StaticInitOrderFiasco() {
    std::cout << "\n=== The Static Initialization Order Fiasco ===" << std::endl;

    /*
     * THE LEGENDARY BUG:
     * ================================================================
     * This has plagued C++ programmers for decades.
     */

    std::cout << R"(
    THE SCENARIO:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ File: config.cpp                                    │
    │   int config_value = load_config_from_file();       │
    │   (dynamic initialization - runs at startup)        │
    │                                                     │
    │ File: main.cpp                                      │
    │   int threshold = config_value * 2;                 │
    │   (also dynamic initialization)                     │
    │                                                     │
    │ PROBLEM: Which initializes FIRST?                   │
    │ • If config_value first: threshold = correct_value * 2 │
    │ • If threshold first: threshold = 0 * 2 = 0 (BUG!)  │
    │                                                     │
    │ The order is UNDEFINED - varies by compiler,        │
    │ linker settings, and even build configuration!      │
    │                                                     │
    │ SYMPTOMS:                                           │
    │ • "It works in debug but not release"               │
    │ • "It works on my machine but not CI"               │
    │ • Random behavior on different systems              │
    │ • Values that "should be there" are 0               │
    │                                                     │
    │ THE FIX WITH constinit:                             │
    │ ┌─────────────────────────────────────────────────┐ │
    │ │ constinit int config_value = 42;  // Fixed at   │ │
    │ │ // compile time - always available before      │ │
    │ │ // any code runs!                              │ │
    │ │ int threshold = config_value * 2;  // Now safe  │ │
    │ └─────────────────────────────────────────────────┘ │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ConstinitVsConstexpr() {
    std::cout << "\n=== constinit vs constexpr ===" << std::endl;

    std::cout << R"(
    COMPARISON TABLE:
    ┌──────────────────┬────────────────┬────────────────┐
    │                  │ constexpr      │ constinit      │
    ├──────────────────┼────────────────┼────────────────┤
    │ Initial value    │ Compile-time   │ Compile-time   │
    │                  │ (guaranteed)   │ (guaranteed)   │
    ├──────────────────┼────────────────┼────────────────┤
    │ Mutable later?   │ NO (const)     │ YES!           │
    ├──────────────────┼────────────────┼────────────────┤
    │ Zero runtime cost│ YES            │ YES            │
    │ for initial value│                │                │
    ├──────────────────┼────────────────┼────────────────┤
    │ Use case         │ True constants │ Mutable globals│
    │                  │ that never     │ with compile-  │
    │                  │ change         │ time init      │
    └──────────────────┴────────────────┴────────────────┘

    EXAMPLE USE CASES:
    ┌─────────────────────────────────────────────────────┐
    │ constexpr:                                          │
    │   constexpr double PI = 3.14159;  // Never changes  │
    │   constexpr int MAX_SIZE = 1000;   // Never changes │
    │                                                     │
    │ constinit:                                          │
    │   constinit int counter = 0;       // Starts at 0,  │
    │   // will be incremented during program             │
    │                                                     │
    │   constinit bool initialized = false; // Will be    │
    │   // set to true after setup                        │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}