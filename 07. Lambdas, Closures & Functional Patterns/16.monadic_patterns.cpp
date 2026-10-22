import std;
/*
 * ============================================================================
 * MONADIC PATTERNS - CHAINING OPERATIONS THAT CAN FAIL
 * ============================================================================
 *
 * WHAT IS A MONAD? (SIMPLIFIED)
 * =============================
 * A monad is a design pattern for CHAINING operations while
 * automatically handling some "context" - like possible failure,
 * optional values, or multiple results.
 *
 * THE PROBLEM IT SOLVES:
 * ======================
 * Without monads, error handling creates deeply nested code:
 *
 *   Result1 r1 = step1();
 *   if (!r1.valid) return error;
 *   Result2 r2 = step2(r1);
 *   if (!r2.valid) return error;
 *   Result3 r3 = step3(r2);
 *   if (!r3.valid) return error;
 *   // ... pyramid of doom!
 *
 * WITH MONADIC CHAINING:
 * ======================
 *   auto result = step1()
 *       .and_then(step2)
 *       .and_then(step3);
 *   // Each step only runs if the previous succeeded!
 *
 * THE KEY OPERATIONS:
 * ===================
 * 1. map/transform: Apply function to value (if present)
 * 2. and_then/flatMap: Chain operations (each can fail)
 * 3. or_else: Provide alternative (if empty/failed)
 *
 * C++20/23 PROVIDES THESE FOR std::optional AND std::expected!
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <algorithm>
#include "functional_header.h"

 // Simulated operations that can fail
std::optional<int> parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    }
    catch(...) {
        return std::nullopt;  // "Failed" signal
    }
}

std::optional<int> divide(int a, int b) {
    if(b == 0) return std::nullopt;  // Can't divide by zero
    return a / b;
}

std::optional<int> sqrt_positive(int x) {
    if(x < 0) return std::nullopt;  // No real sqrt for negative
    return static_cast<int>(std::sqrt(x));
}

void ThePyramidProblem() {
    std::cout << "=== The Problem: Nested Error Handling ===" << std::endl;

    std::string input = "100";

    // WITHOUT monadic chaining - the "pyramid of doom":
    auto parsed = parse_int(input);
    if(parsed) {
        auto divided = divide(*parsed, 4);
        if(divided) {
            auto rooted = sqrt_positive(*divided);
            if(rooted) {
                std::cout << "  Result: " << *rooted << std::endl;
            }
            else {
                std::cout << "  sqrt failed" << std::endl;
            }
        }
        else {
            std::cout << "  divide failed" << std::endl;
        }
    }
    else {
        std::cout << "  parse failed" << std::endl;
    }

    /*
     * THE PROBLEM:
     * ============
     * 1. Deeply nested - hard to read
     * 2. Error handling drowns out the logic
     * 3. Each new step adds another nesting level
     */
}

void MonadicChaining() {
    std::cout << "\n=== Monadic Chaining (C++23) ===" << std::endl;

    /*
     * THE MONADIC SOLUTION:
     * =====================
     * C++23 adds transform, and_then, or_else to std::optional
     * (and std::expected).
     *
     * Each operation only runs if the previous one succeeded!
     */

    std::string input = "100";

    // CHAINED VERSION - flat and readable:
    auto result = parse_int(input)              // Step 1: parse
        .and_then([](int x) { return divide(x, 4); })       // Step 2: divide
        .and_then([](int x) { return sqrt_positive(x); });  // Step 3: sqrt

    // Check if the whole chain succeeded:
    if(result) {
        std::cout << "  Chained result: " << *result << std::endl;
    }
    else {
        std::cout << "  Chain failed somewhere" << std::endl;
    }

    /*
     * HOW IT WORKS:
     * =============
     * - parse_int returns optional<int>
     * - If it has a value, and_then calls the lambda with that value
     * - The lambda returns another optional
     * - If THAT has a value, the next and_then runs
     * - If ANY step fails (returns nullopt), the chain short-circuits
     *   and ALL subsequent steps are skipped
     */
}

void TransformVsAndThen() {
    std::cout << "\n=== transform vs and_then ===" << std::endl;

    /*
     * THE TWO KEY OPERATIONS:
     * =======================
     *
     * transform (like map):
     *   - Takes a function: T -> U
     *   - Applies it to the contained value
     *   - Returns optional<U>
     *   - The function CANNOT fail
     *
     * and_then (like flatMap):
     *   - Takes a function: T -> optional<U>
     *   - Applies it to the contained value
     *   - Returns optional<U> (from the function)
     *   - The function CAN fail (return nullopt)
     */

    std::optional<int> value = 42;

    // TRANSFORM: function returns a plain value
    auto doubled = value.transform([](int x) { return x * 2; });
    std::cout << "  transform (double): " << *doubled << std::endl;

    // AND_THEN: function returns an optional
    auto safe_div = value.and_then([](int x) { return divide(x, 2); });
    std::cout << "  and_then (divide): " << *safe_div << std::endl;

    // Failure propagation:
    std::optional<int> empty;  // No value

    auto transformed_empty = empty.transform([](int x) { return x * 2; });
    std::cout << "  transform on empty: " << transformed_empty.has_value() << " (skipped)" << std::endl;

    auto and_then_empty = empty.and_then([](int x) { return divide(x, 2); });
    std::cout << "  and_then on empty: " << and_then_empty.has_value() << " (skipped)" << std::endl;

    // FAILURE SHORT-CIRCUITS:
    std::optional<int> failure = std::nullopt;
    auto result = failure
        .transform([](int x) { std::cout << "  This never runs!"; return x; })
        .and_then([](int x) { return divide(x, 2); });
    std::cout << "  Chain with failure: " << result.has_value() << " (all skipped)" << std::endl;
}

void OrElsePattern() {
    std::cout << "\n=== or_else: Providing Alternatives ===" << std::endl;

    /*
     * THE or_else OPERATION:
     * =====================
     * Provides a fallback value if the optional is empty.
     * This completes the "maybe" pattern.
     */

    std::optional<std::string> maybe_name;

    // If empty, use default:
    std::string name = maybe_name.value_or("Anonymous");
    std::cout << "  Default name: " << name << std::endl;

    // Chaining with fallback at the end:
    std::string user_input = "not_a_number";

    auto result = parse_int(user_input)
        .transform([](int x) { return x * 2; })
        .and_then([](int x) { return divide(x, 1); })
        .or_else([]() { return std::optional<int>(-1); });  // Fallback

    std::cout << "  Result with fallback: " << *result << std::endl;

    /*
     * FULL PATTERN:
     * =============
     * operation
     *   .transform(step1)      // Must succeed
     *   .and_then(step2)       // Can fail
     *   .transform(step3)      // Must succeed
     *   .or_else(fallback);    // Default if anything failed
     */
}