import std;
/*
 * ============================================================================
 * CLOSURES - LAMBDAS + THEIR ENVIRONMENT
 * ============================================================================
 *
 * WHAT IS A CLOSURE?
 * ==================
 * A closure is a function BUNDLED TOGETHER with the environment
 * (variables) it captured. The function "closes over" those variables,
 * hence the name.
 *
 * THE TECHNICAL DEFINITION:
 * =========================
 * - LAMBDA = the SYNTAX/expression you write: [](int x) { return x; }
 * - CLOSURE = the OBJECT that results: the function + its captured state
 *
 * Every lambda expression creates a closure object.
 * The closure object is what you store in a variable.
 *
 * THINK OF IT LIKE THIS:
 * ======================
 * A regular function is like a recipe card - it's the same
 * no matter where you use it.
 *
 * A closure is like a recipe card WITH your personal notes
 * written in the margins - it remembers YOUR specific context.
 *
 * WHY CLOSURES MATTER:
 * ====================
 * 1. They enable FUNCTIONAL PROGRAMMING patterns
 * 2. They allow CALLBACKS with STATE
 * 3. They support LAZY EVALUATION
 * 4. They're fundamental to JavaScript, Python, etc.
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include "functional_header.h"

void WhatIsAClosure() {
    std::cout << "=== Understanding Closures ===" << std::endl;

    /*
     * THE LAMBDA vs THE CLOSURE:
     * =========================
     *
     * This is a LAMBDA (the expression):
     *   [](int x) { return x + offset; }
     *
     * When evaluated, it creates a CLOSURE (the object):
     *   - The function code (the body)
     *   - The captured 'offset' value (the environment)
     *
     * The closure is what gets stored in 'adder'
     */

    int offset = 10;  // This is part of the "environment"

    // The lambda expression creates a closure that "closes over" 'offset'
    auto adder = [offset](int x) {
        return x + offset;  // 'offset' is part of the closure's state
        };

    // 'adder' is now a CLOSURE: function + captured environment
    // You can pass it around, and it always knows its 'offset'

    std::cout << "  adder(5) = " << adder(5) << std::endl;

    // Create ANOTHER closure from the SAME lambda structure
    // but with a DIFFERENT environment
    int different_offset = 100;
    auto adder2 = [different_offset](int x) {
        return x + different_offset;
        };

    std::cout << "  adder2(5) = " << adder2(5) << " (different closure!)" << std::endl;

    /*
     * KEY INSIGHT:
     * ============
     * Each time you evaluate a lambda expression, you get a NEW closure.
     * The closures are INDEPENDENT - they have their own captured state.
     */
}

void ClosureFactory() {
    std::cout << "\n=== Pattern: Closure Factory ===" << std::endl;

    /*
     * THE POWER OF CLOSURES: Creating customized functions
     * ====================================================
     * This is where closures shine. You can write a function
     * that CREATES and RETURNS other functions, each customized
     * with different captured state.
     *
     * This pattern is called a "closure factory" or "function factory."
     */

     // A factory that creates "multiplier" functions
     // Each created function remembers its own multiplier
    auto make_multiplier = [](int factor) {
        // This lambda RETURNS another lambda
        // The returned lambda CLOSES OVER 'factor'
        return [factor](int value) {
            return value * factor;
            };
        };

    // Create specialized functions:
    auto double_it = make_multiplier(2);   // Closure with factor=2
    auto triple_it = make_multiplier(3);   // Closure with factor=3
    auto times_ten = make_multiplier(10);  // Closure with factor=10

    // Each function has its own "baked-in" behavior:
    std::cout << "  double_it(5) = " << double_it(5) << std::endl;
    std::cout << "  triple_it(5) = " << triple_it(5) << std::endl;
    std::cout << "  times_ten(5) = " << times_ten(5) << std::endl;

    /*
     * WHAT'S HAPPENING:
     * =================
     * When you call make_multiplier(2):
     *   1. 'factor' parameter = 2
     *   2. The inner lambda captures factor=2
     *   3. A closure is created with state {factor: 2}
     *   4. That closure is returned
     *
     * The 'factor' parameter is long gone, but the closure
     * still has its own copy. This is the "closing over" part.
     */
}

void StatefulClosures() {
    std::cout << "\n=== Closures with Mutable State ===" << std::endl;

    /*
     * CLOSURES THAT REMEMBER (state across calls)
     * ===========================================
     * A closure's captured variables persist between calls.
     * This makes closures useful for STATEFUL behavior.
     */

     // A closure that acts as a counter
    auto make_counter = [](std::string name) {
        // Returns a stateful closure
        return [name, count = 0]() mutable {
            ++count;
            return name + " called " + std::to_string(count) + "x";
            };
        };

    auto page_counter = make_counter("page");
    auto click_counter = make_counter("click");

    // Each closure maintains its own independent state
    std::cout << "  " << page_counter() << std::endl;
    std::cout << "  " << page_counter() << std::endl;
    std::cout << "  " << click_counter() << std::endl;
    std::cout << "  " << page_counter() << std::endl;  // Page is at 3
    std::cout << "  " << click_counter() << std::endl; // Click is at 2

    // A closure that acts as an accumulator (bank account pattern)
    auto make_account = [](double initial) {
        return [balance = initial](double amount) mutable {
            balance += amount;
            return balance;
            };
        };

    auto savings = make_account(1000.0);
    std::cout << "\n  Account after deposit: " << savings(500.0) << std::endl;
    std::cout << "  Account after withdrawal: " << savings(-200.0) << std::endl;
    std::cout << "  Account after interest: " << savings(35.0) << std::endl;
}

void ClosuresInPractice() {
    std::cout << "\n=== Real-World Closure Patterns ===" << std::endl;

    // PATTERN 1: Validation with configuration
    auto make_validator = [](int min, int max) {
        return [min, max](int value) -> std::string {
            if(value < min) return "Too low (min: " + std::to_string(min) + ")";
            if(value > max) return "Too high (max: " + std::to_string(max) + ")";
            return "Valid";
            };
        };

    auto age_validator = make_validator(18, 65);
    auto score_validator = make_validator(0, 100);

    std::cout << "  Age 25: " << age_validator(25) << std::endl;
    std::cout << "  Age 10: " << age_validator(10) << std::endl;
    std::cout << "  Score 150: " << score_validator(150) << std::endl;

    // PATTERN 2: Memoized (cached) computation
    auto make_cached = [](auto compute) {
        return [compute, cached = 0, has_result = false]() mutable {
            if(has_result) return cached;
            cached = compute();
            has_result = true;
            return cached;
            };
        };

    int computation_count = 0;
    auto expensive = [&computation_count]() {
        ++computation_count;
        return 42;  // Pretend this is expensive
        };

    auto cached_version = make_cached(expensive);
    std::cout << "\n  First call: " << cached_version() << std::endl;
    std::cout << "  Second call: " << cached_version() << " (cached!)" << std::endl;
    std::cout << "  Computation count: " << computation_count << " (only 1!)" << std::endl;
}