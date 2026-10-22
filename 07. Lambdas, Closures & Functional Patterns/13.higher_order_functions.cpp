import std;
/*
 * ============================================================================
 * HIGHER-ORDER FUNCTIONS - FUNCTIONS THAT TAKE/RETURN FUNCTIONS
 * ============================================================================
 *
 * WHAT IS A HIGHER-ORDER FUNCTION?
 * ================================
 * A higher-order function (HOF) is a function that either:
 *   1. TAKES one or more functions as arguments, OR
 *   2. RETURNS a function as its result
 *
 * This is a core concept from functional programming.
 * If you've used std::transform, std::accumulate, or std::sort
 * with a comparator - you've already used HOFs!
 *
 * WHY ARE THEY USEFUL?
 * ====================
 * 1. ABSTRACTION: Separate WHAT to do from HOW to do it
 * 2. COMPOSABILITY: Build complex behavior from simple pieces
 * 3. REUSE: Write the pattern once, use with many operations
 *
 * CLASSIC HOFs FROM FUNCTIONAL PROGRAMMING:
 * =========================================
 * - map:    apply function to each element
 * - filter: keep elements matching a predicate
 * - reduce/fold: combine elements into one value
 * - compose: combine two functions into one
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "functional_header.h"

void TakingFunctions() {
    std::cout << "=== Functions That Take Functions ===" << std::endl;

    /*
     * HOF CATEGORY 1: Taking functions as parameters
     * ==============================================
     * These let you customize behavior at the call site.
     * The HOF defines the STRUCTURE, you provide the OPERATION.
     */

     // A generic "apply 3 times" HOF
     // It takes a function, applies it three times
    auto thrice = [](auto func, auto value) {
        value = func(value);
        value = func(value);
        value = func(value);
        return value;
        };

    // Use it with different operations:
    auto result1 = thrice([](int x) { return x + 10; }, 0);   // 0→10→20→30
    auto result2 = thrice([](int x) { return x * 2; }, 1);    // 1→2→4→8
    auto result3 = thrice([](std::string s) { return s + "!"; }, std::string("wow"));

    std::cout << "  thrice(add10, 0) = " << result1 << std::endl;
    std::cout << "  thrice(double, 1) = " << result2 << std::endl;
    std::cout << "  thrice(exclaim, \"wow\") = " << result3 << std::endl;

    /*
     * THE POWER:
     * ==========
     * We wrote the "apply 3 times" logic ONCE.
     * It works with ANY operation. That's abstraction!
     */
}

void ReturningFunctions() {
    std::cout << "\n=== Functions That Return Functions ===" << std::endl;

    /*
     * HOF CATEGORY 2: Returning functions
     * ===================================
     * These are "function factories" - they create customized functions.
     */

     // Factory: creates a "clamp" function with specific bounds
    auto make_clamp = [](int lo, int hi) {
        return [lo, hi](int value) {
            return std::clamp(value, lo, hi);
            };
        };

    // Create specialized clamp functions:
    auto clamp_0_100 = make_clamp(0, 100);
    auto clamp_1_10 = make_clamp(1, 10);

    std::cout << "  clamp_0_100(150) = " << clamp_0_100(150) << std::endl;
    std::cout << "  clamp_1_10(5) = " << clamp_1_10(5) << std::endl;
    std::cout << "  clamp_1_10(-5) = " << clamp_1_10(-5) << std::endl;
}

void TheBigThree() {
    std::cout << "\n=== Map, Filter, Reduce ===" << std::endl;

    /*
     * THE HOLY TRINITY OF FUNCTIONAL PROGRAMMING
     * ==========================================
     * These three HOFs cover most data transformation needs.
     */

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 1. MAP: Transform each element
    // C++ equivalent: std::transform (or ranges in C++20)
    std::vector<int> doubled;
    std::transform(numbers.begin(), numbers.end(),
                   std::back_inserter(doubled),
                   [](int n) { return n * 2; });

    std::cout << "  Map (double): ";
    for(int n : doubled) std::cout << n << " ";
    std::cout << std::endl;

    // 2. FILTER: Keep elements matching a condition
    // C++ equivalent: std::copy_if
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });

    std::cout << "  Filter (evens): ";
    for(int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // 3. REDUCE/FOLD: Combine all elements into one value
    // C++ equivalent: std::accumulate
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0,
                              [](int acc, int n) { return acc + n; });

    int product = std::accumulate(numbers.begin(), numbers.end(), 1,
                                  [](int acc, int n) { return acc * n; });

    std::cout << "  Reduce (sum): " << sum << std::endl;
    std::cout << "  Reduce (product): " << product << std::endl;

    /*
     * C++20 NOTE:
     * ===========
     * C++20 ranges make these even more elegant:
     *   auto result = numbers | std::views::filter(even)
     *                         | std::views::transform(double_it);
     */
}

void ComposingOperations() {
    std::cout << "\n=== Composing Multiple Operations ===" << std::endl;

    /*
     * THE REAL POWER: CHAINING
     * ========================
     * HOFs become truly powerful when you CHAIN them together,
     * creating a pipeline of transformations.
     */

    std::vector<std::string> words = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon"
    };

    // TASK: Find total length of all words longer than 5 characters

    // Step 1: FILTER - keep only long words
    std::vector<std::string> long_words;
    std::copy_if(words.begin(), words.end(),
                 std::back_inserter(long_words),
                 [](const std::string& w) { return w.length() > 5; });

    // Step 2: MAP - convert to lengths
    std::vector<size_t> lengths;
    std::transform(long_words.begin(), long_words.end(),
                   std::back_inserter(lengths),
                   [](const std::string& w) { return w.length(); });

    // Step 3: REDUCE - sum the lengths
    size_t total = std::accumulate(lengths.begin(), lengths.end(), 0);

    std::cout << "  Words > 5 chars: ";
    for(const auto& w : long_words) std::cout << w << " ";
    std::cout << std::endl;
    std::cout << "  Total length: " << total << std::endl;
}