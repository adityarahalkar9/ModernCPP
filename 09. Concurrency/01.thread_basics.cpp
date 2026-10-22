/*
 * ============================================================================
 * THREAD BASICS - THE FOUNDATION OF CONCURRENT PROGRAMMING
 * ============================================================================
 *
 * WHAT IS A THREAD?
 * =================
 * A thread is an independent sequence of execution within a process.
 * Think of it as a "worker" that can perform tasks simultaneously
 * with other workers.
 *
 * THE RESTAURANT ANALOGY:
 * =======================
 * - Process = The restaurant (the whole building)
 * - Threads = The chefs working in the kitchen
 * - Multiple chefs can prepare different dishes simultaneously
 * - They share the same kitchen (memory)
 * - They need coordination to avoid collisions (synchronization)
 *
 * WHY MULTIPLE THREADS?
 * =====================
 * 1. PERFORMANCE: Use multiple CPU cores (parallelism)
 * 2. RESPONSIVENESS: Keep UI responsive while working in background
 * 3. I/O OVERLAP: While one thread waits for I/O, others compute
 * 4. MODELING: Some problems are naturally concurrent (servers)
 *
 * THE COST OF THREADS:
 * ====================
 * - Each thread has its own stack (~1-8 MB default)
 * - Context switching overhead (microseconds)
 * - Synchronization complexity (the hard part!)
 * - Potential for race conditions, deadlocks
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "concurrency_header.h"

void WhatIsAThread() {
    std::cout << "=== What is a Thread? ===" << std::endl;

    std::cout << R"(
    PROCESS vs THREAD:
    ┌─────────────────────────────────────────────────────┐
    │ PROCESS:                                           │
    │ ┌─────────────────────────────────────────────┐    │
    │ │  Code   Data   Heap   Stack                  │    │
    │ │  ┌────┐ ┌────┐ ┌────┐ ┌────┐              │    │
    │ │  │    │ │    │ │    │ │ T1 │  Thread 1     │    │
    │ │  │    │ │    │ │    │ ├────┤              │    │
    │ │  │    │ │    │ │    │ │ T2 │  Thread 2     │    │
    │ │  │    │ │    │ │    │ ├────┤              │    │
    │ │  │    │ │    │ │    │ │ T3 │  Thread 3     │    │
    │ │  └────┘ └────┘ └────┘ └────┘              │    │
    │ └─────────────────────────────────────────────┘    │
    │                                                     │
    │ • Threads SHARE: code, global data, heap           │
    │ • Threads HAVE OWN: stack, registers              │
    │ • This sharing is both powerful AND dangerous      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void CreatingThreads() {
    std::cout << "\n=== Creating Threads ===" << std::endl;

    /*
     * CREATING A THREAD:
     * ================================================================
     * A thread is constructed with a "callable" - something that
     * can be called with (). This can be:
     * - A regular function
     * - A lambda
     * - A function object
     * - A member function pointer
     */

     // Method 1: Regular function
     // (Thread starts running immediately when constructed!)
    std::thread t1(simple_function);

    // Method 2: Lambda
    std::thread t2([]() {
        std::cout << "  Lambda thread running, id: "
            << std::this_thread::get_id() << std::endl;
                   });

    // Method 3: Function with arguments
    std::thread t3(function_with_args, 42, "hello");

    // Method 4: Lambda with captures
    int value = 100;
    std::thread t4([value]() {
        std::cout << "  Captured lambda, value = " << value << std::endl;
                   });

    std::cout << "  Main thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << "  Hardware threads: " << std::thread::hardware_concurrency() << std::endl;

    /*
     * CRITICAL RULE: You MUST join or detach before the
     * std::thread object goes out of scope!
     * Otherwise: std::terminate() is called (program crashes).
     */
    t1.join();  // Wait for t1 to finish
    t2.join();  // Wait for t2 to finish
    t3.join();  // Wait for t3 to finish
    t4.join();  // Wait for t4 to finish

    std::cout << "  All threads completed!" << std::endl;
}

void JoinVsDetach() {
    std::cout << "\n=== join() vs detach() ===" << std::endl;

    /*
     * THE TWO WAYS TO "DISPOSE" OF A THREAD:
     * ================================================================
     *
     * join(): "Wait here until this thread finishes"
     *   - Blocks the calling thread
     *   - Thread's resources are cleaned up
     *   - You know exactly when it's done
     *   - Synchronous point in your program
     *
     * detach(): "Let it run on its own, I don't care when it finishes"
     *   - Returns immediately
     *   - Thread becomes "daemon" - runs independently
     *   - std::thread object no longer refers to it
     *   - DANGER: If main() exits while detached thread runs → CRASH
     */

     // JOIN example:
    std::cout << "  Before join: starting worker..." << std::endl;
    std::thread joinable_thread([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    [Worker thread] finished work" << std::endl;
                                });

    std::cout << "  Main waiting (join)..." << std::endl;
    joinable_thread.join();  // Blocks until thread finishes
    std::cout << "  After join: worker is done" << std::endl;

    // DETACH example (dangerous!):
    std::cout << "\n  Detaching a thread (fire and forget)..." << std::endl;
    std::thread detached_thread([]() {
        // This might run AFTER main() returns if we're not careful!
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "    [Detached thread] still running..." << std::endl;
                                });
    detached_thread.detach();  // Now we have no control over it

    std::cout << "  Main continues without waiting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Wait for detached thread

    std::cout << R"(
    WHEN TO USE WHICH:
    ┌─────────────────────────────────────────────────────┐
    │ join():                                            │
    │   • You need the thread's result                    │
    │   • You need to know when it's done                 │
    │   • Deterministic shutdown is required              │
    │   • 95% of cases should use this                    │
    │                                                     │
    │ detach():                                          │
    │   • Background logging/monitoring tasks            │
    │   • Thread runs "forever" (daemon-style)           │
    │   • You don't care about the result                │
    │   • VERY careful about lifetime management!        │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void MultipleThreads() {
    std::cout << "\n=== Multiple Threads Working Together ===" << std::endl;

    /*
     * THE PATTERN: Divide work among threads
     * ================================================================
     */

    std::vector<std::thread> workers;

    // Spawn 4 worker threads
    for(int i = 0; i < 4; ++i) {
        workers.emplace_back([i]() {
            std::cout << "  Worker " << i << " starting (thread id: "
                << std::this_thread::get_id() << ")" << std::endl;

            // Simulate some work
            int result = 0;
            for(int j = 0; j < 1000; ++j) {
                result += i * j;
            }

            std::cout << "  Worker " << i << " finished, result = "
                << result << std::endl;
                             });
    }

    // Wait for all workers
    for(auto& t : workers) {
        t.join();
    }

    std::cout << "  All workers completed!" << std::endl;
}

void ThreadDangers() {
    std::cout << "\n=== The Dangers (Why Synchronization Matters) ===" << std::endl;

    /*
     * RACE CONDITION DEMONSTRATION:
     * ================================================================
     * This is THE most common bug in concurrent programming.
     */

    int counter = 0;  // Shared by all threads - DANGER!

    auto increment_many_times = [&counter](int times) {
        for(int i = 0; i < times; ++i) {
            ++counter;  // NOT ATOMIC! This is actually:
            // 1. Read counter
            // 2. Increment
            // 3. Write back
            // Threads can interleave these steps!
        }
        };

    // Create threads that all increment the same counter
    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(increment_many_times, 100000);
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "  Expected: 4 × 100000 = 400000" << std::endl;
    std::cout << "  Actual:   " << counter << std::endl;
    std::cout << "  (Usually less due to lost updates!)" << std::endl;

    std::cout << R"(
    WHAT HAPPENED:
    ┌─────────────────────────────────────────────────────┐
    │ Thread A: read counter (5)                          │
    │ Thread B: read counter (5)  ← SAME VALUE!           │
    │ Thread A: write 6                                    │
    │ Thread B: write 6    ← OVERWRITES A's increment!     │
    │                                                     │
    │ One increment was LOST. Do this a million times     │
    │ and you lose thousands of updates.                  │
    │                                                     │
    │ THE FIX: Synchronization (mutexes, atomics)         │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}