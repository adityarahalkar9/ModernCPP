import std;
#include "concurrency_header.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " ModernCPP - 09. Concurrency Complete" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // --- Thread Basics (01) ---
    // WhatIsAThread();
    // CreatingThreads();
    // JoinVsDetach();
    // MultipleThreads();
    // ThreadDangers();  // Shows race condition!

    // --- Mutex Basics (02) ---
    // MutexBasics();
    // LockGuard();
    // WhatMutexProtects();

    // --- Lock Types (03) ---
    // LockGuardVsUniqueLock();
    // SharedLock();
    // ScopedLock();

    // --- Deadlock (04) ---
    // DeadlockDemonstration();  // Visual only
    // DeadlockPrevention();
    // DiningPhilosophers();

    // --- Condition Variables (05) ---
    // ConditionVariableBasics();
    // ProducerConsumerQueue();
    // SpuriousWakeups();

    // --- Future/Promise (06) ---
    // FuturePromiseBasics();
    // ExceptionPropagation();
    // ThreadReturnValue();

    // --- Async (07) ---
    // AsyncBasics();
    // LaunchPolicies();
    // ParallelComputation();

    // --- Packaged Task (08) ---
    // PackagedTaskBasics();
    // TaskScheduler();

    // --- Atomics (09) ---
    // AtomicBasics();

    // --- Memory Ordering (10) ---
    // MemoryOrderingBasics();

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "  End of Concurrency Examples" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}