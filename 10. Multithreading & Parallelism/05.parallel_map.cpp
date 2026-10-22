/*
 * ============================================================================
 * PARALLEL MAP - APPLYING OPERATIONS TO EVERY ELEMENT
 * ============================================================================
 *
 * WHAT IS MAP?
 * ============
 * Apply a function to each element of a collection:
 *   result[i] = f(input[i])
 *
 * THE SIMPLEST PARALLEL PATTERN:
 * =============================
 * Every element is independent → embarrassingly parallel!
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>
#include <chrono>
#include "parallelism_header.h"

void ParallelMapPatterns() {
    std::cout << "=== Parallel Map Patterns ===" << std::endl;

    /*
     * PATTERN 1: Using std::transform with execution policy
     * ================================================================
     */

    std::vector<double> input(10'000'000);
    std::iota(input.begin(), input.end(), 1.0);
    std::vector<double> output(input.size());

    // Parallel transform IS parallel map:
    std::transform(std::execution::par, input.begin(), input.end(),
                   output.begin(),
                   [](double x) { return std::sqrt(x); });

    std::cout << "  sqrt(1) = " << output[0] << std::endl;
    std::cout << "  sqrt(1000000) = " << output[999999] << std::endl;

    /*
     * PATTERN 2: Manual chunking (more control)
     * ================================================================
     */

    auto parallel_map_manual = [](std::vector<double>& data, auto func) {
        size_t num_threads = std::thread::hardware_concurrency();
        size_t chunk_size = data.size() / num_threads;

        std::vector<std::thread> threads;
        std::vector<double> results(data.size());

        for(size_t t = 0; t < num_threads; ++t) {
            size_t begin = t * chunk_size;
            size_t end = (t == num_threads - 1) ? data.size() : begin + chunk_size;

            threads.emplace_back([&data, &results, begin, end, func]() {
                for(size_t i = begin; i < end; ++i) {
                    results[i] = func(data[i]);
                }
                                 });
        }

        for(auto& t : threads) {
            t.join();
        }

        return results;
        };

    auto start = std::chrono::steady_clock::now();
    auto result = parallel_map_manual(input, [](double x) { return x * 2.0; });
    auto end = std::chrono::steady_clock::now();

    std::cout << "  Manual parallel map: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << "ms" << std::endl;
    std::cout << "  First element: " << result[0] << " (should be 2.0)" << std::endl;
}