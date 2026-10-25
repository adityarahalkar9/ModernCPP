/*
 * ============================================================================
 * BENCHMARKING CONCURRENT CODE - MEASURE, DON'T GUESS
 * ============================================================================
 *
 * WHY BENCHMARK?
 * ===============
 * Concurrent code performance is counterintuitive:
 * - More threads ≠ more speed (oversubscription, contention)
 * - "Optimizations" can make things slower
 * - Results vary by hardware, OS, load
 *
 * BENCHMARKING RULES:
 * ===================
 * 1. Measure what matters (end-to-end time, not just CPU)
 * 2. Run multiple times, report median/mean
 * 3. Warm up caches before measuring
 * 4. Use realistic data sizes
 * 5. Control for system noise (background processes)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "concurrency_header.h"

void BenchmarkBasics() {
    std::cout << "=== Benchmarking Concurrent Code ===" << std::endl;

    /*
     * A SIMPLE BENCHMARK HARNESS:
     * ================================================================
     */

    auto benchmark = [](const std::string& name, auto func, int runs = 5) {
        std::vector<double> times;

        // Warm-up run (not measured):
        func();

        for(int i = 0; i < runs; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            func();
            auto end = std::chrono::high_resolution_clock::now();

            auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            times.push_back(ms);
        }

        // Statistics:
        double sum = 0;
        for(double t : times) sum += t;
        double mean = sum / times.size();

        std::sort(times.begin(), times.end());
        double median = times[times.size() / 2];

        std::cout << "  " << name << ":"
            << " mean=" << mean << "μs"
            << " median=" << median << "μs"
            << " min=" << times[0] << "μs"
            << " max=" << times.back() << "μs" << std::endl;

        return median;
        };

    // Benchmark: sequential vs parallel sum
    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);

    // Sequential:
    auto seq_median = benchmark("Sequential sum",
                                [&data]() {
                                    volatile long long sum = 0;
                                    for(int v : data) sum += v;
                                });

    // Parallel with std::reduce:
    auto par_median = benchmark("Parallel reduce",
                                [&data]() {
                                    volatile long long sum = std::reduce(
                                        std::execution::par, data.begin(), data.end(), 0LL);
                                });

    // Report speedup:
    if(par_median > 0) {
        std::cout << "  Speedup: " << (seq_median / par_median) << "x" << std::endl;
    }

    std::cout << R"(
    BENCHMARKING BEST PRACTICES:
    ┌─────────────────────────────────────────────────────┐
    │ 1. WARM UP: First run is often slower (caches)      │
    │ 2. MULTIPLE RUNS: Report median (outlier-resistant)│
    │ 3. SAME DATA: Use identical inputs for fair compare│
    │ 4. REALISTIC SIZE: Too small = overhead dominates   │
    │ 5. SYSTEM QUIET: Close other programs              │
    │                                                     │
    │ COMMON PITFALLS:                                    │
    │ • Compiler optimizes work away (use volatile sink)  │
    │ • Measuring setup time instead of work             │
    │ • Too few iterations (noise dominates)             │
    │ • Comparing on different data                      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ScalingBenchmark() {
    std::cout << "\n=== Thread Scaling Benchmark ===" << std::endl;

    /*
     * MEASURING HOW PERFORMANCE SCALES WITH THREADS:
     * ================================================================
     */

    std::vector<int> data(10'000'000);
    std::iota(data.begin(), data.end(), 1);

    auto measure_parallel_sum = [&data](size_t num_threads) {
        auto start = std::chrono::high_resolution_clock::now();

        // Split work among threads:
        std::vector<std::thread> threads;
        std::vector<long long> partial_sums(num_threads, 0);

        size_t chunk = data.size() / num_threads;

        for(size_t t = 0; t < num_threads; ++t) {
            size_t begin = t * chunk;
            size_t end = (t == num_threads - 1) ? data.size() : begin + chunk;

            threads.emplace_back([&data, &partial_sums, t, begin, end]() {
                long long partial = 0;
                for(size_t i = begin; i < end; ++i) {
                    partial += data[i];
                }
                partial_sums[t] = partial;
                                 });
        }

        for(auto& th : threads) th.join();

        long long total = 0;
        for(long long p : partial_sums) total += p;

        auto end_time = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();

        return std::make_pair(total, ms);
        };

    // Test different thread counts:
    std::cout << "  Threads | Time (ms) | Speedup" << std::endl;
    std::cout << "  --------|-----------|--------" << std::endl;

    // Baseline (sequential):
    auto [seq_result, seq_ms] = measure_parallel_sum(1);
    std::cout << "  1       | " << seq_ms << "       | 1.00x" << std::endl;

    for(size_t threads : {2, 4, 8}) {
        auto [result, ms] = measure_parallel_sum(threads);
        double speedup = static_cast<double>(seq_ms) / ms;
        std::cout << "  " << threads << "       | " << ms
            << "       | " << speedup << "x" << std::endl;
    }

    std::cout << R"(
    EXPECTED RESULTS:
    ┌─────────────────────────────────────────────────────┐
    │ • Speedup should increase with more threads         │
    │ • But NOT linearly (Amdahl's Law)                   │
    │ • Diminishing returns after #physical cores         │
    │ • Might get WORSE with too many threads            │
    │   (oversubscription, context switching)            │
    │                                                     │
    │ IDEAL:                                              │
    │   4 cores → ~3.5x speedup (some overhead)           │
    │   8 threads on 4 cores → might be SLOWER           │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}