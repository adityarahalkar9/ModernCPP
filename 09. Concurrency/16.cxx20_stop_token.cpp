/*
 * ============================================================================
 * STOP_TOKEN - COOPERATIVE CANCELLATION MECHANISM (C++20)
 * ============================================================================
 *
 * THE CANCELLATION PROBLEM:
 * =========================
 * How do you tell a thread to stop?
 *
 * BAD APPROACHES:
 * ===============
 * - Kill the thread: Dangerous! No cleanup, resources leak
 * - Shared flag + mutex: Works, but clunky
 * -pthread_cancel(): Not in C++ standard
 *
 * THE SOLUTION: stop_token
 * =======================
 * A token that can be checked by the worker.
 * Main thread signals "please stop" via stop_source.
 * Worker sees the signal and exits gracefully.
 *
 * THE THREE COMPONENTS:
 * =====================
 * - stop_source: Can request a stop
 * - stop_token: Can check if stop was requested
 * - stop_callback: Run code when stop is requested
 *
 * ============================================================================
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "concurrency_header.h"

void StopTokenComponents() {
    std::cout << "=== stop_source, stop_token, stop_callback ===" << std::endl;

    /*
     * THE THREE PIECES:
     * ================================================================
     */

     // 1. stop_source: The "control" end
    std::stop_source source;

    // 2. stop_token: The "listening" end
    std::stop_token token = source.get_token();

    // 3. stop_callback: React when stop is requested
    std::stop_callback callback(token, []() {
        std::cout << "  [Callback] Stop was requested!" << std::endl;
                                });

    // Initially not requested:
    std::cout << "  Stop requested? " << token.stop_requested() << std::endl;

    // Request the stop:
    std::cout << "  Requesting stop..." << std::endl;
    source.request_stop();

    // Now it's requested (callback already fired):
    std::cout << "  Stop requested? " << token.stop_requested() << std::endl;

    /*
     * MULTIPLE LISTENERS:
     * ================================================================
     */
    {
        std::stop_source src2;
        auto tok2 = src2.get_token();

        // Multiple workers can share the same token:
        auto worker = [tok2](int id) {
            while(!tok2.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            std::cout << "  Worker " << id << " stopping" << std::endl;
            };

        std::jthread t1(worker, 1);
        std::jthread t2(worker, 2);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        src2.request_stop();  // Both workers see this!
    }

    std::cout << R"(
    THE RELATIONSHIP:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │  stop_source ──get_token()──→ stop_token            │
    │      │                          │                   │
    │      │ request_stop()           │ stop_requested()  │
    │      └──────────────────────────→│                   │
    │                                 │                   │
    │  "Please stop"              "OK, stopping"          │
    │                                                     │
    │  stop_callback(token, fn)                          │
    │       └──→ called when stop_requested becomes true │
    │                                                     │
    │ ONE source can have MANY tokens/listeners           │
    │ All see the stop signal simultaneously             │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void PatternsWithStopToken() {
    std::cout << "\n=== Common Patterns with stop_token ===" << std::endl;

    /*
     * PATTERN 1: Worker loop with periodic check
     * ================================================================
     */
    {
        std::jthread worker([](std::stop_token st) {
            std::cout << "  Pattern 1: Loop with check" << std::endl;

            while(!st.stop_requested()) {
                // Do a unit of work
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            std::cout << "  Stopped gracefully" << std::endl;
                            });

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        worker.request_stop();
    }

    /*
     * PATTERN 2: Check during long operation
     * ================================================================
     */
    {
        std::jthread worker([](std::stop_token st) {
            std::cout << "  Pattern 2: Check during long operation" << std::endl;

            for(int i = 0; i < 100 && !st.stop_requested(); ++i) {
                // Long operation, but we check between steps
                volatile int dummy = i * i;
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }

            if(st.stop_requested()) {
                std::cout << "  Cancelled mid-operation, partial state is valid" << std::endl;
            }
                            });

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        worker.request_stop();
    }

    /*
     * PATTERN 3: Cleanup on stop
     * ================================================================
     */
    {
        std::jthread worker([](std::stop_token st) {
            std::cout << "  Pattern 3: Cleanup on stop" << std::endl;

            // Simulate acquiring a resource:
            std::cout << "  Acquiring resource..." << std::endl;

            while(!st.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            // Cleanup code ALWAYS runs:
            std::cout << "  Releasing resource (cleanup)..." << std::endl;
                            });

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        worker.request_stop();
    }

    std::cout << R"(
    WHEN TO USE stop_token:
    ┌─────────────────────────────────────────────────────┐
    │ • Long-running workers that need cancellation      │
    │ • Background tasks with graceful shutdown          │
    │ • Any jthread that might run "forever"             │
    │                                                     │
    │ DESIGN RULES:                                       │
    │ 1. Check stop_requested() frequently               │
    │ 2. Keep the gap between checks short               │
    │ 3. Always have cleanup after the loop              │
    │ 4. Stop is COOPERATIVE - worker decides when       │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}