/*
 * ============================================================================
 * CONDITION VARIABLES - EFFICIENT WAITING AND NOTIFICATION
 * ============================================================================
 *
 * WHAT IS A CONDITION VARIABLE?
 * =============================
 * A synchronization primitive that lets threads WAIT for a
 * condition to become true, and other threads to NOTIFY them
 * when it might have changed.
 *
 * THE WAITING ROOM ANALOGY:
 * =========================
 * - Patients (threads) wait in a waiting room
 * - Doctor's office (condition) might be available
 * - Nurse (notifying thread) calls patients when ready
 * - Patients don't waste resources by constantly checking
 *
 * WHY NOT JUST POLL?
 * ==================
 * BAD (busy waiting):
 *   while (!ready) {
 *       // Check again... and again... wasting CPU!
 *   }
 *
 * GOOD (condition variable):
 *   cv.wait(lock, []{ return ready; });  // Sleep until notified
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include "concurrency_header.h"

void ConditionVariableBasics() {
    std::cout << "=== Condition Variable Basics ===" << std::endl;

    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    // WAITER thread: waits for 'ready' to become true
    std::thread waiter([&]() {
        std::unique_lock<std::mutex> lock(mtx);  // MUST be unique_lock

        std::cout << "  Waiter: waiting for signal..." << std::endl;

        // Wait until 'ready' is true
        // This atomically: releases lock + sleeps
        cv.wait(lock, [&ready]() { return ready; });
        // When notified: reacquires lock + checks predicate + wakes up

        std::cout << "  Waiter: got the signal! Proceeding..." << std::endl;
                       });

    // Give waiter time to start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // NOTIFIER thread: sets ready and notifies
    std::thread notifier([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;  // Change the condition
            std::cout << "  Notifier: setting ready=true" << std::endl;
        }  // Release lock BEFORE notifying (important!)

        cv.notify_one();  // Wake up ONE waiting thread
        std::cout << "  Notifier: sent notification" << std::endl;
                         });

    waiter.join();
    notifier.join();

    std::cout << R"(
    HOW IT WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ WAITER:                                            │
    │   1. Acquire lock                                   │
    │   2. Check predicate (not ready? continue waiting)  │
    │   3. Release lock + SLEEP                           │
    │   4. [Notified] Wake up                             │
    │   5. Reacquire lock                                 │
    │   6. Check predicate again (spurious wakeups!)      │
    │   7. If true → proceed, else → wait again           │
    │                                                     │
    │ NOTIFIER:                                          │
    │   1. Acquire lock                                   │
    │   2. Change the condition                           │
    │   3. Release lock                                   │
    │   4. notify_one() or notify_all()                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ProducerConsumerQueue() {
    std::cout << "\n=== Producer-Consumer with Condition Variables ===" << std::endl;

    /*
     * THE CLASSIC PATTERN:
     * ================================================================
     * Producers add items, consumers remove them.
     * Condition variables coordinate:
     * - Consumers wait when queue is EMPTY
     * - Producers wait when queue is FULL (bounded buffer)
     */

    std::queue<int> queue;
    std::mutex queue_mutex;
    std::condition_variable cv_not_empty;  // Notified when items added
    std::condition_variable cv_not_full;   // Notified when space available
    constexpr size_t MAX_SIZE = 5;
    bool done = false;

    // CONSUMER: takes items from queue
    auto consumer = [&]() {
        while(true) {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait until queue has items OR we're done
            cv_not_empty.wait(lock, [&]() {
                return !queue.empty() || done;
                              });

            if(queue.empty() && done) {
                break;  // No more items coming
            }

            int item = queue.front();
            queue.pop();
            std::cout << "  [Consumer] took " << item
                << " (queue size: " << queue.size() << ")" << std::endl;

            lock.unlock();
            cv_not_full.notify_one();  // Tell producer there's space

            // Simulate processing time
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "  [Consumer] done" << std::endl;
        };

    // PRODUCER: adds items to queue
    auto producer = [&]() {
        for(int i = 1; i <= 10; ++i) {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait until queue has space
            cv_not_full.wait(lock, [&]() {
                return queue.size() < MAX_SIZE;
                             });

            queue.push(i);
            std::cout << "  [Producer] added " << i
                << " (queue size: " << queue.size() << ")" << std::endl;

            lock.unlock();
            cv_not_empty.notify_one();  // Tell consumer there are items

            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        // Signal that production is done
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            done = true;
        }
        cv_not_empty.notify_all();  // Wake all waiting consumers
        std::cout << "  [Producer] done" << std::endl;
        };

    std::thread prod_thread(producer);
    std::thread cons_thread(consumer);

    prod_thread.join();
    cons_thread.join();

    std::cout << "  All items produced and consumed!" << std::endl;
}

void SpuriousWakeups() {
    std::cout << "\n=== Spurious Wakeups (Why We Need Predicates) ===" << std::endl;

    /*
     * THE SUBTLE DANGER:
     * ================================================================
     * Condition variables can wake up WITHOUT being notified!
     * This is called a "spurious wakeup."
     *
     * WHY? It's an implementation detail of the OS/hardware.
     * The standard allows it for performance reasons.
     *
     * THE CONSEQUENCE:
     * If you wait without checking a condition, you might
     * proceed when you shouldn't!
     */

    std::cout << R"(
    WRONG (vulnerable to spurious wakeups):
    ┌─────────────────────────────────────────────────────┐
    │ cv.wait(lock);  // Might wake up for NO reason!      │
    │ // Code here might run when it shouldn't            │
    └─────────────────────────────────────────────────────┘

    RIGHT (protected by predicate):
    ┌─────────────────────────────────────────────────────┐
    │ cv.wait(lock, []{ return condition; });              │
    │ // Only proceeds if condition is TRUE               │
    │ // Even if woken spuriously, rechecks and sleeps    │
    └─────────────────────────────────────────────────────┘

    THE PREDICATE FORM IS EQUIVALENT TO:
    ┌─────────────────────────────────────────────────────┐
    │ while (!condition) {                               │
    │     cv.wait(lock);  // Wait, but recheck on wake    │
    │ }                                                   │
    └─────────────────────────────────────────────────────┘

    RULE: ALWAYS use the predicate version!
)" << std::endl;
}