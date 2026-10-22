/*
 * ATOMIC OPERATIONS - LOCK-FREE SYNCHRONIZATION
 *
 * Atomics provide indivisible operations without mutexes.
 * Faster than mutexes for simple operations.
 */

import std;
#include <atomic>
#include <thread>
#include "concurrency_header.h"

void AtomicBasics() {
    std::cout << "=== Atomics: Lock-Free Synchronization ===" << std::endl;

    std::atomic<int> counter{0};

    // Atomic increment - no race condition!
    auto worker = [&counter](int times) {
        for(int i = 0; i < times; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
        };

    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, 100000);
    }
    for(auto& t : threads) t.join();

    std::cout << "  Counter: " << counter.load() << " (expected 400000)" << std::endl;
}