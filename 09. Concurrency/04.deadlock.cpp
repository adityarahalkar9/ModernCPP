/*
 * ============================================================================
 * DEADLOCK - THE KISS OF DEATH IN CONCURRENT PROGRAMMING
 * ============================================================================
 *
 * WHAT IS DEADLOCK?
 * =================
 * A situation where two or more threads are blocked forever,
 * each waiting for a resource held by another.
 *
 * THE DINING PHILOSOPHERS ANALOGY:
 * ================================
 * - 5 philosophers sit at a round table
 * - 5 chopsticks (one between each pair)
 * - A philosopher needs TWO chopsticks to eat
 * - If everyone grabs their LEFT chopstick simultaneously...
 * - Everyone waits for the RIGHT chopstick forever
 * - DEADLOCK! No one eats.
 *
 * THE FOUR CONDITIONS (ALL must be true):
 * =======================================
 * 1. MUTUAL EXCLUSION: Resources can't be shared
 * 2. HOLD AND WAIT: Thread holds one resource, waits for another
 * 3. NO PREEMPTION: Can't force-take resources
 * 4. CIRCULAR WAIT: A waits for B, B waits for A (cycle)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include "concurrency_header.h"

void DeadlockDemonstration() {
    std::cout << "=== Deadlock Demonstration ===" << std::endl;

    /*
     * THE CLASSIC DEADLOCK PATTERN:
     * ================================================================
     */

    std::mutex mutex_a;
    std::mutex mutex_b;

    // Thread 1: Locks A, then wants B
    auto thread1_func = [&mutex_a, &mutex_b]() {
        std::cout << "  T1: Locking A..." << std::endl;
        mutex_a.lock();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // Give T2 time to lock B

        std::cout << "  T1: Now wants B (held by T2)..." << std::endl;
        mutex_b.lock();  // BLOCKS FOREVER (in real scenario)

        std::cout << "  T1: Got both (never happens!)" << std::endl;
        mutex_b.unlock();
        mutex_a.unlock();
        };

    // Thread 2: Locks B, then wants A
    auto thread2_func = [&mutex_a, &mutex_b]() {
        std::cout << "  T2: Locking B..." << std::endl;
        mutex_b.lock();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // Give T1 time to lock A

        std::cout << "  T2: Now wants A (held by T1)..." << std::endl;
        mutex_a.lock();  // BLOCKS FOREVER

        std::cout << "  T2: Got both (never happens!)" << std::endl;
        mutex_a.unlock();
        mutex_b.unlock();
        };

    // Uncomment to see deadlock (program hangs):
    // std::thread t1(thread1_func);
    // std::thread t2(thread2_func);
    // t1.join();  // Never completes
    // t2.join();  // Never completes

    std::cout << R"(
    VISUALIZING THE DEADLOCK:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │   Thread 1              Thread 2                    │
    │   ┌──────┐              ┌──────┐                    │
    │   │      │──wants───────│      │                    │
    │   │  A   │   B          │  B   │                    │
    │   │      │              │      │───────wants         │
    │   └──────┘              └──────┘   A    │           │
    │      │                      ▲          │           │
    │      └──────────has──────────┼──────────┘           │
    │                             │                      │
    │                    holds    │                      │
    │                    ←────────┘                      │
    │                                                     │
    │   T1 holds A, waits for B                          │
    │   T2 holds B, waits for A                          │
    │   CIRCULAR WAIT → DEADLOCK!                        │
    │   Both wait FOREVER                                │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void DeadlockPrevention() {
    std::cout << "\n=== Deadlock Prevention Strategies ===" << std::endl;

    /*
     * STRATEGY 1: LOCK ORDERING (The Gold Standard)
     * ================================================================
     * Always acquire locks in the SAME ORDER everywhere.
     */
    {
        std::mutex mutex_a, mutex_b;

        // RULE: Always lock A before B
        auto safe_thread1 = [&mutex_a, &mutex_b]() {
            std::lock_guard<std::mutex> la(mutex_a);  // A first
            std::lock_guard<std::mutex> lb(mutex_b);  // Then B
            std::cout << "  T1: safely locked A then B" << std::endl;
            };

        auto safe_thread2 = [&mutex_a, &mutex_b]() {
            std::lock_guard<std::mutex> la(mutex_a);  // A first (SAME ORDER!)
            std::lock_guard<std::mutex> lb(mutex_b);  // Then B
            std::cout << "  T2: safely locked A then B" << std::endl;
            };

        std::thread t1(safe_thread1);
        std::thread t2(safe_thread2);
        t1.join();
        t2.join();
        std::cout << "  No deadlock - consistent ordering!" << std::endl;
    }

    /*
     * STRATEGY 2: scoped_lock (Automatic deadlock avoidance)
     * ================================================================
     */
    {
        std::mutex mutex_x, mutex_y;

        auto worker = [&mutex_x, &mutex_y](int id) {
            // scoped_lock handles the ordering internally
            std::scoped_lock both(mutex_x, mutex_y);
            std::cout << "  Worker " << id << ": got both locks safely" << std::endl;
            };

        std::vector<std::thread> threads;
        for(int i = 0; i < 3; ++i) {
            threads.emplace_back(worker, i);
        }
        for(auto& t : threads) t.join();
    }

    /*
     * STRATEGY 3: std::timed_mutex (Detect and recover)
     * ================================================================
     */
    {
        std::timed_mutex tmtx;

        auto try_worker = [&tmtx](int id) {
            std::unique_lock<std::timed_mutex> lock(tmtx, std::defer_lock);
            if(lock.try_lock_for(std::chrono::milliseconds(100))) {
                std::cout << "  Worker " << id << ": got lock within 100ms" << std::endl;
            }
            else {
                std::cout << "  Worker " << id << ": TIMEOUT - giving up" << std::endl;
            }
            };

        std::thread tw1(try_worker, 1);
        std::thread tw2(try_worker, 2);
        tw1.join();
        tw2.join();
    }

    std::cout << R"(
    PREVENTION STRATEGIES SUMMARY:
    ┌─────────────────────────────────────────────────────┐
    │ 1. LOCK ORDERING (best):                           │
    │    • Establish a global order (address, hierarchy)  │
    │    • ALL code must follow it                       │
    │    • Simple and effective                          │
    │                                                     │
    │ 2. std::scoped_lock (easiest):                     │
    │    • For locking multiple mutexes                   │
    │    • Handles ordering automatically                │
    │                                                     │
    │ 3. try_lock with timeout:                          │
    │    • Detect potential deadlock                     │
    │    • Back off and retry                            │
    │    • Good for complex scenarios                    │
    │                                                     │
    │ 4. REDUCE LOCK SCOPE:                              │
    │    • Hold locks for minimum time                   │
    │    • Less chance of overlap                         │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void DiningPhilosophers() {
    std::cout << "\n=== The Dining Philosophers Problem ===" << std::endl;

    /*
     * THE CLASSIC THOUGHT EXPERIMENT:
     * ================================================================
     */

    const int NUM_PHILOSOPHERS = 5;
    std::vector<std::mutex> chopsticks(NUM_PHILOSOPHERS);

    // Without proper ordering, this deadlocks:
    // Everyone picks up left chopstick, waits for right forever

    // SOLUTION: Odd philosophers pick up in reverse order
    auto philosopher = [&](int id) {
        int left = id;
        int right = (id + 1) % NUM_PHILOSOPHERS;

        // To break the cycle, even philosophers go left-first,
        // odd philosophers go right-first
        int first, second;
        if(id % 2 == 0) {
            first = left; second = right;
        }
        else {
            first = right; second = left;
        }

        // Pick up first chopstick
        chopsticks[first].lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Pick up second chopstick
        chopsticks[second].lock();

        std::cout << "  Philosopher " << id << " is eating" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Put down chopsticks
        chopsticks[second].unlock();
        chopsticks[first].unlock();
        };

    std::vector<std::thread> philosophers;
    for(int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for(auto& p : philosophers) {
        p.join();
    }

    std::cout << "  All philosophers ate (no deadlock!)" << std::endl;
}