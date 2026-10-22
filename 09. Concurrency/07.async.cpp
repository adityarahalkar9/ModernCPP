/*
 * ============================================================================
 * STD::ASYNC - HIGH-LEVEL ASYNCHRONOUS EXECUTION
 * ============================================================================
 *
 * WHAT IS std::async?
 * ===================
 * A function that runs a task asynchronously and returns a future
 * holding the result. It's the EASIEST way to run code in parallel.
 *
 * THE LAUNCH POLICIES:
 * ====================
 * 1. std::launch::async:     Run in a NEW thread (true parallelism)
 * 2. std::launch::deferred:  Run in CURRENT thread when get() is called
 * 3. Default (both):         Implementation chooses
 *
 * THE EASE OF USE:
 * ================
 * // Instead of manually creating threads + promises:
 * auto result = std::async(std::launch::async, expensive_function, args);
 * // ... do other work ...
 * auto value = result.get();  // Wait for result
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <numeric>
#include "concurrency_header.h"

void AsyncBasics() {
    std::cout << "=== std::async Basics ===" << std::endl;

    /*
     * THE SIMPLEST PARALLELISM:
     * ================================================================
     */

     // Launch an async task:
    std::cout << "  Launching async task..." << std::endl;
    auto future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 42;  // The task's return value
                             });

    // Do other work while waiting:
    std::cout << "  Doing other work while task runs..." << std::endl;
    for(int i = 0; i < 5; ++i) {
        std::cout << "    Step " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Get the result (blocks if not ready yet):
    int result = future.get();
    std::cout << "  Result from async: " << result << std::endl;
}

void LaunchPolicies() {
    std::cout << "\n=== Launch Policies Explained ===" << std::endl;

    /*
     * POLICY 1: std::launch::async
     * ================================================================
     * Task runs in a NEW thread immediately.
     */
    {
        auto start = std::chrono::steady_clock::now();

        auto f = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return "async result";
                            });

        // Can do other things here while task runs in another thread
        std::cout << "  [async] doing other work..." << std::endl;

        std::string result = f.get();
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "  [async] result: " << result
            << " (took ~" << elapsed << "ms)" << std::endl;
    }

    /*
     * POLICY 2: std::launch::deferred
     * ================================================================
     * Task runs in the CALLING thread, but only when get() is called.
     */
    {
        auto f = std::async(std::launch::deferred, []() {
            std::cout << "  [deferred] now executing (in caller's thread)" << std::endl;
            return "deferred result";
                            });

        std::cout << "  [deferred] not executed yet!" << std::endl;

        // get() triggers the execution:
        std::string result = f.get();
        std::cout << "  [deferred] result: " << result << std::endl;
    }

    std::cout << R"(
    LAUNCH POLICY COMPARISON:
    ┌──────────────────┬────────────────┬────────────────┐
    │                  │ async          │ deferred       │
    ├──────────────────┼────────────────┼────────────────┤
    │ Where it runs    │ New thread     │ Caller's thread│
    ├──────────────────┼────────────────┼────────────────┤
    │ When it starts   │ Immediately   │ At get() call  │
    ├──────────────────┼────────────────┼────────────────┤
    │ Parallelism      │ YES            │ NO (lazy)      │
    ├──────────────────┼────────────────┼────────────────┤
    │ Use case         │ True parallel │ Lazy evaluation│
    └──────────────────┴────────────────┴────────────────┘

    DEFAULT (no policy): Implementation chooses!
    Might be async OR deferred - be explicit!
)" << std::endl;
}

void ParallelComputation() {
    std::cout << "\n=== Parallel Computation with async ===" << std::endl;

    /*
     * REAL-WORLD EXAMPLE: Parallel sum
     * ================================================================
     */

    std::vector<int> data(10'000'000);
    std::iota(data.begin(), data.end(), 1);  // 1 to 10,000,000

    // SEQUENTIAL version:
    auto start = std::chrono::steady_clock::now();
    long long seq_sum = 0;
    for(int v : data) {
        seq_sum += v;
    }
    auto end = std::chrono::steady_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // PARALLEL version using async:
    start = std::chrono::steady_clock::now();

    // Split into 4 chunks:
    size_t chunk_size = data.size() / 4;
    std::vector<std::future<long long>> futures;

    for(int i = 0; i < 4; ++i) {
        auto begin_it = data.begin() + (i * chunk_size);
        auto end_it = (i == 3) ? data.end() : (begin_it + chunk_size);

        futures.push_back(std::async(std::launch::async,
                                     [begin_it, end_it]() {
                                         long long partial = 0;
                                         for(auto it = begin_it; it != end_it; ++it) {
                                             partial += *it;
                                         }
                                         return partial;
                                     }));
    }

    // Collect partial results:
    long long par_sum = 0;
    for(auto& f : futures) {
        par_sum += f.get();
    }

    end = std::chrono::steady_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "  Sequential: sum=" << seq_sum << ", time=" << seq_time << "μs" << std::endl;
    std::cout << "  Parallel:   sum=" << par_sum << ", time=" << par_time << "μs" << std::endl;
    std::cout << "  (Parallel is often faster for large datasets!)" << std::endl;
}