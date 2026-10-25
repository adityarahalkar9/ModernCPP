/*
 * ============================================================================
 * SYNCHRONIZATION PATTERNS - COMMON SOLUTIONS TO COMMON PROBLEMS
 * ============================================================================
 *
 * A CATALOG OF PATTERNS:
 * ======================
 * 1. Mutex + data (protecting shared state)
 * 2. Condition variable (waiting for conditions)
 * 3. Future/promise (one-way communication)
 * 4. Barrier (everyone waits for everyone)
 * 5. Semaphore (limited concurrent access)
 * 6. Read-write lock (many readers, one writer)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include "concurrency_header.h"

void PatternCatalog() {
    std::cout << "=== Synchronization Pattern Catalog ===" << std::endl;

    /*
     * PATTERN 1: MUTEX PROTECTING DATA
     * ================================================================
     * Problem: Multiple threads modifying shared data
     */
    {
        std::mutex data_mutex;
        std::vector<int> shared_data;

        auto writer = [&shared_data, &data_mutex](int id, int count) {
            for(int i = 0; i < count; ++i) {
                std::lock_guard<std::mutex> lock(data_mutex);
                shared_data.push_back(id * 100 + i);
            }
            };

        std::vector<std::thread> threads;
        for(int i = 0; i < 3; ++i) {
            threads.emplace_back(writer, i, 100);
        }
        for(auto& t : threads) t.join();

        std::cout << "  1. Mutex: vector has " << shared_data.size() << " elements" << std::endl;
    }

    /*
     * PATTERN 2: CONDITION VARIABLE (WAIT/SIGNAL)
     * ================================================================
     * Problem: Thread must wait for a condition to become true
     */
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        std::thread waiter([&]() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&ready]() { return ready; });
            std::cout << "  2. Condition var: waiter proceeded" << std::endl;
                           });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
        }
        cv.notify_one();
        waiter.join();
    }

    /*
     * PATTERN 3: FUTURE/PROMISE (RESULT DELIVERY)
     * ================================================================
     * Problem: Getting a result from a worker thread
     */
    {
        std::promise<int> promise;
        std::future<int> future = promise.get_future();

        std::thread worker([&promise]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            promise.set_value(42);  // The "answer"
                           });

        int result = future.get();  // Blocks until available
        std::cout << "  3. Future: got result " << result << std::endl;
        worker.join();
    }

    /*
     * PATTERN 4: SHARED MUTEX (READ/WRITE)
     * ================================================================
     * Problem: Many readers, few writers
     */
    {
        std::shared_mutex rw_mutex;
        int shared_value = 100;

        // Readers (can run concurrently):
        auto reader = [&shared_value, &rw_mutex]() {
            std::shared_lock<std::shared_mutex> lock(rw_mutex);
            return shared_value;
            };

        // Writer (exclusive):
        auto writer = [&shared_value, &rw_mutex](int new_val) {
            std::unique_lock<std::shared_mutex> lock(rw_mutex);
            shared_value = new_val;
            };

        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back([reader, i]() {
                volatile int val = reader();
                                 });
        }
        threads.emplace_back(writer, 200);

        for(auto& t : threads) t.join();
        std::cout << "  4. Shared mutex: final value = " << shared_value << std::endl;
    }

    std::cout << R"(
    PATTERN SELECTION GUIDE:
    ┌─────────────────────────────────────────────────────┐
    │ Problem                        → Pattern             │
    ├─────────────────────────────────────────────────────┤
    │ Protect shared data            → mutex              │
    │ Wait for condition             → condition_variable │
    │ Get result from thread         → future/promise     │
    │ Many readers, one writer       → shared_mutex       │
    │ Wait for all threads           → barrier/latch      │
    │ Limit concurrent access        → semaphore          │
    │ Simple counter/flag            → atomic             │
    │ One-time initialization        → call_once          │
    │ Thread-specific storage        → thread_local       │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}