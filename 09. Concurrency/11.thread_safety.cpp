/*
 * ============================================================================
 * THREAD SAFETY - WRITING CODE THAT WORKS CONCURRENTLY
 * ============================================================================
 *
 * WHAT DOES "THREAD-SAFE" MEAN?
 * =============================
 * A piece of code is thread-safe if it behaves correctly
 * when accessed from multiple threads simultaneously.
 *
 * THE THREE LEVELS OF THREAD SAFETY:
 * =================================
 * 1. NOT thread-safe: Needs external synchronization
 * 2. Conditionally safe: Safe if used correctly (documented rules)
 * 3. Unconditionally safe: Safe no matter how you use it
 *
 * HOW TO ACHIEVE THREAD SAFETY:
 * ============================
 * 1. Don't share mutable state (best!)
 * 2. Use mutexes to protect shared state
 * 3. Use atomics for simple operations
 * 4. Use immutable data structures
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include "concurrency_header.h"

void ThreadSafetyLevels() {
    std::cout << "=== Thread Safety Levels ===" << std::endl;

    /*
     * LEVEL 1: NOT THREAD-SAFE (needs external protection)
     * ================================================================
     */
    {
        // std::vector is NOT thread-safe for writes:
        // Multiple threads calling push_back simultaneously = data race!

        std::vector<int> shared_vec;

        auto unsafe_worker = [&shared_vec](int id) {
            for(int i = 0; i < 1000; ++i) {
                shared_vec.push_back(i);  // RACE CONDITION!
            }
            };

        std::cout << "  std::vector: NOT thread-safe for concurrent writes" << std::endl;
        std::cout << "  Must protect with external mutex" << std::endl;
    }

    /*
     * LEVEL 2: CONDITIONALLY SAFE (with proper usage)
     * ================================================================
     */
    {
        // With a mutex, the vector becomes "conditionally safe"
        // IF everyone uses the mutex correctly

        std::vector<int> safe_vec;
        std::mutex vec_mutex;

        auto safe_worker = [&safe_vec, &vec_mutex](int id) {
            for(int i = 0; i < 1000; ++i) {
                std::lock_guard<std::mutex> lock(vec_mutex);
                safe_vec.push_back(i);
            }
            };

        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back(safe_worker, i);
        }
        for(auto& t : threads) t.join();

        std::cout << "  Protected vector size: " << safe_vec.size()
            << " (expected 4000)" << std::endl;
    }

    /*
     * LEVEL 3: UNCONDITIONALLY SAFE (atomic operations)
     * ================================================================
     */
    {
        std::atomic<int> atomic_counter{0};

        auto atomic_worker = [&atomic_counter](int id) {
            for(int i = 0; i < 100000; ++i) {
                atomic_counter.fetch_add(1, std::memory_order_relaxed);
            }
            };

        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back(atomic_worker, i);
        }
        for(auto& t : threads) t.join();

        std::cout << "  Atomic counter: " << atomic_counter.load()
            << " (always correct!)" << std::endl;
    }

    std::cout << R"(
    THREAD SAFETY OF COMMON TYPES:
    ┌─────────────────────────────────────────────────────┐
    │ std::vector:    NOT thread-safe (writes)            │
    │ std::map:       NOT thread-safe (writes)            │
    │ std::cout:      NOT thread-safe (interleaves)       │
    │ std::atomic:    Thread-safe (by design)             │
    │ std::mutex:     Thread-safe (that's its job!)       │
    │ const objects:  Thread-safe (if truly immutable)    │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void DesigningThreadSafeClass() {
    std::cout << "\n=== Designing a Thread-Safe Class ===" << std::endl;

    /*
     * PATTERN: Protect ALL access to mutable state
     * ================================================================
     */

    class ThreadSafeCounter {
    private:
        mutable std::mutex mtx_;
        int count_ = 0;
        std::vector<int> history_;

    public:
        // Every method that touches state must lock
        void increment() {
            std::lock_guard<std::mutex> lock(mtx_);
            ++count_;
            history_.push_back(count_);
        }

        int get() const {
            std::lock_guard<std::mutex> lock(mtx_);
            return count_;
        }

        // COMPOUND OPERATIONS need special care!
        // This is NOT the same as calling get() then set():
        int increment_and_get() {
            std::lock_guard<std::mutex> lock(mtx_);
            ++count_;
            return count_;  // The get is atomic with the increment
        }

        // Batch operation - all under one lock (efficient!)
        void increment_by(int amount) {
            std::lock_guard<std::mutex> lock(mtx_);
            for(int i = 0; i < amount; ++i) {
                ++count_;
            }
        }

        // Snapshot - consistent view of state
        std::pair<int, size_t> get_status() const {
            std::lock_guard<std::mutex> lock(mtx_);
            return {count_, history_.size()};
        }
    };

    ThreadSafeCounter counter;
    std::vector<std::thread> threads;

    for(int i = 0; i < 4; ++i) {
        threads.emplace_back([&counter]() {
            for(int j = 0; j < 1000; ++j) {
                counter.increment();
            }
                             });
    }

    for(auto& t : threads) t.join();

    auto [count, history_size] = counter.get_status();
    std::cout << "  Final count: " << count << " (expected 4000)" << std::endl;
    std::cout << "  History size: " << history_size << std::endl;

    std::cout << R"(
    RULES FOR THREAD-SAFE CLASSES:
    ┌─────────────────────────────────────────────────────┐
    │ 1. EVERY method that accesses mutable state must lock│
    │ 2. Compound operations need ONE lock for the WHOLE  │
    │    operation (not separate locks for parts)          │
    │ 3. Const methods can still need locks (if state      │
    │    can change concurrently)                          │
    │ 4. Document thread-safety guarantees                │
    │ 5. Consider: Is a lock-free version possible?        │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}