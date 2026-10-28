/*
 * ============================================================================
 * CONCURRENCY PITFALLS - THE HALL OF SHAME
 * ============================================================================
 *
 * A collection of the most common (and devastating) mistakes
 * in concurrent programming.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include "concurrency_header.h"

void Pitfall1_ForgottenJoin() {
    std::cout << "=== Pitfall 1: Forgetting join() ===" << std::endl;

    /*
     * THE BUG:
     * ================================================================
     * If a std::thread object is destroyed without join() or detach(),
     * std::terminate() is called → program CRASHES.
     */

    std::cout << R"(
    WRONG:
    ┌─────────────────────────────────────────────────────┐
    │ void func() {                                       │
    │     std::thread t(worker);   // Thread starts       │
    │     // ... something throws ...                    │
    │ }  // t destroyed without join → TERMINATE!        │
    │                                                     │
    │ FIX: Use std::jthread (C++20) or RAII wrapper      │
    └─────────────────────────────────────────────────────┘

    RIGHT (C++20):
    ┌─────────────────────────────────────────────────────┐
    │ void func() {                                       │
    │     std::jthread t(worker);  // Auto-joins!         │
    │     // Exception? No problem - still joins         │
    │ }                                                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall2_DetachedThreadAccess() {
    std::cout << "\n=== Pitfall 2: Detached Thread Using Dead Resources ===" << std::endl;

    /*
     * THE BUG:
     * ================================================================
     * Detached thread continues running after the objects
     * it references are destroyed.
     */

    std::cout << R"(
    WRONG:
    ┌─────────────────────────────────────────────────────┐
    │ void func() {                                       │
    │     int local_var = 42;                            │
    │     std::thread t([&local_var]() {                 │
    │         // Uses local_var by reference             │
    │         std::this_thread::sleep_for(1s);          │
    │         use(local_var);  // local_var is DEAD!     │
    │     });                                            │
    │     t.detach();                                    │
    │ }  // local_var destroyed, thread still running     │
    │    // → UNDEFINED BEHAVIOR (crash, garbage, etc.)   │
    └─────────────────────────────────────────────────────┘

    FIX: Capture by value, or keep object alive:
    ┌─────────────────────────────────────────────────────┐
    │ void func() {                                       │
    │     int local_var = 42;                            │
    │     std::thread t([local_var]() {  // BY VALUE     │
    │         use(local_var);  // Safe: has own copy     │
    │     });                                            │
    │     t.detach();  // Now safe                        │
    │ }                                                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall3_LockScopeTooLarge() {
    std::cout << "\n=== Pitfall 3: Lock Scope Too Large ===" << std::endl;

    /*
     * THE BUG:
     * ================================================================
     * Holding a lock longer than necessary serializes everything.
     */

    std::mutex mtx;
    std::vector<int> data;

    // BAD: Lock held during expensive operation:
    auto bad_worker = [&mtx, &data]() {
        std::lock_guard<std::mutex> lock(mtx);
        // ALL of this is serialized, even the expensive computation!
        for(int i = 0; i < 1000000; ++i) {
            // Expensive computation INSIDE the lock:
            data.push_back(i * i);
        }
        };

    // GOOD: Minimal lock scope:
    auto good_worker = [&mtx, &data]() {
        // Compute OUTSIDE the lock:
        std::vector<int> local_result;
        local_result.reserve(1000000);
        for(int i = 0; i < 1000000; ++i) {
            local_result.push_back(i * i);
        }

        // Only lock for the quick update:
        std::lock_guard<std::mutex> lock(mtx);
        data.insert(data.end(), local_result.begin(), local_result.end());
        };

    std::cout << R"(
    RULE: Hold locks for the MINIMUM time necessary.
    ┌─────────────────────────────────────────────────────┐
    │ BAD:                                                │
    │   lock();                                           │
    │   expensive_computation();  // Serialized!         │
    │   update_data();                                   │
    │   unlock();                                         │
    │                                                     │
    │ GOOD:                                               │
    │   result = expensive_computation();  // Parallel!  │
    │   lock();                                           │
    │   update_data(result);  // Quick                    │
    │   unlock();                                         │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall4_DoubleCheckedLocking() {
    std::cout << "\n=== Pitfall 4: Double-Checked Locking (Subtle) ===" << std::endl;

    /*
     * THE BUG (famous!):
     * ================================================================
     * A "clever" optimization that's actually broken.
     */

    std::cout << R"(
    THE BROKEN PATTERN:
    ┌─────────────────────────────────────────────────────┐
    │ if (instance == nullptr) {          // Check 1      │
    │     lock();                                          │
    │     if (instance == nullptr) {      // Check 2      │
    │         instance = new Singleton();                  │
    │     }                                                │
    │     unlock();                                        │
    │ }                                                    │
    │                                                      │
    │ PROBLEM: Between "new Singleton()" and assigning    │
        │ to instance, another thread might see a non - null    │
        │ pointer to a PARTIALLY CONSTRUCTED object!│
        │                                                      │
        │(Instruction reordering by CPU / compiler)            │
        │                                                      │
        │ FIX : Use std::call_once, or C++11 magic statics     │
        └─────────────────────────────────────────────────────┘

        THE CORRECT SOLUTIONS :
    ┌─────────────────────────────────────────────────────┐
        │ // C++11 static local (thread-safe by standard):   │
        │ Singleton & get() {
        │
            │     static Singleton instance;  // Magic static!    │
        │     return instance;                                │
            │
    }                                                    │
        │                                                      │
            │ // Or std::call_once:                              │
            │ static std::once_flag flag;                         │
            │ std::call_once(flag, []() {                         │
                             │     instance = new Singleton();                     │
                             │ });                                                  │
            └─────────────────────────────────────────────────────┘
            )" << std::endl;
}

void Pitfall5_RacingOnDestroy() {
    std::cout << "\n=== Pitfall 5: Using Destroyed Objects ===" << std::endl;

    std::cout << R"(
    SCENARIO: Thread outlives the object it references
    ┌─────────────────────────────────────────────────────┐
    │ class Handler {                                     │
    │     std::thread worker;                             │
    │     SomeData* data;                                 │
    │                                                      │
    │     void start() {                                  │
    │         worker = std::thread([this]() {             │
    │             while (running) {                       │
    │                 process(data);  // 'this' might     │
    │             }              // be dead!             │
    │         });                                          │
    │     }                                                │
    │                                                      │
    │     ~Handler() {                                     │
    │         // If we forget to stop/join worker:        │
    │         // 'this' and 'data' are destroyed          │
    │         // while thread still uses them!           │
    │     }                                                │
    │ };                                                   │
    │                                                      │
    │ FIX: Always signal stop + join in destructor       │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}