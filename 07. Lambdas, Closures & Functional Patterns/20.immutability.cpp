import std;
/*
   IMMUTABILITY IN C++ - FUNCTIONAL STYLE
 
   WHAT IS IMMUTABILITY?
       Data that cannot be changed after creation.
       Instead of modifying, you create NEW values.
 
   FUNCTIONAL STYLE:
       Instead of:
         for (auto& x : vec) x *= 2;  // Mutate in place
       Do:
         auto result = transform(vec, double_it);  // Create new
  
   WHY IMMUTABILITY?
   1. Easier reasoning (data doesn't change behind your back)
   2. Thread-safety (immutable data needs no locks)
   3. Undo/redo (just keep old versions)
   4. Debugging (data has a history)
  
   THE C++ BALANCE:
   C++ is not a pure functional language. We can't make everything
   immutable without performance costs. But we can:
     - Prefer const
     - Return new values instead of mutating
     - Use value semantics where practical
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "functional_header.h"

void ConstCorrectness() {
    std::cout << "=== const as Lightweight Immutability ===" << std::endl;

    /*
     * THE SIMPLEST FORM: const
     * ========================
     * const promises (enforced by compiler) that data won't change.
     */

    const int immutable = 42;
    // immutable = 100;  // ERROR: compiler enforces immutability

    std::cout << "  immutable = " << immutable << std::endl;

    // CONST PARAMETERS: functions that don't modify their inputs
    // This is a CONTRACT - "I promise not to change this"
    auto process = [](const std::vector<int>& v) {
        // v.clear();  // ERROR: can't modify const reference
        // v.push_back(1);  // ERROR

        // Can only READ:
        int sum = 0;
        for(int x : v) sum += x;
        return sum;
        };

    std::vector<int> data = {1, 2, 3, 4, 5};
    std::cout << "  Sum (data unchanged): " << process(data) << std::endl;
    std::cout << "  data.size() still = " << data.size() << " (not modified)" << std::endl;
}

void PureFunctions() {
    std::cout << "\n=== Pure Functions ===" << std::endl;

    /*
     * PURE FUNCTIONS: The gold standard
     * =================================
     * A pure function:
     *   1. Always returns the same output for the same input
     *   2. Has NO side effects (doesn't modify anything outside)
     *
     * Pure functions are:
     *   - Easy to test
     *   - Easy to reason about
     *   - Cacheable (memoizable)
     *   - Parallelizable (no shared state)
     */

     // PURE: only uses inputs, no side effects
    auto pure_add = [](int a, int b) {
        return a + b;  // Always same result for same inputs
        };

    // PURE: works on copy, returns new value
    auto pure_double_all = [](std::vector<int> v) {
        // v is a COPY - modifying it doesn't affect the caller's vector
        for(int& x : v) x *= 2;
        return v;  // Return the modified COPY
        };

    std::vector<int> original = {1, 2, 3};
    auto doubled = pure_double_all(original);

    std::cout << "  Original (unchanged): ";
    for(int x : original) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "  Doubled (new vector): ";
    for(int x : doubled) std::cout << x << " ";
    std::cout << std::endl;

    // IMPURE (for contrast): modifies the input
    auto impure_double_all = [](std::vector<int>& v) {
        for(int& x : v) x *= 2;  // Modifies caller's vector!
        };

    std::vector<int> will_be_modified = {1, 2, 3};
    impure_double_all(will_be_modified);
    std::cout << "  After impure call: ";
    for(int x : will_be_modified) std::cout << x << " ";
    std::cout << " (was modified!)" << std::endl;
}

void ValueSemantics() {
    std::cout << "\n=== Value Semantics ===" << std::endl;

    /*
     * VALUE SEMANTICS: Objects that behave like values
     * ================================================================
     * When you copy, you get an independent copy.
     * Modifying one doesn't affect the other.
     *
     * This is the DEFAULT for int, double, etc.
     * std::vector, std::string also have value semantics.
     */

    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = a;  // COPY - b is independent

    b.push_back(4);  // Only affects b
    b[0] = 100;      // Only affects b

    std::cout << "  a (original): ";
    for(int x : a) std::cout << x << " ";
    std::cout << " <- unchanged" << std::endl;

    std::cout << "  b (modified): ";
    for(int x : b) std::cout << x << " ";
    std::cout << " <- independent" << std::endl;

    /*
     * WHY THIS MATTERS:
     * =================
     * With value semantics, you can reason about code locally.
     * You don't need to worry about "who else has a reference to this?"
     */
}

void ImmutabilityTradeoffs() {
    std::cout << "\n=== Trade-offs of Immutability ===" << std::endl;

    std::cout << R"(
    PROS OF IMMUTABILITY:
    ┌─────────────────────────────────────────────────────┐
    │ • Thread-safe without locks                        │
    │ • Easy to reason about                             │
    │ • Great for debugging (data doesn't change)        │
    │ • Enables time-travel debugging                    │
    │ • Free undo/redo (keep old versions)               │
    │ • Safe to share                                    │
    └─────────────────────────────────────────────────────┘

    CONS OF IMMUTABILITY:
    ┌─────────────────────────────────────────────────────┐
    │ • More memory (copies everywhere)                   │
    │ • Slower (allocation + copying)                     │
    │ • Not natural in C++ (designed for mutation)       │
    │ • Can be awkward for in-place algorithms           │
    └─────────────────────────────────────────────────────┘

    C++ PRAGMATIC APPROACH:
    ┌─────────────────────────────────────────────────────┐
    │ • Use const liberally                              │
    │ • Prefer pure functions when practical             │
    │ • Use value semantics for small types              │
    │ • Allow mutation for performance-critical code     │
    │ • Document when functions mutate their inputs      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}