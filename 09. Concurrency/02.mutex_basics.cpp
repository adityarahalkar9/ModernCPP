/*
 * ============================================================================
 * MUTEX BASICS - PROTECTING SHARED DATA
 * ============================================================================
 *
 * WHAT IS A MUTEX?
 * =================
 * A mutex (MUTual EXclusion) is a lock that allows only ONE
 * thread to access a section of code at a time.
 *
 * THE BATHROOM ANALOGY:
 * =====================
 * - The bathroom = protected resource (shared data)
 * - The door lock = mutex
 * - Only ONE person can be inside at a time
 * - Others must wait outside (block)
 * - Person must unlock when done (release)
 *
 * WHAT IT PROTECTS:
 * =================
 * NOT just one variable - it protects an INVARIANT:
 * - A consistent state that must be maintained
 * - Could span multiple variables
 * - Example: "balance >= 0" must always be true
 *
 * THE RULE:
 * =========
 * If ANY thread accesses shared data while ANY other thread
 * might be modifying it, you need a mutex (or atomics).
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include "concurrency_header.h"

std::mutex print_mutex;  // Protects std::cout (shared resource!)

void MutexBasics() {
    std::cout << "=== Mutex Basics ===" << std::endl;

    /*
     * THE PROBLEM: std::cout is shared
     * ================================================================
     * Without protection, output from multiple threads interleaves
     * into garbage like "HelloThWeroerldld"
     */

     // BAD VERSION (commented out - demonstrates the problem):
    auto bad_printer = [](int id) {
        for(int i = 0; i < 3; ++i) {
            // Without mutex, this can interleave:
            std::cout << "Thread " << id << " message " << i << std::endl;
        }
        };

    // GOOD VERSION: with mutex protection
    auto good_printer = [](int id) {
        for(int i = 0; i < 3; ++i) {
            print_mutex.lock();   // Acquire the lock
            std::cout << "  Thread " << id << ": message " << i << std::endl;
            print_mutex.unlock(); // Release the lock
        }
        };

    // Create threads with protected output:
    std::vector<std::thread> threads;
    for(int i = 0; i < 3; ++i) {
        threads.emplace_back(good_printer, i);
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "  (Each message is complete - no interleaving!)" << std::endl;
}

void LockGuard() {
    std::cout << "\n=== std::lock_guard - The Safe Way ===" << std::endl;

    /*
     * THE PROBLEM WITH MANUAL lock/unlock:
     * ================================================================
     *
     * mutex.lock();
     * do_something();
     * if (error) {
     *     return;  // BUG! Mutex never unlocked!
     * }
     * do_more();
     * mutex.unlock();  // Never reached on early return!
     *
     * This causes DEADLOCK - thread holds lock forever.
     */

     /*
      * THE SOLUTION: RAII for mutexes
      * ================================================================
      * std::lock_guard automatically:
      * - Locks when constructed
      * - Unlocks when destroyed (goes out of scope)
      * - Works even with exceptions, early returns, etc.
      */

    int shared_counter = 0;
    std::mutex counter_mutex;

    auto safe_increment = [&shared_counter, &counter_mutex](int times) {
        for(int i = 0; i < times; ++i) {
            // lock_guard acquires in constructor
            std::lock_guard<std::mutex> lock(counter_mutex);

            // This entire section is protected:
            ++shared_counter;  // Now atomic from outside perspective

            // lock_guard releases in destructor (even if exception!)
        }
        };

    // Run 4 threads with safe increment:
    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(safe_increment, 10000);
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "  Expected: 40000" << std::endl;
    std::cout << "  Actual:   " << shared_counter << " (CORRECT!)" << std::endl;

    std::cout << R"(
    WHY lock_guard IS SUPERIOR:
    ┌─────────────────────────────────────────────────────┐
    │ Manual:                                            │
    │   mutex.lock();                                     │
    │   risky_code();  // What if this throws?           │
    │   mutex.unlock();  // Might never run!             │
    │                                                     │
    │ lock_guard:                                        │
    │   {                                                │
    │       std::lock_guard<std::mutex> lock(mutex);      │
    │       risky_code();  // Exception? No problem!     │
    │   }  // Destructor ALWAYS releases the lock         │
    │                                                     │
    │ EXCEPTION SAFE: GUARANTEED unlock                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void WhatMutexProtects() {
    std::cout << "\n=== Understanding WHAT a Mutex Protects ===" << std::endl;

    /*
     * A MUTEX PROTECTS AN INVARIANT, NOT A VARIABLE:
     * ================================================================
     */

    class BankAccount {
    private:
        mutable std::mutex mtx_;
        double balance_;
        int transaction_count_;

    public:
        explicit BankAccount(double initial)
            : balance_(initial), transaction_count_(0) {
        }

        // The mutex protects the RELATIONSHIP between
        // balance_ and transaction_count_
        void deposit(double amount) {
            std::lock_guard<std::mutex> lock(mtx_);
            balance_ += amount;           // These two operations
            ++transaction_count_;         // must be atomic TOGETHER
        }

        bool withdraw(double amount) {
            std::lock_guard<std::mutex> lock(mtx_);
            if(balance_ >= amount) {     // Check and modify must be
                balance_ -= amount;       // atomic TOGETHER
                ++transaction_count_;
                return true;
            }
            return false;
        }

        // Get a consistent snapshot:
        std::pair<double, int> get_status() const {
            std::lock_guard<std::mutex> lock(mtx_);
            return {balance_, transaction_count_};
        }
    };

    BankAccount account(1000.0);

    // Multiple threads performing transactions:
    auto worker = [&account](int id, int num_ops) {
        for(int i = 0; i < num_ops; ++i) {
            account.deposit(10.0);
            account.withdraw(5.0);
        }
        };

    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i, 100);
    }

    for(auto& t : threads) {
        t.join();
    }

    auto [balance, count] = account.get_status();
    std::cout << "  Final balance: $" << balance << std::endl;
    std::cout << "  Transactions: " << count << " (4 × 100 × 2 = 800)" << std::endl;

    std::cout << R"(
    THE INVARIANT BEING PROTECTED:
    ┌─────────────────────────────────────────────────────┐
    │ INVARIANT: "transaction_count_ accurately reflects   │
    │ the number of operations that modified balance_"     │
    │                                                     │
    │ WITHOUT the mutex:                                  │
    │   Thread A: balance += 10                            │
    │   Thread B: balance += 10                            │
    │   Thread A: count++ (but B's deposit already         │
    │              happened!)                             │
    │   Thread B: count++                                 │
    │   → count is now WRONG (might be 2 or 3)            │
    │                                                     │
    │ WITH the mutex:                                     │
    │   Each deposit+count pair is atomic                │
    │   INVARIANT maintained                             │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}