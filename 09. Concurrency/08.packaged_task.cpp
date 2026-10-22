/*
 * ============================================================================
 * PACKAGED_TASK - WRAPPING CALLABLES IN FUTURES
 * ============================================================================
 *
 * WHAT IS std::packaged_task?
 * ============================
 * A wrapper around a callable that connects its return value
 * to a future. It's the bridge between "regular function" and
 * "asynchronous result."
 *
 * THE PIPELINE ANALOGY:
 * =====================
 * - packaged_task = A machine on an assembly line
 * - You feed it input (operator())
 * - It produces output (future)
 * - The output can be collected elsewhere
 *
 * WHEN TO USE:
 * ============
 * - You have a function you want to run later
 * - You need the result as a future
 * - More control than std::async (you choose WHEN/WHERE to run)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include "concurrency_header.h"

void PackagedTaskBasics() {
    std::cout << "=== packaged_task Basics ===" << std::endl;

    /*
     * THE BASIC PATTERN:
     * ================================================================
     */

     // Wrap a lambda in a packaged_task
    std::packaged_task<int(int, int)> task(
        [](int a, int b) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return a + b;
        }
    );

    // Get the future BEFORE running the task
    std::future<int> result = task.get_future();

    // Run the task in a thread:
    std::thread worker(std::move(task), 40, 2);  // Move the task!

    // Get the result:
    int value = result.get();
    std::cout << "  Task result: " << value << std::endl;

    worker.join();

    std::cout << R"(
    THE RELATIONSHIP:
    ┌─────────────────────────────────────────────────────┐
    │ std::packaged_task<Signature>                       │
    │         │                                           │
    │         │ get_future()                              │
    │         ▼                                           │
    │ std::future<Result>                                 │
    │         │                                           │
    │         │ get()  (blocks until task completes)       │
    │         ▼                                           │
    │ Result value                                        │
    │                                                     │
    │ Compare with std::async:                           │
    │   async = "run this NOW, give me the result"       │
    │   packaged_task = "wrap this, I'll run it when     │
    │                     I choose, result comes via     │
    │                     future"                         │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void TaskScheduler() {
    std::cout << "\n=== Using packaged_task in a Task Queue ===" << std::endl;

    /*
     * THE PATTERN: Deferred execution with results
     * ================================================================
     * This is how many task schedulers and thread pools work.
     */

     // A queue of tasks:
    std::vector<std::packaged_task<int()>> task_queue;
    std::vector<std::future<int>> result_futures;

    // Submit tasks (they don't run yet):
    for(int i = 1; i <= 3; ++i) {
        std::packaged_task<int()> task([i]() {
            return i * i;  // Square
                                       });
        result_futures.push_back(task.get_future());
        task_queue.push_back(std::move(task));
    }

    // Later... execute all tasks:
    std::cout << "  Executing tasks from queue:" << std::endl;
    for(auto& task : task_queue) {
        task();  // Run the task (result goes to the future)
    }

    // Collect all results:
    std::cout << "  Results: ";
    for(auto& f : result_futures) {
        std::cout << f.get() << " ";
    }
    std::cout << std::endl;
}