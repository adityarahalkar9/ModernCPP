/*
 * ============================================================================
 * FORK-JOIN PARALLELISM - DIVIDE AND CONQUER
 * ============================================================================
 *
 * WHAT IS FORK-JOIN?
 * ==================
 * A parallelism pattern where:
 * 1. FORK: Split work into subtasks, run them in parallel
 * 2. JOIN: Wait for all subtasks to complete
 * 3. Combine results
 *
 * THE RECURSIVE TREE:
 * ===================
 *
 *          [Full Problem]
 *         /              \
 *    [Half 1]          [Half 2]     ← Fork (split)
 *    /     \          /     \
 *  [Q1]   [Q2]     [Q3]   [Q4]      ← Fork again
 *   |      |        |      |
 *   └──────┴────────┴──────┘         ← Join (combine)
 *
 * USED FOR:
 * =========
 * - Parallel sorting (merge sort)
 * - Parallel reduction (sum, max)
 * - Parallel scanning
 * - Any divide-and-conquer algorithm
 *
 * ============================================================================
 */


import std;
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include "parallelism_header.h"

void ForkJoinBasics() {
    std::cout << "=== Fork-Join Pattern ===" << std::endl;

    /*
     * MANUAL FORK-JOIN WITH FUTURES:
     * ================================================================
     */

    auto parallel_sum = [](std::vector<int>::iterator begin,
                           std::vector<int>::iterator end) -> long long {
                               auto size = std::distance(begin, end);

                               // BASE CASE: Small enough, do it sequentially
                               if(size < 10000) {
                                   return std::accumulate(begin, end, 0LL);
                               }

                               // FORK: Split into two halves
                               auto mid = begin + size / 2;

                               // Launch first half in another thread:
                               auto handle = std::async(std::launch::async,
                                                        [mid, end]() {
                                                            return std::accumulate(mid, end, 0LL);
                                                        });

                               // Compute second half in THIS thread (don't waste the current thread!)
                               long long first_half = std::accumulate(begin, mid, 0LL);

                               // JOIN: Wait for the other half
                               long long second_half = handle.get();

                               // COMBINE:
                               return first_half + second_half;
        };

    std::vector<int> data(10'000'000);
    std::iota(data.begin(), data.end(), 1);

    auto start = std::chrono::steady_clock::now();
    long long sum = parallel_sum(data.begin(), data.end());
    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  Sum of 1..10,000,000 = " << sum << std::endl;
    std::cout << "  (Expected: " << (10'000'000LL * 10'000'001 / 2) << ")" << std::endl;
    std::cout << "  Time: " << ms << "ms" << std::endl;

    std::cout << R"(
    THE RECURSION TREE (for 8M elements):
    ┌─────────────────────────────────────────────────────┐
    │ Level 0: [—————————— 8M ——————————]                  │
    │           split into 2×4M                          │
    │                                                     │
    │ Level 1: [———— 4M ————] [———— 4M ————]              │
    │           split into 4×2M                          │
    │                                                     │
    │ Level 2: [—2M—] [—2M—] [—2M—] [—2M—]               │
    │           split into 8×1M                          │
    │                                                     │
    │ Level 3: ... until < 10000 (base case)             │
    │           then sequential accumulate                │
    └─────────────────────────────────────────────────────┘

    KEY INSIGHT:
    ┌─────────────────────────────────────────────────────┐
    │ In the code above:                                  │
    │   - First half → NEW THREAD (async)                 │
    │   - Second half → CURRENT THREAD                    │
    │                                                     │
    │ This is CRITICAL! If you spawn a thread for BOTH    │
    │ halves, the current thread just waits (wasted!)     │
    │ Always do half the work yourself!                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ParallelMergeSort() {
    std::cout << "\n=== Parallel Merge Sort (Classic Fork-Join) ===" << std::endl;

    /*
     * MERGE SORT IS PERFECT FOR FORK-JOIN:
     * ================================================================
     */

    auto parallel_merge = [](std::vector<int>& v,
                             std::vector<int>& temp,
                             size_t begin, size_t end,
                             auto&& self) -> void {
                                 if(end - begin < 1000) {
                                     // Base case: sequential sort for small ranges
                                     std::sort(v.begin() + begin, v.begin() + end);
                                     return;
                                 }

                                 size_t mid = (begin + end) / 2;

                                 // FORK: sort first half in parallel
                                 auto future = std::async(std::launch::deferr
                                                          std::async(std::launch::async, [&v, &temp, mid, end, &self]() {
                                                              self(v, temp, mid, end, self);
                                                                     });

                                 // Current thread sorts the first half:
                                 self(v, temp, begin, mid, self);

                                 // JOIN: wait for second half
                                 future.get();

                                 // MERGE the two sorted halves:
                                 std::inplace_merge(v.begin() + begin, v.begin() + mid, v.begin() + end);
                                                          };

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), std::mt19937{42});

    std::vector<int> temp(data.size());  // Scratch buffer

    auto start = std::chrono::steady_clock::now();
    parallel_merge(data, temp, 0, data.size(),
                   [&parallel_merge](auto& v, auto& t, size_t b, size_t e, auto&& s) {
                       parallel_merge(v, t, b, e, parallel_merge);
                   });
    auto end = std::chrono::steady_clock::now();

    std::cout << "  Sorted 1M elements in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << "ms" << std::endl;
    std::cout << "  Correctly sorted: " << std::is_sorted(data.begin(), data.end()) << std::endl;
}