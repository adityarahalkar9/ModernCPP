/*
 * ============================================================================
 * FALSE SHARING - THE SILENT PERFORMANCE KILLER
 * ============================================================================
 *
 * WHAT IS FALSE SHARING?
 * =====================
 * When multiple threads frequently modify DIFFERENT variables
 * that happen to be on the SAME CACHE LINE.
 *
 * THE PROBLEM:
 * ============
 * CPU caches work at CACHE LINE granularity (typically 64 bytes).
 * If two threads write to different variables on the same line:
 * - The cache line bounces between cores
 * - Each write invalidates the other core's copy
 * - Performance can drop 10-100x!
 *
 * VISUALIZATION:
 * ==============
 *   Core 1 cache: [ var_a | var_b | ... ]  ← 64-byte cache line
 *   Core 2 cache: [ var_a | var_b | ... ]  ← same line
 *
 *   Thread 1 writes var_a → invalidates Core 2's line
 *   Thread 2 writes var_b → invalidates Core 1's line
 *   → PING-PONG of the entire line!
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include "parallelism_header.h"

void FalseSharingDemonstration() {
    std::cout << "=== False Sharing: The Performance Killer ===" << std::endl;

    /*
     * BAD VERSION: False sharing
     * ================================================================
     */
    {
        struct Counters {
            std::atomic<int> a;  // Thread 1 modifies this
            std::atomic<int> b;  // Thread 2 modifies this
            std::atomic<int> c;  // Thread 3 modifies this
            std::atomic<int> d;  // Thread 4 modifies this
            // All four atomics are likely on the SAME cache line!
        };

        Counters counters;

        auto bad_worker = [&counters](int id, int iterations) {
            std::atomic<int>* target;
            switch(id) {
                case 0: target = &counters.a; break;
                case 1: target = &counters.b; break;
                case 2: target = &counters.c; break;
                default: target = &counters.d; break;
            }

            for(int i = 0; i < iterations; ++i) {
                target->fetch_add(1, std::memory_order_relaxed);
            }
            };

        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back(bad_worker, i, 5'000'000);
        }
        for(auto& t : threads) t.join();
        auto end = std::chrono::steady_clock::now();

        auto bad_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "  WITHOUT padding: " << bad_ms << "ms" << std::endl;
        std::cout << "  Total: " << (counters.a + counters.b + counters.c + counters.d) << std::endl;
    }

    /*
     * GOOD VERSION: Aligned to cache lines
     * ================================================================
     */
    {
        struct alignas(64) PaddedCounter {
            std::atomic<int> value;
            char padding[64 - sizeof(std::atomic<int>)];
            // Padding ensures each counter is on its own cache line
        };

        std::vector<PaddedCounter> counters(4);

        auto good_worker = [&counters](int id, int iterations) {
            for(int i = 0; i < iterations; ++i) {
                counters[id].value.fetch_add(1, std::memory_order_relaxed);
            }
            };

        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back(good_worker, i, 5'000'000);
        }
        for(auto& t : threads) t.join();
        auto end = std::chrono::steady_clock::now();

        auto good_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "  WITH padding:    " << good_ms << "ms" << std::endl;

        long total = 0;
        for(auto& c : counters) total += c.value.load();
        std::cout << "  Total: " << total << std::endl;

        std::cout << R"(
    THE CACHE LINE PROBLEM:
    ┌─────────────────────────────────────────────────────┐
    │ 64-byte Cache Line:                                 │
    │ [  a  |  b  |  c  |  d  | ... unused ... ]         │
    │    ↑       ↑       ↑       ↑                        │
    │  Thread1 Thread2 Thread3 Thread4                    │
    │                                                     │
    │ Each write by ANY thread invalidates the line      │
    │ in ALL other cores' caches!                         │
    │                                                     │
    │ FIX: Separate into different cache lines           │
    │ [  a  | --- padding --- ]  ← Thread 1's line        │
    │ [  b  | --- padding --- ]  ← Thread 2's line        │
    │ [  c  | --- padding --- ]  ← Thread 3's line        │
    │ [  d  | --- padding --- ]  ← Thread 4's line        │
    └─────────────────────────────────────────────────────┘

    WHEN TO WORRY ABOUT FALSE SHARING:
    ┌─────────────────────────────────────────────────────┐
    │ • Multiple threads writing to nearby memory          │
    │ • High write frequency (millions/sec)               │
    │ • Performance mysteriously bad despite parallelism │
    │                                                     │
    │ COMMON CULPRITS:                                    │
    │ • Arrays of counters/accumulators                   │
    │ • Element access in tight loops                     │
    │ • "One variable per thread" patterns                │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
    }
}