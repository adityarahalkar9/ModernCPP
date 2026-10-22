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

