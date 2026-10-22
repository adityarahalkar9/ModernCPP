import std;
/*
 * LAZY EVALUATION - COMPUTING ONLY WHEN NEEDED
 * 
 * WHAT IS LAZY EVALUATION?
 * ========================
 * Deferring computation until the result is actually needed.
 * This can save enormous amounts of work if the result is never used,
 * or if only part of the result is needed.
 *
 * TWO LEVELS OF LAZINESS:
 * =======================
 * 1. DEFERRED CALL: Don't call the function until needed
 * 2. INFINITE STRUCTURES: Generate values on-demand (C++20 views)

 * BENEFITS:
 * - Avoid unnecessary computation
 * - Work with "infinite" sequences
 * - Compose operations without intermediate storage
 * - Better composability
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "functional_header.h"

void DeferredEvaluation() {
    std::cout << "=== Deferred Evaluation ===" << std::endl;

    /*
     * PATTERN 1: The "thunk" - a deferred computation
     * ================================================================
     * Instead of computing now, store HOW to compute later.
     */

    int computation_count = 0;

    // EAGER: computed immediately
    auto eager = [&computation_count]() {
        ++computation_count;
        return 42;  // Pretend this is expensive
        }();
    // ^ The lambda runs RIGHT NOW

    std::cout << "  Eager value: " << eager << " (computations so far: "
        << computation_count << ")" << std::endl;

    // LAZY: just store the function, don't run it
    auto lazy = [&computation_count]() {
        ++computation_count;
        return 42;  // Same computation, but NOT run yet
        };
    // ^ Nothing has happened yet!

    std::cout << "  After storing lazy: " << computation_count << " computations" << std::endl;

    // NOW call it:
    int result = lazy();
    std::cout << "  After calling lazy: " << result << " (computations: "
        << computation_count << ")" << std::endl;

    /*
     * USE CASE: Only compute if needed
     */
    bool need_result = false;

    auto maybe_compute = [need_result, &computation_count]() -> int {
        if(!need_result) {
            std::cout << "    (Skipping computation - not needed)" << std::endl;
            return 0;
        }
        ++computation_count;
        return 100;  // Expensive computation
        };

    int value = maybe_compute();  // Skips the expensive part
    std::cout << "  Value (skipped): " << value << std::endl;
}

void MemoizationPattern() {
    std::cout << "\n=== Memoization: Lazy + Caching ===" << std::endl;

    /*
     * MEMOIZATION:
     * ============
     * Lazy evaluation + caching = compute once, remember forever.
     * Perfect for expensive pure functions (same input → same output).
     */

    int call_count = 0;

    // A memoized function - only computes the first time
    class Memoized {
        std::function<int()> compute_;
        mutable int cached_ = 0;
        mutable bool has_cached_ = false;

    public:
        explicit Memoized(std::function<int()> f) : compute_(std::move(f)) {}

        int operator()() const {
            if(!has_cached_) {
                cached_ = compute_();
                has_cached_ = true;
            }
            return cached_;
        }
    };

    Memoized expensive_result([&call_count]() {
        ++call_count;
        std::cout << "    [Computing... (this should appear ONCE)]" << std::endl;
        return 100;  // Pretend this is expensive
                              });

    // First call - computes
    std::cout << "  First call: " << expensive_result() << std::endl;
    // Second call - cached
    std::cout << "  Second call: " << expensive_result() << std::endl;
    // Third call - still cached
    std::cout << "  Third call: " << expensive_result() << std::endl;

    std::cout << "  Total computations: " << call_count << " (should be 1)" << std::endl;
}

void InfiniteSequences() {
    std::cout << "\n=== Infinite Sequences (Concept) ===" << std::endl;

    /*
     * LAZY GENERATORS:
     * =================
     * With lazy evaluation, you can work with "infinite" sequences.
     * Values are only generated as needed.
     */

     // A "generator" that produces values on demand
    auto make_counter = [start = 0]() mutable {
        return [current = start]() mutable {
            return current++;
            };
        };

    // Create a generator starting at 1
    auto next = make_counter();
    next();  // Skip 0

    // Take only the first 5 values (don't generate infinitely!)
    std::cout << "  First 5 values: ";
    for(int i = 0; i < 5; ++i) {
        std::cout << next() << " ";
    }
    std::cout << std::endl;

    /*
     * C++20 views make this elegant:
     * ==============================
     * auto evens = std::views::iota(0)              // 0, 1, 2, 3, ... (infinite)
     *              | std::views::filter(even);       // 0, 2, 4, ... (infinite)
     *
     * // Only take 5:
     * auto first5 = evens | std::views::take(5);    // 0, 2, 4, 6, 8 (finite!)
     */
}

void ShortCircuitEvaluation() {
    std::cout << "\n=== Short-Circuit Patterns ===" << std::endl;

    /*
     * SHORT-CIRCUIT: Stop as soon as you have the answer
     * ================================================================
     */

    std::vector<int> numbers = {1, 3, 5, 8, 9, 12, 15, 18, 21};

    // FIND FIRST: stops at first match (doesn't scan the rest)
    auto first_even = std::find_if(numbers.begin(), numbers.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "  First even: " << *first_even << " (stopped early!)" << std::endl;

    // ANY_OF: stops as soon as one matches
    bool has_even = std::any_of(numbers.begin(), numbers.end(),
                                [](int n) {
                                    std::cout << "    checking " << n << "...";
                                    return n % 2 == 0;
                                });
    std::cout << "  has_even: " << has_even << std::endl;

    // NONE_OF: stops as soon as one matches (returns false)
    std::cout << "  Checking for negatives (stops early): ";
    bool no_negatives = std::none_of(numbers.begin(), numbers.end(),
                                     [](int n) {
                                         std::cout << n << " ";
                                         return n < 0;
                                     });
    std::cout << "-> " << no_negatives << std::endl;

    /*
     * THE LESSON:
     * ===========
     * These algorithms don't do MORE work than necessary.
     * They're not just convenient - they're efficient.
     */
}