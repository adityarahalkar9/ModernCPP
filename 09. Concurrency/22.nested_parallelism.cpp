/*
 * ============================================================================
 * NESTED PARALLELISM - PARALLELISM INSIDE PARALLELISM
 * ============================================================================
 *
 * WHAT IS NESTED PARALLELISM?
 * ===========================
 * A parallel task that spawns additional parallel subtasks.
 *
 * THE DANGER: THREAD EXPLOSION
 * ============================
 * If each level multiplies threads:
 *   Level 1: 4 threads
 *   Level 2: 4 × 4 = 16 threads
 *   Level 3: 4 × 4 × 4 = 64 threads
 *   → Oversubscription, thrashing, terrible performance!
 *
 * SOLUTIONS:
 * ==========
 * 1. Limit total threads (not per-level)
 * 2. Use work-stealing schedulers
 * 3. Make inner levels sequential if outer is parallel enough
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include "concurrency_header.h"

void ThreadExplosion() {
    std::cout << "=== The Thread Explosion Problem ===" << std::endl;

    /*
     * THE PROBLEM VISUALIZED:
     * ================================================================
     */

    std::cout << R"(
    NESTED PARALLELISM GONE WRONG:
    ┌─────────────────────────────────────────────────────┐
    │ Level 0 (main):                                     │
    │   ┌─────────┐                                       │
    │   │ Task A  │──┬── Level 1:                         │
    │   └─────────┘  │   ┌─────────┐                       │
    │                ├───│ Sub A1 │──┬── Level 2:         │
    │                │   └─────────┘  │   ┌─────────┐     │
    │                │   ┌─────────┐  ├───│ SubSub1 │     │
    │                │   │ Sub A2 │──┤   └─────────┘     │
    │                │   └─────────┘  │   ┌─────────┐     │
    │                │                └───│ SubSub2 │     │
    │                │                    └─────────┘     │
    │                                                     │
    │ If each level creates 4 threads:                    │
    │   Level 1: 4 threads                                │
    │   Level 2: 16 threads                               │
    │   Level 3: 64 threads!                             │
    │                                                     │
    │ With only 8 cores, 64 threads = thrashing          │
    └─────────────────────────────────────────────────────┘
)" << std::endl;

    /*
     * SOLUTION: Bounded concurrency
     * ================================================================
     */

     // Use a semaphore-like counter to limit total active threads:
    std::atomic<int> active_threads{0};
    constexpr int MAX_THREADS = 8;  // Match hardware

    class ThreadLimiter {
    private:
        std::atomic<int>& counter_;
        int limit_;

    public:
        ThreadLimiter(std::atomic<int>& counter, int limit)
            : counter_(counter), limit_(limit) {
        }

        bool try_acquire() {
            int current = counter_.load(std::memory_order_relaxed);
            while(current < limit_) {
                if(counter_.compare_exchange_weak(current, current + 1)) {
                    return true;
                }
                // 'current' updated by CAS failure, retry
            }
            return false;  // At limit
        }

        void release() {
            counter_.fetch_sub(1, std::memory_order_relaxed);
        }
    };

    ThreadLimiter limiter(active_threads, MAX_THREADS);

    // Nested parallel work with limiting:
    std::function<void(int)> nested_task = [&](int level) {
        if(level >= 3) return;  // Max depth

        // Do this level's work:
        volatile int work = 0;
        for(int i = 0; i < 1000; ++i) {
            work += i;
        }

        // Try to spawn subtasks (if under thread limit):
        if(limiter.try_acquire()) {
            auto subtask = std::async(std::launch::async, [&nested_task, &limiter, level]() {
                nested_task(level + 1);
                limiter.release();
                                      });
            // Continue with this level's other work...
            subtask.get();  // Simplified: wait for subtask
        }
        else {
            // At thread limit, do it sequentially:
            nested_task(level + 1);
        }
        };

    // Run the nested task:
    auto start = std::chrono::steady_clock::now();
    nested_task(0);
    auto end = std::chrono::steady_clock::now();

    std::cout << "  Completed with thread limiting in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << "ms" << std::endl;
}

void PracticalNestedParallel() {
    std::cout << "\n=== Practical Nested Parallelism ===" << std::endl;

    /*
     * PRACTICAL EXAMPLE: Processing a matrix with nested loops
     * ================================================================
     */

     // Create a large matrix:
    constexpr size_t ROWS = 1000;
    constexpr size_t COLS = 1000;
    std::vector<std::vector<double>> matrix(ROWS, std::vector<double>(COLS, 1.0));

    auto process_matrix = [&matrix](size_t row_begin, size_t row_end) {
        double sum = 0;
        for(size_t r = row_begin; r < row_end; ++r) {
            for(size_t c = 0; c < COLS; ++c) {
                sum += matrix[r][c] * std::sqrt(matrix[r][c] + 1);
            }
        }
        return sum;
        };

    // STRATEGY 1: Parallelize only OUTER loop (usually best!)
    {
        size_t num_threads = std::thread::hardware_concurrency();
        size_t rows_per_thread = ROWS / num_threads;

        auto start = std::chrono::steady_clock::now();

        std::vector<std::future<double>> futures;
        for(size_t t = 0; t < num_threads; ++t) {
            size_t begin = t * rows_per_thread;
            size_t end = (t == num_threads - 1) ? ROWS : begin + rows_per_thread;

            futures.push_back(std::async(std::launch::async,
                                         [process_matrix, begin, end]() {
                                             return process_matrix(begin, end);
                                         }));
        }

        double total = 0;
        for(auto& f : futures) total += f.get();

        auto end_time = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();

        std::cout << "  Outer parallel: " << ms << "ms, result=" << total << std::endl;
    }

    std::cout << R"(
    NESTED PARALLELISM GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ 1. PARALLELIZE ONLY ONE LEVEL (usually outer)       │
    │    The outer level usually provides enough work     │
    │                                                     │
    │ 2. IF YOU MUST NEST:                                │
    │    Use a global thread limiter                      │
    │    Total threads ≤ hardware_concurrency            │
    │                                                     │
    │ 3. PREFER PARALLEL ALGORITHMS:                      │
    │    std::for_each(par, ...) handles nesting          │
    │    intelligently                                    │
    │                                                     │
    │ 4. MEASURE!                                         │
    │    Nested parallelism often HURTS performance       │
    │    Don't assume more threads = faster              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}