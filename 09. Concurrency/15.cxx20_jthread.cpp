/*
 * ============================================================================
 * STD::JTHREAD (C++20) - THE IMPROVED THREAD
 * ============================================================================
 *
 * WHAT IS jthread?
 * ================
 * "Joining thread" - a std::thread that AUTOMATICALLY joins
 * in its destructor.
 *
 * THE PROBLEM WITH std::thread:
 * =============================
 * If you forget to join() or detach(), the destructor calls
 * std::terminate() and CRASHES your program!
 *
 * jthread fixes this:
 * - Destructor automatically joins
 * - No more "forgot to join" bugs
 * - Exception safety guaranteed
 *
 * BONUS: Built-in cancellation support!
 * =====================================
 * jthread carries a stop_token that can signal the thread to stop.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "concurrency_header.h"

void JThreadBasics() {
    std::cout << "=== std::jthread (C++20) ===" << std::endl;

    /*
     * THE PROBLEM WITH std::thread:
     * ================================================================
     */
    {
        // DANGEROUS VERSION:
        // {
        //     std::thread t([]() {
        //         std::this_thread::sleep_for(std::chrono::seconds(1));
        //     });
        //     // If exception happens here, or we forget join...
        //     // → std::terminate() → CRASH!
        //     t.join();  // Must remember this!
        // }
    }

    /*
     * THE SOLUTION WITH jthread:
     * ================================================================
     */
    {
        std::cout << "  Creating jthread (auto-joins on scope exit)..." << std::endl;

        std::jthread t([]() {
            std::cout << "  [jthread] working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "  [jthread] done" << std::endl;
                       });

        std::cout << "  Main continues..." << std::endl;
        // NO join() needed! Destructor handles it!
    }  // t automatically joins here

    std::cout << "  jthread completed (no explicit join needed!)" << std::endl;

    /*
     * EXCEPTION SAFETY:
     * ================================================================
     */
    {
        std::cout << "\n  Exception safety demonstration:" << std::endl;

        try {
            std::jthread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                           });

            // Simulate an exception:
            throw std::runtime_error("Something went wrong!");

            // Even with exception, jthread still joins in destructor!
        }
        catch(const std::exception& e) {
            std::cout << "  Caught: " << e.what() << std::endl;
            std::cout << "  (jthread still joined properly - no crash!)" << std::endl;
        }
    }

    std::cout << R"(
    COMPARISON:
    ┌─────────────────────────────────────────────────────┐
    │ std::thread:                                       │
    │   • Must call join() or detach()                    │
    │   • Forget → std::terminate() → CRASH              │
    │   • Not exception-safe                            │
    │                                                     │
    │ std::jthread (C++20):                              │
    │   • Auto-joins in destructor                        │
    │   • Exception-safe                                  │
    │   • Built-in stop_token support                     │
    │   • USE THIS INSTEAD!                              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void JThreadWithStopToken() {
    std::cout << "\n=== jthread + stop_token (Cancellation) ===" << std::endl;

    /*
     * COOPERATIVE CANCELLATION:
     * ================================================================
     * jthread carries a stop_token that the thread can check.
     * Call request_stop() to signal the thread to stop.
     */

     // Worker that checks for stop signal:
    std::jthread worker([](std::stop_token stoken) {
        std::cout << "  [Worker] starting, will check for stop signal" << std::endl;

        int iteration = 0;
        while(!stoken.stop_requested()) {
            // Do some work:
            std::cout << "  [Worker] iteration " << iteration << std::endl;
            ++iteration;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "  [Worker] received stop signal, cleaning up..." << std::endl;
        // Do any necessary cleanup here
                        });

    // Let it run for a bit:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Signal the worker to stop:
    std::cout << "\n  Main: requesting stop..." << std::endl;
    worker.request_stop();

    // jthread destructor waits for it to finish:
    std::cout << "  Main: waiting for worker to acknowledge..." << std::endl;
    // (destructor will join)

    std::cout << R"(
    HOW COOPERATIVE CANCELLATION WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ Main Thread:                 Worker Thread:          │
    │                              loop {                 │
    │                                if (stop_requested) │
    │ request_stop() ──────────────→   break;            │
    │                                do_work();          │
    │                              }                      │
    │                              cleanup();            │
    │                              return;               │
    │                                                     │
    │ (join in destructor) ←───────┘                      │
    │                                                     │
    │ KEY: The worker CHECKS the stop_token.              │
    │ It's COOPERATIVE, not forced.                      │
    │ Worker can do cleanup before exiting.              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}