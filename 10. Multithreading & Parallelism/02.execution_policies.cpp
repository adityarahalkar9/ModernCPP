/*
 * ============================================================================
 * EXECUTION POLICIES - CONTROLLING HOW ALGORITHMS RUN
 * ============================================================================
 *
 * THE FOUR POLICIES (C++17 + C++20):
 * ==================================
 *
 * 1. seq (sequential):
 *    - Single-threaded, exactly like before C++17
 *    - Deterministic order
 *    - Use for: small datasets, debugging
 *
 * 2. par (parallel):
 *    - Multi-threaded (uses multiple cores)
 *    - Order within elements preserved per-element
 *    - Use for: large datasets, CPU-bound work
 *
 * 3. par_unseq (parallel + vectorized) [C++17]:
 *    - Multi-threaded AND may use SIMD/vector instructions
 *    - Elements may be processed out of order
 *    - Use for: maximum performance on numeric code
 *
 * 4. unseq (vectorized only) [C++20]:
 *    - Single-threaded but may use SIMD
 *    - Use for: small data that fits in vector registers
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "parallelism_header.h"

void AllPoliciesDemo() {
    std::cout << "=== Execution Policies Explained ===" << std::endl;

    std::vector<double> data(1'000'000);
    std::iota(data.begin(), data.end(), 1.0);

    // 1. SEQUENTIAL (the old default):
    {
        auto start = std::chrono::steady_clock::now();
        std::for_each(data.begin(), data.end(),
                      [](double& x) { x = std::sqrt(x); });
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  seq:      " << ms << "μs (single-threaded)" << std::endl;
    }

    // 2. PARALLEL:
    {
        std::vector<double> copy = data;
        auto start = std::chrono::steady_clock::now();
        std::for_each(std::execution::par, copy.begin(), copy.end(),
                      [](double& x) { x = std::sqrt(x); });
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  par:      " << ms << "μs (multi-threaded)" << std::endl;
    }

    // 3. PARALLEL + UNSEQUENCED (may vectorize):
    {
        std::vector<double> copy = data;
        auto start = std::chrono::steady_clock::now();
        std::for_each(std::execution::par_unseq, copy.begin(), copy.end(),
                      [](double& x) { x = std::sqrt(x); });
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  par_unseq: " << ms << "μs (parallel + SIMD)" << std::endl;
    }

    std::cout << R"(
    VISUALIZING THE POLICIES:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ seq:                                                │
    │   Core 1: [1][2][3][4][5][6][7][8]                  │
    │   (one core does everything, in order)              │
    │                                                     │
    │ par:                                                │
    │   Core 1: [1][2][3][4]                              │
    │   Core 2:   [5][6][7][8]                            │
    │   (split work across cores)                         │
    │                                                     │
    │ par_unseq:                                          │
    │   Core 1: [1,2][3,4]  ← SIMD processes pairs       │
    │   Core 2:   [5,6][7,8]     simultaneously          │
    │   (multiple cores + vector instructions)            │
    └─────────────────────────────────────────────────────┘

    WHEN TO USE EACH:
    ┌─────────────────────────────────────────────────────┐
    │ seq:       Small data (<1000 elements), debugging   │
    │ par:       Large data, CPU-bound work               │
    │ par_unseq: Numeric heavy lifting, SIMD-friendly     │
    │                                                     │
    │ START WITH par - it's the sweet spot for most code │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void PolicySafetyRules() {
    std::cout << "\n=== Safety Rules for Parallel Algorithms ===" << std::endl;

    /*
     * RULE 1: Element access functions must not cause data races
     * ================================================================
     */

     // SAFE: Each thread touches different elements
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(std::execution::par, v.begin(), v.end(),
                  [](int& x) { ++x; });  // OK: each element modified by one thread

    // UNSAFE: Writing to a shared variable from multiple threads
    // int counter = 0;
    // std::for_each(std::execution::par, v.begin(), v.end(),
    //               [&counter](int) { ++counter; });  // DATA RACE!

    std::cout << R"(
    THE GOLDEN RULES:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Element access functions must not data race      │
    │    Each element should be touched by ONE thread     │
    │                                                     │
    │ 2. For unseq/par_unseq:                             │
    │    NO dependencies between element accesses        │
    │    (can't read element i and write element i+1)     │
    │                                                     │
    │ 3. Exceptions:                                       │
    │    If element access throws → std::terminate()!    │
    │    Keep your lambdas exception-free                │
    │                                                     │
    │ 4. Don't hold locks in element functions:           │
    │    It serializes your "parallel" code               │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}