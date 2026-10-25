/*
 * ============================================================================
 * TASK GRANULARITY - FINDING THE RIGHT TASK SIZE
 * ============================================================================
 *
 * THE GOLDILOCKS PROBLEM:
 * =======================
 * Tasks can be:
 * - TOO SMALL: Overhead dominates (thread creation, scheduling)
 * - TOO LARGE: Poor load balancing, limited parallelism
 * - JUST RIGHT: Amortize overhead while staying parallel
 *
 * RULES OF THUMB:
 * ===============
 * - Task should take AT LEAST 100x the overhead of scheduling it
 * - For std::thread: overhead ~10-100μs → tasks should be >10ms
 * - For thread pool: overhead ~1μs → tasks should be >100μs
 * - For std::async: similar to thread pool
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <numeric>
#include "concurrency_header.h"

void GranularityExperiment() {
    std::cout << "=== Task Granularity Experiment ===" << std::endl;

    /*
     * MEASURING THE EFFECT OF TASK SIZE:
     * ================================================================
     */

    constexpr size_t TOTAL_WORK = 10'000'000;  // Total units of work

    // Simulate a unit of work (keep it simple for timing):
    auto do_work = [](size_t units) {
        volatile size_t result = 0;
        for(size_t i = 0; i < units; ++i) {
            result += i % 7;  // Trivial computation
        }
        return result;
        };

    // Test different chunk sizes:
    std::vector<size_t> chunk_sizes = {100, 1000, 10000, 100000, 1000000};

    std::cout << "  Chunk Size | Num Tasks | Time (ms) | Per-task (μs)" << std::endl;
    std::cout << "  " << std::string(60, '-') << std::endl;

    for(size_t chunk : chunk_sizes) {
        size_t num_tasks = TOTAL_WORK / chunk;

        auto start = std::chrono::high_resolution_clock::now();

        // Parallel execution with this chunk size:
        std::vector<std::future<size_t>> futures;

        for(size_t t = 0; t < num_tasks; ++t) {
            futures.push_back(std::async(std::launch::async,
                                         [do_work, chunk]() {
                                             return do_work(chunk);
                                         }));
        }

        // Wait for all:
        size_t total = 0;
        for(auto& f : futures) {
            total += f.get();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double per_task_us = (ms * 1000.0) / num_tasks;

        std::cout << "  " << std::setw(10) << chunk
            << " | " << std::setw(9) << num_tasks
            << " | " << std::setw(9) << ms
            << " | " << std::setw(11) << std::fixed << std::setprecision(1) << per_task_us
            << std::endl;
    }

    std::cout << R"(
    EXPECTED PATTERN:
    ┌─────────────────────────────────────────────────────┐
    │ TOO SMALL (100-1000):                              │
    │   • Overhead of launching threads dominates         │
    │   • Terrible performance                           │
    │                                                     │
    │ SWEET SPOT (10000-100000):                         │
    │   • Good parallelism                               │
    │   • Overhead amortized                             │
    │   • Best performance                               │
    │                                                     │
    │ TOO LARGE (1000000):                               │
    │   • Might not have enough tasks for all cores      │
    │   • Load imbalance if tasks vary in size           │
    └─────────────────────────────────────────────────────┘

    GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ Task duration should be:                            │
    │   • > 100μs for thread pool tasks                   │
    │   • > 1ms for std::thread                           │
    │   • > 10μs for std::async                           │
    │                                                     │
    │ If tasks are too small:                             │
    │   → BATCH them together                             │
    │   → Use parallel algorithms instead                │
    │   → Reduce number of tasks                          │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}