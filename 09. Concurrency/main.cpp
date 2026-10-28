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

    // --- Thread Safety (11) ---
    // ThreadSafetyLevels();
    // DesigningThreadSafeClass();

    // --- Producer-Consumer (12) ---
    // CompleteProducerConsumer();

    // --- Readers-Writers (13) ---
    // ReadersWritersPattern();

    // --- Thread Pool (14) ---
    // ThreadPoolImplementation();

    // --- jthread (15) ---
    // JThreadBasics();
    // JThreadWithStopToken();

    // --- Stop Token (16) ---
    // StopTokenComponents();
    // PatternsWithStopToken();

    // --- Atomic Patterns (17) ---
    // AtomicCounterPattern();
    // AtomicFlagPattern();
    // CASPattern();

    // --- Lock-Free Queue (18) ---
    // SPSCQueue();

    // --- Benchmarking (19) ---
    // BenchmarkBasics();
    // ScalingBenchmark();

    // --- Amdahl's Law (20) ---
    // AmdahlsLawExplained();
    // GustafsonsLaw();

    // --- Task Granularity (21) ---
    // GranularityExperiment();

    // --- Nested Parallelism (22) ---
    // ThreadExplosion();
    // PracticalNestedParallel();

    // --- Sync Patterns (23) ---
    // PatternCatalog();

    // --- Pitfalls (24) ---
    // Pitfall1_ForgottenJoin();
    // Pitfall2_DetachedThreadAccess();
    // Pitfall3_LockScopeTooLarge();

    // --- Best Practices (25) ---
    // BestPracticesSummary();
    // DecisionTree();

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "  End of Concurrency Examples" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}