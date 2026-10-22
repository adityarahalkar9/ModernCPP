/*
 * ============================================================================
 * PARALLEL REDUCE - COMBINING ELEMENTS
 * ============================================================================
 *
 * WHAT IS REDUCE?
 * ===============
 * Combine all elements into a single value:
 *   result = op(op(op(a[0], a[1]), a[2]), ...)
 *
 * EXAMPLES: sum, product, max, min, count
 *
 * THE PARALLEL CHALLENGE:
 * =======================
 * Sequential: (a+b)+c == a+(b+c) — order doesn't matter for +.
 * But some operations DO depend on order (subtraction, string concat).
 *
 * SAFE operations: +, *, max, min, AND, OR, XOR
 * UNSAFE: -, /, string concatenation
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
#include <algorithm>
#include <chrono>
#include "parallelism_header.h"

void ParallelReduceDemo() {
    std::cout << "=== Parallel Reduction ===" << std::endl;

    std::vector<long long> data(100'000'000);
    std::iota(data.begin(), data.end(), 1LL);

    // SEQUENTIAL:
    auto start = std::chrono::steady_clock::now();
    long long seq_result = std::accumulate(data.begin(), data.end(), 0LL);
    auto end = std::chrono::steady_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // PARALLEL (C++17):
    start = std::chrono::steady_clock::now();
    long long par_result = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    end = std::chrono::steady_clock::now();
    auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Sum 1..100M:" << std::endl;
    std::cout << "  Sequential: " << seq_result << " (" << seq_ms << "ms)" << std::endl;
    std::cout << "  Parallel:   " << par_result << " (" << par_ms << "ms)" << std::endl;
    std::cout << "  Speedup: " << (seq_ms / static_cast<double>(par_ms)) << "x" << std::endl;
}

void TreeReduction() {
    std::cout << "\n=== Tree Reduction (Manual Implementation) ===" << std::endl;

    /*
     * HOW PARALLEL REDUCTION WORKS INTERNALLY:
     * ================================================================
     */

    auto tree_reduce = [](const std::vector<int>& data, auto op, int identity) {
        // Recursive split until small enough
        std::function<int(size_t, size_t)> reduce_range =
            [&](size_t begin, size_t end) -> int {
            if(end - begin <= 1000) {
                // Base case: sequential
                int result = identity;
                for(size_t i = begin; i < end; ++i) {
                    result = op(result, data[i]);
                }
                return result;
            }

            size_t mid = (begin + end) / 2;

            // Fork: compute right half in parallel
            auto right_future = std::async(std::launch::async,
                                           [&reduce_range, mid, end]() {
                                               return reduce_range(mid, end);
                                           });

            // Compute left half in current thread
            int left = reduce_range(begin, mid);

            // Join
            int right = right_future.get();

            // Combine
            return op(left, right);
            };

        return reduce_range(0, data.size());
        };

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);

    // Parallel MAX using tree reduction:
    int max_val = tree_reduce(data,
                              [](int a, int b) { return std::max(a, b); },
                              std::numeric_limits<int>::min());

    std::cout << "  Max of 1..1M: " << max_val << " (expected 1000000)" << std::endl;

    std::cout << R"(
    TREE REDUCTION VISUALIZATION:
    ┌─────────────────────────────────────────────────────┐
    │ Sequential:                                         │
    │   (((1+2)+3)+4)+5+...  ← one long chain             │
    │                                                     │
    │ Tree (parallel):                                    │
    │        [1,2,3,4,5,6,7,8]                            │
    │       /                    \                        │
    │   [1,2,3,4]            [5,6,7,8]                   │
    │   /       \            /       \                    │
    │ [1,2]   [3,4]      [5,6]     [7,8]                 │
    │   ↓       ↓          ↓         ↓                    │
    │   3   +   7    =    11    +   15                    │
    │        \                  /                         │
    │         10     +     26                             │
    │               \    /                                │
    │                36                                    │
    │                                                     │
    │ Depth is log₂(n) instead of n!                      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}