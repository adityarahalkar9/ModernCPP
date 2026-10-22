/*
 * ============================================================================
 * FUTURE AND PROMISE - ONE-WAY VALUE TRANSMISSION
 * ============================================================================
 *
 * WHAT ARE THEY?
 * ===============
 * - promise<T>: The WRITING end - where a value is set
 * - future<T>:  The READING end - where the value is retrieved
 *
 * They form a one-shot communication channel between threads.
 *
 * THE MAILBOX ANALOGY:
 * ====================
 * - promise = The slot where you put a letter
 * - future  = The slot where you check for the letter
 * - One person puts a letter in (set_value)
 * - Another person retrieves it (get)
 * - Once used, it's consumed (one-time use)
 *
 * WHY USE THEM?
 * =============
 * 1. Return values from threads (threads can't normally return)
 * 2. Exception propagation between threads
 * 3. Synchronization point (future.get() blocks until ready)
 *
 * ============================================================================
*/

import std;
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include "concurrency_header.h"

void FuturePromiseBasics() {
    std::cout << "=== Future and Promise Basics ===" << std::endl;

    /*
     * THE BASIC PATTERN:
     * ================================================================
     */

     // Create the promise/future pair
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    // Thread that will SET the value
    std::thread producer([&promise]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "  [Producer] computing value..." << std::endl;
        int result = 42;  // Pretend this is expensive computation

        std::cout << "  [Producer] setting value: " << result << std::endl;
        promise.set_value(result);  // Fulfill the promise
                         });

    // Main thread (consumer) waits for the value
    std::cout << "  [Consumer] waiting for value..." << std::endl;
    int value = future.get();  // BLOCKS until promise is fulfilled

    std::cout << "  [Consumer] got value: " << value << std::endl;

    producer.join();

    std::cout << R"(
    THE LIFECYCLE:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Create promise + get future from it              │
    │ 2. Give future to the waiting thread                │
    │ 3. Give promise to the producing thread            │
    │ 4. Producer computes + sets value on promise        │
    │ 5. Consumer's future.get() unblocks with value      │
    │                                                     │
    │ future.get() can be called ONLY ONCE!              │
    │ After get(), the future is exhausted.              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ExceptionPropagation() {
    std::cout << "\n=== Exception Propagation ===" << std::endl;

    /*
     * THE SUPERPOWER: Exceptions cross thread boundaries!
     * ================================================================
     */

    std::promise<double> promise;
    std::future<double> future = promise.get_future();

    std::thread worker([&promise]() {
        try {
            // Simulate an error:
            throw std::runtime_error("Something went wrong in worker thread!");
        }
        catch(...) {
            // Capture the exception and send it through the promise
            promise.set_exception(std::current_exception());
        }
                       });

    worker.join();

    // On the consumer side:
    try {
        double result = future.get();  // This re-throws the exception!
        std::cout << "  Got result: " << result << std::endl;
    }
    catch(const std::exception& e) {
        std::cout << "  [Consumer] caught exception from worker: "
            << e.what() << std::endl;
    }

    std::cout << R"(
    THE MAGIC:
    ┌─────────────────────────────────────────────────────┐
    │ WORKER THREAD:                                      │
    │   try {                                             │
    │       throw some_error();                           │
    │   } catch (...) {                                   │
    │       promise.set_exception(std::current_exception()); │
    │   }                                                 │
    │                                                     │
    │ MAIN THREAD:                                        │
    │   try {                                             │
    │       result = future.get();  // RE-THROWS here!    │
    │   } catch (const SomeError& e) {                    │
    │       // Handle as if it was thrown locally!         │
    │   }                                                 │
    │                                                     │
    │ The exception "travels" from worker to main!        │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ThreadReturnValue() {
    std::cout << "\n=== Getting Return Values from Threads ===" << std::endl;

    /*
     * THE PROBLEM: std::thread can't return values
     * ================================================================
     * std::thread's function must return void.
     * How do we get a result back?
     *
     * SOLUTION: Use promise/future
     */

     // Spawn multiple computing threads:
    std::vector<std::thread> threads;
    std::vector<std::future<int>> futures;

    for(int i = 1; i <= 5; ++i) {
        std::promise<int> promise;
        futures.push_back(promise.get_future());

        threads.emplace_back([i, p = std::move(promise)]() mutable {
            // Compute something (pretend it's expensive)
            int result = 1;
            for(int j = 1; j <= i; ++j) {
                result *= j;  // Compute i!
            }

            // Send result back:
            p.set_value(result);
                             });
    }

    // Collect results:
    std::cout << "  Factorials computed in parallel:" << std::endl;
    for(size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
        int factorial = futures[i].get();
        std::cout << "    " << (i + 1) << "! = " << factorial << std::endl;
    }
}