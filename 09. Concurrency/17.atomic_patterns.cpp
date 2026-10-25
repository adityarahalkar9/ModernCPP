/*
 * ============================================================================
 * ATOMIC PATTERNS - LOCK-FREE PROGRAMMING TECHNIQUES
 * ============================================================================
 *
 * WHEN TO USE ATOMICS vs MUTEXES:
 * ===============================
 * ATOMICS: Simple operations on single variables
 *          (counter, flag, pointer swap)
 *
 * MUTEXES: Complex invariants spanning multiple variables
 *          (linked list consistency, multi-field updates)
 *
 * THE ATOMIC TOOLBOX:
 * ===================
 * - load(): Read atomically
 * - store(): Write atomically
 * - fetch_add/fetch_sub: Atomic arithmetic
 * - compare_exchange_strong/weak: CAS (compare-and-swap)
 * - exchange: Atomic swap
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include "concurrency_header.h"

void AtomicCounterPattern() {
    std::cout << "=== Pattern 1: Atomic Counter ===" << std::endl;

    /*
     * THE SIMPLEST ATOMIC USE CASE:
     * ================================================================
     */

    std::atomic<int> counter{0};

    auto worker = [&counter](int iterations) {
        for(int i = 0; i < iterations; ++i) {
            // Atomic increment - no race conditions!
            counter.fetch_add(1, std::memory_order_relaxed);
        }
        };

    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, 100000);
    }
    for(auto& t : threads) t.join();

    std::cout << "  Counter: " << counter.load()
        << " (expected 400000, no lost updates!)" << std::endl;
}

void AtomicFlagPattern() {
    std::cout << "\n=== Pattern 2: Atomic Flag (Stop Signal) ===" << std::endl;

    /*
     * FLAG FOR THREAD COORDINATION:
     * ================================================================
     */

    std::atomic<bool> should_stop{false};
    std::atomic<int> progress{0};

    auto worker = [&should_stop, &progress]() {
        while(!should_stop.load(std::memory_order_acquire)) {
            // Do work:
            progress.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "  Worker: stopping gracefully" << std::endl;
        };

    std::thread t(worker);

    // Let it run:
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Signal to stop:
    std::cout << "  Signaling stop..." << std::endl;
    should_stop.store(true, std::memory_order_release);

    t.join();
    std::cout << "  Progress made: " << progress.load() << " iterations" << std::endl;
}

void CASPattern() {
    std::cout << "\n=== Pattern 3: Compare-and-Swap (CAS) ===" << std::endl;

    /*
     * COMPARE-AND-SWAP - THE FOUNDATION OF LOCK-FREE:
     * ================================================================
     * "If the value is still X, change it to Y"
     * If it changed in the meantime, retry!
     */

    std::atomic<int> value{0};

    // Lock-free update pattern:
    auto atomic_update = [&value](int increment) {
        int expected = value.load();
        int desired;

        do {
            desired = expected + increment;
            // CAS: if value is still 'expected', set it to 'desired'
            // Returns true if successful
        } while(!value.compare_exchange_weak(expected, desired));
        // Note: if CAS fails, 'expected' is updated with the CURRENT value
        // So the loop retries with the new expected value
        };

    // Multiple threads doing atomic updates:
    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(atomic_update, 1000);
    }
    for(auto& t : threads) t.join();

    std::cout << "  Final value: " << value.load() << " (expected 4000)" << std::endl;

    std::cout << R"(
    HOW CAS WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ Thread A:                                          │
    │   expected = 100                                    │
    │   desired = 101                                     │
    │   CAS: is value still 100?                         │
    │     YES → set to 101, return true (success!)       │
    │                                                     │
    │ Thread B (racing with A):                          │
    │   expected = 100                                    │
    │   desired = 150                                     │
    │   CAS: is value still 100?                         │
    │     NO (A already changed it to 101)               │
    │     → return false, 'expected' updated to 101      │
    │     → retry with expected=101                      │
    │                                                     │
    │ This is how lock-free algorithms avoid locks!      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void AtomicSharedPointer() {
    std::cout << "\n=== Pattern 4: Atomic shared_ptr (C++20) ===" << std::endl;

    /*
     * ATOMIC SMART POINTER OPERATIONS:
     * ================================================================
     * C++20 allows std::atomic<std::shared_ptr<T>>
     * for lock-free pointer swapping.
     */

     // Simulating a configuration that can be swapped atomically:
    auto config = std::make_shared<std::string>("initial_config");

    // Reader threads see either old or new config, never garbage:
    auto reader = [&config](int id) {
        for(int i = 0; i < 5; ++i) {
            // Atomic load - always sees a valid state:
            auto current = std::atomic_load(&config);
            std::cout << "  Reader " << id << " sees: " << *current << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        };

    // Writer thread updates the config:
    std::thread updater([&config]() {
        for(int i = 0; i < 3; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            auto new_config = std::make_shared<std::string>(
                "config_v" + std::to_string(i + 1));

            // Atomic swap - readers see old or new, never invalid:
            std::atomic_store(&config, new_config);
            std::cout << "  Updater installed config_v" << (i + 1) << std::endl;
        }
                        });

    std::vector<std::thread> readers;
    for(int i = 0; i < 2; ++i) {
        readers.emplace_back(reader, i);
    }

    updater.join();
    for(auto& r : readers) r.join();
}