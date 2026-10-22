/*
 * ============================================================================
 * PARALLEL ALGORITHMS (C++17) - THE STL GETS MULTITHREADED
 * ============================================================================
 *
 * WHAT CHANGED IN C++17?
 * =====================
 * The Standard Library algorithms can now run in PARALLEL!
 * Over 60 algorithms received parallel versions:
 *   std::sort, std::transform, std::accumulate, std::for_each,
 *   std::find, std::count, std::copy, std::reduce, etc.
 *
 * HOW TO USE THEM:
 * ================
 * Just add an execution policy as the FIRST argument:
 *
 *   std::sort(seq, vec.begin(), vec.end());     // Sequential (old way)
 *   std::sort(par, vec.begin(), vec.end());     // PARALLEL!
 *
 * THE IMPACT:
 * ===========
 * Sorting 10 million elements:
 *   Sequential: ~800ms
 *   Parallel:   ~200ms (4x speedup on 4 cores)
 *   Your code change: ONE WORD!
 *
 * ============================================================================
 */


import std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include "parallelism_header.h"

void ParallelSortDemo() {
    std::cout << "=== Parallel std::sort (C++17) ===" << std::endl;

    /*
     * THE SIMPLEST PARALLELISM YOU'LL EVER WRITE:
     * ================================================================
     */

     // Create test data:
    std::vector<int> data(5'000'000);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), std::mt19937{42});

    // Make a copy for fair comparison:
    auto data_copy = data;

    // SEQUENTIAL sort:
    auto start = std::chrono::steady_clock::now();
    std::sort(data.begin(), data.end());  // No policy = sequential
    auto end = std::chrono::steady_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // PARALLEL sort - notice the ONE difference:
    start = std::chrono::steady_clock::now();
    std::sort(std::execution::par, data_copy.begin(), data_copy.end());  // ← ADD THIS!
    end = std::chrono::steady_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Sorting 5,000,000 elements:" << std::endl;
    std::cout << "  Sequential: " << seq_time << "ms" << std::endl;
    std::cout << "  Parallel:   " << par_time << "ms" << std::endl;
    std::cout << "  Speedup:    " << (seq_time / static_cast<double>(par_time)) << "x" << std::endl;

    // Verify both are correctly sorted:
    bool both_sorted = std::is_sorted(data.begin(), data.end())
        && std::is_sorted(data_copy.begin(), data_copy.end());
    std::cout << "  Both correctly sorted: " << both_sorted << std::endl;
}

void ParallelTransformDemo() {
    std::cout << "\n=== Parallel std::transform ===" << std::endl;

    /*
     * TRANSFORMING DATA IN PARALLEL:
     * ================================================================
     */

    std::vector<double> input(10'000'000);
    std::iota(input.begin(), input.end(), 1.0);

    std::vector<double> output_seq(input.size());
    std::vector<double> output_par(input.size());

    // Sequential transform:
    auto start = std::chrono::steady_clock::now();
    std::transform(input.begin(), input.end(), output_seq.begin(),
                   [](double x) { return x * x; });  // Square each element
    auto end = std::chrono::steady_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Parallel transform:
    start = std::chrono::steady_clock::now();
    std::transform(std::execution::par, input.begin(), input.end(), output_par.begin(),
                   [](double x) { return x * x; });
    end = std::chrono::steady_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Transforming 10M elements (squaring):" << std::endl;
    std::cout << "  Sequential: " << seq_time << "ms" << std::endl;
    std::cout << "  Parallel:   " << par_time << "ms" << std::endl;

    // Verify results match:
    bool same = std::equal(output_seq.begin(), output_seq.end(), output_par.begin());
    std::cout << "  Results identical: " << same << std::endl;
}

void ParallelReduceDemo() {
    std::cout << "\n=== Parallel std::reduce vs std::accumulate ===" << std::endl;

    /*
     * ACCUMULATE vs REDUCE:
     * ================================================================
     *
     * std::accumulate (old): Sequential only, left-fold
     * std::reduce (C++17):   Can be parallel, can reassociate
     *
     * IMPORTANT: reduce can apply the operation in ANY order!
     * This means your operation MUST be commutative and associative.
     */

    std::vector<long long> data(10'000'000);
    std::iota(data.begin(), data.end(), 1LL);

    // Sequential accumulate:
    auto start = std::chrono::steady_clock::now();
    long long sum_acc = std::accumulate(data.begin(), data.end(), 0LL);
    auto end = std::chrono::steady_clock::now();
    auto acc_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Parallel reduce:
    start = std::chrono::steady_clock::now();
    long long sum_red = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    end = std::chrono::steady_clock::now();
    auto red_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Summing 10M elements:" << std::endl;
    std::cout << "  accumulate (seq): " << sum_acc << " in " << acc_time << "ms" << std::endl;
    std::cout << "  reduce (par):     " << sum_red << " in " << red_time << "ms" << std::endl;

    std::cout << R"(
    WHEN IS PARALLEL REDUCE SAFE?
    ┌─────────────────────────────────────────────────────┐
    │ The operation must be:                              │
    │   • ASSOCIATIVE: (a+b)+c == a+(b+c)                 │
    │     ✓ Addition, multiplication                     │
    │     ✗ Subtraction, division                        │
    │                                                     │
    │   • COMMUTATIVE: a+b == b+a                        │
    │     ✓ Addition, max, min                           │
    │     ✗ String concatenation (order matters!)        │
    │                                                     │
    │ SAFE OPERATIONS:                                    │
    │   Sum, product, max, min, bitwise OR/AND/XOR        │
    │                                                     │
    │ UNSAFE OPERATIONS:                                  │
    │   Subtraction, division, string concat, list append │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}