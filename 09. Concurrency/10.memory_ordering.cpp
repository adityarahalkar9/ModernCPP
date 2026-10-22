/*
 * MEMORY ORDERING - HOW OPERATIONS ARE SEEN BY OTHER THREADS
 *
 * This is the most complex topic in concurrency.
 * Controls visibility of memory operations across threads.
 */

#include <atomic>
#include "concurrency_header.h"

void MemoryOrderingBasics() {
    std::cout << "=== Memory Ordering ===" << std::endl;
    std::cout << R"(
    THE PROBLEM: CPU REORDERING
    ┌─────────────────────────────────────────────────────┐
    │ Thread 1:                                          │
    │   data = 42;          // Step 1                    │
    │   ready = true;       // Step 2                    │
    │                                                     │
    │ Thread 2:                                          │
    │   if (ready)          // Might see ready=true      │
    │       use(data);      // But data might be old!    │
    │                                                     │
    │ CPU might reorder Step 1 and Step 2!               │
    └─────────────────────────────────────────────────────┘

    MEMORY ORDERS (weakest to strongest):
    ┌─────────────────────────────────────────────────────┐
    │ relaxed:  No ordering, just atomicity              │
    │ acquire:  No reads AFTER this can be reordered     │
    │ release:  No writes BEFORE this can be reordered   │
    │ acq_rel:  Both acquire and release                 │
    │ seq_cst:  Full ordering (DEFAULT - use this!)      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}