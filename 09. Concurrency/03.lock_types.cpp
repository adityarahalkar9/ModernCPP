/*
 * ============================================================================
 * LOCK TYPES - DIFFERENT TOOLS FOR DIFFERENT JOBS
 * ============================================================================
 *
 * THE LOCK FAMILY:
 * =================
 * 1. lock_guard:    Simple, fixed-scope, no flexibility
 * 2. unique_lock:   Flexible, can lock/unlock, movable
 * 3. shared_lock:   For read-mostly data (multiple readers)
 * 4. scoped_lock:   Lock multiple mutexes without deadlock
 *
 * WHEN TO USE EACH:
 * =================
 * lock_guard:   Default choice for simple cases (95% of the time)
 * unique_lock:  When you need flexibility (defer, timeout, condition vars)
 * shared_lock:  When reads dominate and writes are rare
 * scoped_lock:  When locking multiple mutexes
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include "concurrency_header.h"

void LockGuardVsUniqueLock() {
    std::cout << "=== lock_guard vs unique_lock ===" << std::endl;

    std::mutex mtx;

    /*
     * lock_guard: SIMPLE AND FAST
     * ================================================================
     * - Locks in constructor, unlocks in destructor
     * - CANNOT be unlocked manually
     * - CANNOT be moved
     * - Zero overhead
     */
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "  lock_guard: locked (will auto-unlock at scope end)" << std::endl;
        // Cannot do: lock.unlock(); - not available!
    }  // auto-unlocked here

    /*
     * unique_lock: FLEXIBLE
     * ================================================================
     * - Can lock/unlock multiple times
     * - Can be constructed without locking (deferred)
     * - Can be moved (transferred)
     * - Slightly more overhead
     * - REQUIRED for condition_variable
     */
    {
        std::unique_lock<std::mutex> lock(mtx);  // Locks immediately
        std::cout << "  unique_lock: locked" << std::endl;

        lock.unlock();  // Manual unlock!
        std::cout << "  unique_lock: manually unlocked" << std::endl;

        lock.lock();    // Lock again!
        std::cout << "  unique_lock: manually re-locked" << std::endl;
    }  // Auto-unlocks (if still locked)

    /*
     * DEFERRED LOCKING:
     * ================================================================
     */
    {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        // Not locked yet!
        std::cout << "  unique_lock (deferred): not locked yet" << std::endl;

        // ... do some non-critical work ...

        lock.lock();  // NOW lock it
        std::cout << "  unique_lock: locked when needed" << std::endl;
    }

    std::cout << R"(
    DECISION GUIDE:
    ┌─────────────────────────────────────────────────────┐
    │ Use lock_guard when:                               │
    │   • Simple lock/unlock pattern                      │
    │   • Lock for entire scope                          │
    │   • Performance matters (slightly faster)          │
    │   • THE DEFAULT CHOICE                            │
    │                                                     │
    │ Use unique_lock when:                              │
    │   • Need to unlock before scope ends                │
    │   • Need to lock/unlock multiple times              │
    │   • Using with condition_variable                  │
    │   • Need to transfer ownership (move)              │
    │   • Want deferred locking                          │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void SharedLock() {
    std::cout << "\n=== shared_lock (Read-Write Locks) ===" << std::endl;

    /*
     * THE READERS-WRITERS PROBLEM:
     * ================================================================
     * Some data is read MUCH more often than written.
     *
     * - Multiple readers can read SIMULTANEOUSLY (no conflict)
     * - But writers need EXCLUSIVE access
     *
     * std::shared_mutex allows this optimization!
     */

    class SharedData {
    private:
        mutable std::shared_mutex mtx_;
        std::vector<int> data_;

    public:
        SharedData() : data_(1000000, 42) {}

        // MULTIPLE READERS can call this simultaneously
        int read(size_t index) const {
            std::shared_lock<std::shared_mutex> lock(mtx_);
            return data_[index];
        }

        // Only ONE WRITER at a time (readers blocked)
        void write(size_t index, int value) {
            std::unique_lock<std::shared_mutex> lock(mtx_);
            data_[index] = value;
        }

        size_t size() const {
            std::shared_lock<std::shared_mutex> lock(mtx_);
            return data_.size();
        }
    };

    SharedData shared_data;

    // Spawn multiple readers (can run concurrently):
    std::vector<std::thread> readers;
    for(int i = 0; i < 4; ++i) {
        readers.emplace_back([&shared_data, i]() {
            int sum = 0;
            for(size_t j = 0; j < shared_data.size(); j += 100) {
                sum += shared_data.read(j);
            }
            std::cout << "  Reader " << i << ": sum = " << sum << std::endl;
                             });
    }

    // Spawn a writer:
    std::thread writer([&shared_data]() {
        for(int i = 0; i < 10; ++i) {
            shared_data.write(i, i * 100);
        }
        std::cout << "  Writer: done" << std::endl;
                       });

    for(auto& t : readers) t.join();
    writer.join();

    std::cout << R"(
    HOW IT WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ READING (shared_lock):                             │
    │   Thread 1: [READ READ READ READ]                   │
    │   Thread 2: [READ READ READ READ]  ← Simultaneous!  │
    │   Thread 3: [READ READ READ READ]                   │
    │                                                     │
    │ WRITING (unique_lock):                             │
    │   Thread 1: [WRITE WRITE WRITE]                     │
    │   Thread 2: [---wait---] [READ READ]                │
    │   Thread 3: [---wait---] [READ READ]                │
    │                                                     │
    │ PERFECT FOR: Configuration, caches, settings       │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ScopedLock() {
    std::cout << "\n=== scoped_lock - Deadlock-Free Multiple Locking ===" << std::endl;

    /*
     * THE MULTIPLE MUTEX PROBLEM:
     * ================================================================
     * Sometimes you need to lock TWO mutexes at once.
     * If done naively, you get DEADLOCK:
     *
     * Thread A: lock(mutex1); lock(mutex2);  // A has 1, wants 2
     * Thread B: lock(mutex2); lock(mutex1);  // B has 2, wants 1
     * → Both wait forever!
     */

    std::mutex mtx1, mtx2;

    // NAIVE VERSION (deadlock-prone):
    // std::thread a([&]() {
    //     mtx1.lock();
    //     mtx2.lock();  // Might deadlock!
    //     // ...
    //     mtx2.unlock();
    //     mtx1.unlock();
    // });

    // SAFE VERSION with scoped_lock:
    auto safe_worker = [&](int id) {
        // scoped_lock acquires ALL mutexes without deadlock risk
        std::scoped_lock lock(mtx1, mtx2);

        // Both mutexes are held - safe to access both resources
        std::cout << "  Worker " << id << ": holding both mutexes safely"
            << std::endl;
        };  // scoped_lock releases BOTH (in reverse order)

    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(safe_worker, i);
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << R"(
    HOW scoped_lock AVOIDS DEADLOCK:
    ┌─────────────────────────────────────────────────────┐
    │ It uses the "try and back off" strategy:           │
    │                                                     │
    │ 1. Try to lock mutex1                                │
    │ 2. Try to lock mutex2                                │
    │ 3. If step 2 fails (someone else has it):           │
    │    → Release mutex1                                  │
    │    → Wait a random amount                            │
    │    → Try again from step 1                          │
    │                                                     │
    │ This is similar to std::lock() algorithm but        │
    │ wrapped in RAII for exception safety                │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}