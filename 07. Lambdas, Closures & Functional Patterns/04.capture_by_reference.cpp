import std;

/*
 * ============================================================================
 * CAPTURE BY REFERENCE - THE LIVE CONNECTION
 * ============================================================================
 *
 * WHAT HAPPENS WHEN YOU CAPTURE BY REFERENCE?
 * ===========================================
 * When you write [&x] or [&], the compiler:
 *   1. Stores a POINTER/REFERENCE to the original variable in the lambda
 *   2. When the lambda uses 'x', it's accessing the ORIGINAL memory
 *   3. Changes to the original are SEEN by the lambda (and vice versa)
 *
 * THE "WALKIE-TALKIE" METAPHOR:
 * ============================
 * By-value capture = a PHOTOGRAPH (frozen snapshot)
 * By-reference capture = a LIVE VIDEO FEED (always current)
 *
 * THE DANGER: DANGLING REFERENCES
 * ===============================
 * If the captured variable is DESTROYED before the lambda,
 * the lambda has a reference to dead memory. Using it is
 * UNDEFINED BEHAVIOR (crashes, garbage data, security holes).
 *
 * WHEN TO USE BY-REFERENCE:
 * =========================
 * + When the lambda is used IMMEDIATELY (same scope, synchronously)
 * + When you need to MODIFY the outside variable
 * + When you need the CURRENT value (not a snapshot)
 * + When copying would be too expensive
 *
 * WHEN TO AVOID BY-REFERENCE:
 * ===========================
 * - Lambda stored for later (variable might be gone)
 * - Lambda passed to another thread (race conditions, lifetime issues)
 * - When the variable's lifetime is shorter than the lambda's
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include "functional_header.h"

void ReferenceCaptureBasics() {
    std::cout << "=== By-Reference Capture ===" << std::endl;

    int value = 10;

    // [&value] = "capture a live connection to 'value'"
    // NOT a copy - the lambda and outside code share the SAME variable
    auto get_current = [&value]() {
        return value;  // Always reads the CURRENT value
        };

    auto modify = [&value]() {
        value *= 2;  // Changes the ACTUAL outside variable
        };

    std::cout << "  Initial value: " << value << std::endl;
    std::cout << "  get_current() = " << get_current() << std::endl;

    modify();  // The lambda changes the outside variable!
    std::cout << "  After modify(): value = " << value << " (changed by lambda!)" << std::endl;

    // Outside changes are seen by the lambda
    value = 1000;
    std::cout << "  After outside change: get_current() = " << get_current() << std::endl;

    /*
     * UNDER THE HOOD:
     * ===============
     * The generated class looks like:
     *
     *   class LambdaType {
     *       int& value;  // <- REFERENCE member, not a copy
     *   public:
     *       void operator()() { value *= 2; }  // Modifies through reference
     *   };
     */
}

void ReferenceForAccumulation() {
    std::cout << "\n=== Pattern: Reference for Accumulation ===" << std::endl;

    /*
     * THE CLASSIC USE CASE: Using [&] to accumulate results
     * ================================================================
     * This is the #1 reason to use reference captures.
     * The lambda acts as a "writer" to an outside variable,
     * building up a result across multiple calls or iterations.
     */

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // We want to compute several things in ONE pass through the data
    // (efficient!) using reference captures to collect results
    int sum = 0;
    int product = 1;
    int count_evens = 0;
    int max_val = numbers[0];

    // Each lambda "writes to" an outside variable via reference
    auto adder = [&sum](int n) { sum += n; };
    auto multiplier = [&product](int n) { product *= n; };
    auto even_counter = [&count_evens](int n) { if(n % 2 == 0) ++count_evens; };
    auto max_tracker = [&max_val](int n) { if(n > max_val) max_val = n; };

    // Single pass through data - all four lambdas update their targets
    for(int n : numbers) {
        adder(n);
        multiplier(n);
        even_counter(n);
        max_tracker(n);
    }

    std::cout << "  Sum: " << sum << std::endl;
    std::cout << "  Product: " << product << std::endl;
    std::cout << "  Even count: " << count_evens << std::endl;
    std::cout << "  Max: " << max_val << std::endl;
}

void TheDanglingDanger() {
    std::cout << "\n=== THE DANGER: Dangling References ===" << std::endl;

    /*
     * CRITICAL CONCEPT: THE DANGLING REFERENCE PROBLEM
     * ================================================
     * This is the #1 pitfall of [&] capture.
     *
     * SCENARIO:
     * 1. Create a lambda that captures a local variable by reference
     * 2. Return the lambda (or store it somewhere that outlives the scope)
     * 3. The local variable is DESTROYED when its scope ends
     * 4. The lambda now holds a reference to DEAD memory
     * 5. Calling the lambda = UNDEFINED BEHAVIOR
     */

     // SAFE: Lambda used in the same scope - reference is valid
    {
        int safe_local = 42;
        auto safe_lambda = [&safe_local]() {
            return safe_local * 2;  // OK: safe_local is alive here
            };
        std::cout << "  Safe (same scope): " << safe_lambda() << std::endl;
    }  // safe_local destroyed here, but so is the lambda - no problem

    // DANGEROUS: Lambda outlives the captured variable
    // This code is SHOWN but not executed to avoid actual UB:
    std::cout << "  (Demonstrating the DANGER pattern - not executed:)" << std::endl;
    std::cout << R"(
    // THIS WOULD BE UNDEFINED BEHAVIOR:
    auto make_dangling = []() {
        int local = 42;
        return [&local]() {     // Captures reference to 'local'
            return local * 2;    // 'local' will be dead when this runs!
        };
        // 'local' destroyed here - lambda now has dangling reference
    };
    auto f = make_dangling();
    auto result = f();  // UB! Accessing dead memory
)" << std::endl;

    /*
     * HOW TO AVOID THIS:
     * =================
     * 1. If the lambda needs to outlive the variable → capture BY VALUE
     * 2. Use smart pointers (shared_ptr) for shared ownership
     * 3. Keep the lambda's lifetime within the variable's lifetime
     * 4. For classes, use std::enable_shared_from_this
     */
}

void ReferenceVsValueDecision() {
    std::cout << "\n=== Decision Guide: Reference vs Value ===" << std::endl;

    std::cout << R"(
    WHEN TO USE [x] (BY VALUE):
    ┌─────────────────────────────────────────────────────┐
    │ • Lambda might outlive the variable                 │
    │ • You want a snapshot (value won't change)          │
    │ • Capturing small types (int, pointers, etc.)       │
    │ • Thread safety (each lambda has own copy)          │
    │ • You DON'T need to modify the original             │
    └─────────────────────────────────────────────────────┘

    WHEN TO USE [&x] (BY REFERENCE):
    ┌─────────────────────────────────────────────────────┐
    │ • Lambda is used immediately in same scope          │
    │ • You NEED to modify the original variable          │
    │ • You need the CURRENT value (not snapshot)         │
    │ • The object is too large to copy                   │
    │ • Accumulating results from iterations              │
    └─────────────────────────────────────────────────────┘

    GOLDEN RULE:
    "If in doubt, capture by value.
     It's safer (no dangling) at the cost of a copy.
     Only use references when you KNOW the lifetime is safe."
)" << std::endl;
}