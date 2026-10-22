/*
 * ============================================================================
 * PARALLEL SORTING - MULTIPLE APPROACHES
 * ============================================================================
 */


import std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <chrono>
#include <random>
#include <numeric>
#include "parallelism_header.h"

void SortingComparison() {
    std::cout << "=== Parallel Sorting Approaches ===" << std::endl;

    const size_t N = 10'000'000;

    // Prepare test data:
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), std::mt19937{42});

    auto data_copy1 = data;  // For parallel std::sort
    auto data_copy2 = data;  // For our custom parallel sort

    // APPROACH 1: Sequential std::sort
    auto start = std::chrono::steady_clock::now();
    std::sort(data.begin(), data.end());
    auto end = std::chrono::steady_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // APPROACH 2: Parallel std::sort (C++17)
    start = std::chrono::steady_clock::now();
    std::sort(std::execution::par, data_copy1.begin(), data_copy1.end());
    end = std::chrono::steady_clock::now();
    auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // APPROACH 3: Custom parallel quicksort (simplified)
    auto parallel_quicksort = [](std::vector<int>& v, int begin, int end, auto&& self) -> void {
        if(end - begin < 10000) {
            std::sort(v.begin() + begin, v.begin() + end);
            return;
        }

        // Simple partition (Lomuto scheme):
        int pivot = v[end - 1];
        int i = begin;
        for(int j = begin; j < end - 1; ++j) {
            if(v[j] < pivot) {
                std::swap(v[i], v[j]);
                ++i;
            }
        }
        std::swap(v[i], v[end - 1]);

        // Recurse in parallel:
        auto right_future = std::async(std::launch::async,
                                       [&v, i, end, &self]() {
                                           self(v, i + 1, end, self);
                                       });

        self(v, begin, i, self);  // Left half in current thread
        right_future.get();       // Wait for right half
        };

    start = std::chrono::steady_clock::now();
    parallel_quicksort(data_copy2, 0, data_copy2.size(), parallel_quicksort);
    end = std::chrono::steady_clock::now();
    auto custom_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Sorting " << N << " elements:" << std::endl;
    std::cout << "  1. Sequential:    " << seq_ms << "ms" << std::endl;
    std::cout << "  2. std::par:      " << par_ms << "ms" << std::endl;
    std::cout << "  3. Custom parallel: " << custom_ms << "ms" << std::endl;

    std::cout << R"(
    RECOMMENDATION:
    ┌─────────────────────────────────────────────────────┐
    │ USE std::sort(std::execution::par, ...)             │
    │                                                     │
    │ It's:                                               │
    │   • Well-tested                                     │
    │   • Highly optimized                                │
    │   • One-word change from sequential                 │
    │                                                     │
    │ Only write custom sorts for:                        │
    │   • Special data distributions                      │
    │   • Custom comparison logic that's expensive        │
    │   • Learning purposes                              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}