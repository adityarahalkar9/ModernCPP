import std;
/*
 * ============================================================================
 * CURRYING AND PARTIAL APPLICATION
 * ============================================================================
 *
 * WHAT IS CURRYING?
 * =================
 * Named after Haskell Curry (the mathematician, not the food!).
 *
 * Currying transforms a function that takes MULTIPLE arguments
 * into a CHAIN of functions that each take ONE argument.
 *
 * UNCURRIED:        f(a, b, c)
 * CURRIED:          f(a)(b)(c)
 *
 * The curried version returns a new function at each step.
 *
 * WHAT IS PARTIAL APPLICATION?
 * ============================
 * Closely related. Partial application fixes SOME arguments
 * of a function, creating a new function with the remaining ones.
 *
 * Original:    f(a, b, c)
 * Partial:     g = f(_, b, c)  -- 'a' is still needed
 *              g(x) = f(x, b, c)
 *
 * WHY DO THIS?
 * ============
 * 1. Create specialized versions of general functions
 * 2. Reuse common configurations
 * 3. Enable function composition
 * 4. Delay evaluation until all arguments are available
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "functional_header.h"

void CurryingBasics() {
    std::cout << "=== Currying Basics ===" << std::endl;

    /*
     * MANUAL CURRYING - THE CONCEPT:
     * ==============================
     */

     // UNCURRIED: takes all arguments at once
    auto add3_uncurried = [](int a, int b, int c) {
        return a + b + c;
        };

    // CURRIED: takes one argument, returns function for the rest
    auto add3_curried = [](int a) {
        // Returns a function that takes b
        return [a](int b) {
            // Returns a function that takes c
            return [a, b](int c) {
                // Finally computes the result
                return a + b + c;
                };
            };
        };

    // Using the uncurried version (all at once):
    int result1 = add3_uncurried(1, 2, 3);
    std::cout << "  Uncurried: " << result1 << std::endl;

    // Using the curried version (step by step):
    auto add1 = add3_curried(1);        // Returns function: (b, c) pending
    auto add12 = add1(2);               // Returns function: (c) pending
    int result2 = add12(3);             // Finally computes: 1+2+3

    std::cout << "  Curried step by step: " << result2 << std::endl;

    // Or all in one expression (looks the same as the call!):
    int result3 = add3_curried(1)(2)(3);
    std::cout << "  Curried inline: " << result3 << std::endl;
}

void PartialApplication() {
    std::cout << "\n=== Partial Application ===" << std::endl;

    /*
     * PARTIAL APPLICATION - THE PRACTICAL VERSION:
     * ============================================
     * You don't always need FULL currying.
     * Often you just want to fix SOME arguments.
     */

     // A general function:
    auto power = [](double base, int exp) {
        return std::pow(base, exp);
        };

    // Create specialized versions by partial application:
    // (Manually - capturing the fixed argument)
    auto square = [power](double x) {
        return power(x, 2);    // exp is fixed at 2
        };

    auto cube = [power](double x) {
        return power(x, 3);    // exp is fixed at 3
        };

    auto sqrt_custom = [power](double x) {
        return power(x, 0.5);  // exp is fixed at 0.5
        };

    std::cout << "  square(5) = " << square(5) << std::endl;
    std::cout << "  cube(3) = " << cube(3) << std::endl;
    std::cout << "  sqrt(16) = " << sqrt_custom(16) << std::endl;

    /*
     * WITH std::bind (older style):
     * =============================
     * auto square = std::bind(power, std::placeholders::_1, 2);
     * The _1 means "first argument comes later"
     */
}

void PracticalCurrying() {
    std::cout << "\n=== Practical Uses of Partial Application ===" << std::endl;

    /*
     * USE CASE 1: Logger with configuration
     * =====================================
     */

     // General logging function:
    auto log_message = [](const std::string& format,
                          const std::string& prefix,
                          const std::string& message) {
                              std::cout << "  [" << prefix << "] " << format << ": " << message << std::endl;
        };

    // Create specialized loggers:
    auto error_logger = [log_message](const std::string& msg) {
        log_message("ERROR", "app", msg);
        };

    auto debug_logger = [log_message](const std::string& msg) {
        log_message("DEBUG", "app", msg);
        };

    error_logger("Something went wrong");
    debug_logger("Just checking in");

    /*
     * USE CASE 2: Validator generation
     * ================================
     */
    auto make_range_checker = [](std::string field_name,
                                 double min, double max) {
                                     // Returns a fully configured validator
                                     return [field_name, min, max](double value) -> std::string {
                                         if(value < min) {
                                             return field_name + " too low (min: " + std::to_string(min) + ")";
                                         }
                                         if(value > max) {
                                             return field_name + " too high (max: " + std::to_string(max) + ")";
                                         }
                                         return field_name + " valid";
                                         };
        };

    // Create specialized validators:
    auto check_age = make_range_checker("Age", 18, 65);
    auto check_score = make_range_checker("Score", 0, 100);
    auto check_temp = make_range_checker("Temperature", -40, 120);

    std::cout << "\n  " << check_age(25) << std::endl;
    std::cout << "  " << check_age(10) << std::endl;
    std::cout << "  " << check_score(150) << std::endl;
    std::cout << "  " << check_temp(98.6) << std::endl;
}

void CurryingVsPartial() {
    std::cout << "\n=== Currying vs Partial Application ===" << std::endl;

    std::cout << R"(
    THE DIFFERENCE (often confused):
    ┌─────────────────────────────────────────────────────────────┐
    │                                                             │
    │  CURRYING:                                                  │
    │  • Transforms f(a,b,c) into f(a)(b)(c)                       │
    │  • Automatic - takes ANY multi-arg function                 │
    │  • ALL arguments become optional/deferred                    │
    │  • It's a structural transformation                          │
    │                                                             │
    │  PARTIAL APPLICATION:                                       │
    │  • Fixes SOME arguments: f(a,b,c) → f(_,b,c)                 │
    │  • You choose which arguments to fix                        │
    │  • Creates a SIMPLER function with fewer parameters         │
    │  • It's about specialization                                │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘

    WHEN TO USE:
    ┌─────────────────────────────────────────────────────────────┐
    │ Use PARTIAL APPLICATION when:                               │
    │  • You have a specific configuration to reuse              │
    │  • You want to create named, specialized functions         │
    │                                                             │
    │ Use CURRYING when:                                          │
    │  • You're doing heavy functional composition                │
    │  • Working with functional programming libraries           │
    │  • You need maximum flexibility in argument order          │
    └─────────────────────────────────────────────────────────────┘
)" << std::endl;
}