#pragma once
import std;

// Simple functions for thread demos
void simple_function();
void function_with_args(int x, const std::string& s);

// File 01 - Thread Basics
void WhatIsAThread();
void CreatingThreads();
void JoinVsDetach();
void MultipleThreads();
void ThreadDangers();

// File 02 - Mutex Basics
void MutexBasics();
void LockGuard();
void WhatMutexProtects();

// File 03 - Lock Types
void LockGuardVsUniqueLock();
void SharedLock();
void ScopedLock();

// File 04 - Deadlock
void DeadlockDemonstration();
void DeadlockPrevention();
void DiningPhilosophers();

// File 05 - Condition Variables
void ConditionVariableBasics();
void ProducerConsumerQueue();
void SpuriousWakeups();

// File 06 - Future/Promise
void FuturePromiseBasics();
void ExceptionPropagation();
void ThreadReturnValue();

// File 07 - Async
void AsyncBasics();
void LaunchPolicies();
void ParallelComputation();

// File 08 - Packaged Task
void PackagedTaskBasics();
void TaskScheduler();

// File 09 - Atomics
void AtomicBasics();

// File 10 - Memory Ordering
void MemoryOrderingBasics();

// Simple functions for thread demos
void simple_function();
void function_with_args(int x, const std::string& s);

// File 01 - Thread Basics
void WhatIsAThread();
void CreatingThreads();
void JoinVsDetach();
void MultipleThreads();
void ThreadDangers();

// File 02 - Mutex Basics
void MutexBasics();
void LockGuard();
void WhatMutexProtects();

// File 03 - Lock Types
void LockGuardVsUniqueLock();
void SharedLock();
void ScopedLock();

// File 04 - Deadlock
void DeadlockDemonstration();
void DeadlockPrevention();
void DiningPhilosophers();

// File 05 - Condition Variables
void ConditionVariableBasics();
void ProducerConsumerQueue();
void SpuriousWakeups();

// File 06 - Future/Promise
void FuturePromiseBasics();
void ExceptionPropagation();
void ThreadReturnValue();

// File 07 - Async
void AsyncBasics();
void LaunchPolicies();
void ParallelComputation();

// File 08 - Packaged Task
void PackagedTaskBasics();
void TaskScheduler();

// File 09 - Atomics
void AtomicBasics();

// File 10 - Memory Ordering
void MemoryOrderingBasics();

// File 11 - Thread Safety
void ThreadSafetyLevels();
void DesigningThreadSafeClass();

// File 12 - Producer-Consumer
void CompleteProducerConsumer();

// File 13 - Readers-Writers
void ReadersWritersPattern();

// File 14 - Thread Pool
void ThreadPoolImplementation();

// File 15 - jthread (C++20)
void JThreadBasics();
void JThreadWithStopToken();

// File 16 - Stop Token
void StopTokenComponents();
void PatternsWithStopToken();

// File 17 - Atomic Patterns
void AtomicCounterPattern();
void AtomicFlagPattern();
void CASPattern();
void AtomicSharedPointer();

// File 18 - Lock-Free Queue
void SPSCQueue();

// File 19 - Benchmarking
void BenchmarkBasics();
void ScalingBenchmark();

// File 20 - Amdahl's Law
void AmdahlsLawExplained();
void GustafsonsLaw();

// File 21 - Task Granularity
void GranularityExperiment();

// File 22 - Nested Parallelism
void ThreadExplosion();
void PracticalNestedParallel();

// File 23 - Sync Patterns
void PatternCatalog();

// File 24 - Pitfalls
void Pitfall1_ForgottenJoin();
void Pitfall2_DetachedThreadAccess();
void Pitfall3_LockScopeTooLarge();
void Pitfall4_DoubleCheckedLocking();
void Pitfall5_RacingOnDestroy();

// File 25 - Best Practices
void BestPracticesSummary();
void DecisionTree();