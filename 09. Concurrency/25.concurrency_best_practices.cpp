/*
 * ============================================================================
 * CONCURRENCY BEST PRACTICES - THE MASTER GUIDE
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "concurrency_header.h"

void BestPracticesSummary() {
    std::cout << "=== Concurrency Best Practices ===" << std::endl;

    std::cout << R"(
    ╔═════════════════════════════════════════════════════╗
    ║      CONCURRENCY BEST PRACTICES CHEAT SHEET         ║
    ╚═════════════════════════════════════════════════════╝

    THREAD MANAGEMENT:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Prefer std::jthread over std::thread (C++20)     │
    │ 2. Always join (or detach with extreme care)       │
    │ 3. Use thread pools for many small tasks            │
    │ 4. Don't create more threads than cores             │
    │ 5. Keep tasks large enough (>100μs)                 │
    └─────────────────────────────────────────────────────┘

    DATA SHARING:
    ┌─────────────────────────────────────────────────────┐
    │ 1. DON'T SHARE if possible (thread-local, messages) │
    │ 2. If sharing, protect with mutex                   │
    │ 3. For simple values, use atomics                   │
    │ 4. Document thread-safety of your interfaces       │
    │ 5. Prefer immutable data (no locks needed)         │
    └─────────────────────────────────────────────────────┘

    LOCKING:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Use lock_guard/unique_lock (never manual lock)  │
    │ 2. Lock scope: MINIMAL                              │
    │ 3. Lock ordering: CONSISTENT (avoid deadlock)       │
    │ 4. Multiple locks: use std::scoped_lock             │
    │ 5. Never call unknown code while holding a lock     │
    └─────────────────────────────────────────────────────┘

    PERFORMANCE:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Measure before optimizing                        │
    │ 2. Use parallel algorithms (std::execution)         │
    │ 3. Avoid false sharing (align to cache lines)       │
    │ 4. Too many threads < fewer threads                 │
    │ 5. Amdahl's Law: sequential parts limit you         │
    └─────────────────────────────────────────────────────┘

    DEBUGGING:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Use ThreadSanitizer (-fsanitize=thread)          │
    │ 2. Race conditions often disappear with logging     │
    │ 3. Test with different thread counts                │
    │ 4. Concurrency bugs are non-deterministic           │
    │ 5. Consider bounded model checking for critical code│
    └─────────────────────────────────────────────────────┘

    DESIGN:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Message passing > shared state                   │
    │ 2. Higher-level abstractions (future, async)        │
    │ 3. Separate I/O threads from compute threads        │
    │ 4. Document your threading model                   │
    │ 5. Simple designs are correct designs              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void DecisionTree() {
    std::cout << "\n=== Decision Tree: Which Tool to Use ===" << std::endl;

    std::cout << R"(
    START: I need concurrent code...
    │
    ├─ Can I use parallel algorithms?
    │   └─ YES → std::sort(std::execution::par, ...)
    │            (easiest, often best)
    │
    ├─ Do I need a result from another thread?
    │   ├─ YES, just one result → std::async
    │   └─ YES, many results → std::future per task
    │
    ├─ Is it long-running background work?
    │   └─ YES → std::jthread + stop_token
    │
    ├─ Many small tasks?
    │   └─ YES → Thread pool
    │
    ├─ Producer/consumer pattern?
    │   └─ YES → Condition variable + queue
    │
    ├─ Many readers, rare writers?
    │   └─ YES → std::shared_mutex
    │
    └─ Simple shared counter/flag?
        └─ YES → std::atomic
)" << std::endl;
}