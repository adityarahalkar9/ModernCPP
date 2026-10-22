/*
 * ============================================================================
 * LOAD BALANCING - DISTRIBUTING WORK FAIRLY
 * ============================================================================
 *
 * THE PROBLEM:
 * ============
 * If work isn't distributed evenly, some threads finish early
 * and sit idle while others still work.
 *
 * THE SOLUTION:
 * =============
 * - Static partitioning: Fixed chunks (simple, fast for uniform work)
 * - Dynamic scheduling: Threads grab work when ready (better for uneven work)
 * - Work stealing: Idle threads steal from busy threads' queues
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include "parallelism_header.h"

void StaticVsDynamic() {
    std::cout << "=== Static vs Dynamic Load Balancing ===" << std::endl;

    /*
     * STATIC PARTITIONING:
     * ================================================================
     * Split work equally beforehand. Problem: if work is UNEVEN,
     * some threads finish early.
     */

    std::vector<int> work_sizes = {1000, 2000, 100, 5000, 200};
    // Very uneven workload simulation

    // STATIC: split indices evenly (bad for this data)
    size_t total = 0;
    for(int w : work_sizes) total += w;
    size_t chunk = total / 4;  // 4 threads

    std::cout << "  Static chunks: " << chunk << " work units per thread" << std::endl;
    std::cout << "  (But actual work is unevenly distributed!)" << std::endl;

    /*
     * DYNAMIC SCHEDULING (with atomic counter):
     * ================================================================
     * Threads grab the next piece of work when they finish the current one.
     */

    std::atomic<size_t> next_item{0};

    auto dynamic_worker = [&next_item, &work_sizes](int thread_id) {
        int items_processed = 0;

        while(true) {
            // Atomically grab the next item index
            size_t i = next_item.fetch_add(1);

            if(i >= work_sizes.size()) {
                break;  // No more work
            }

            // Do the work for item i:
            int work = work_sizes[i];
            volatile int dummy = 0;
            for(int j = 0; j < work; ++j) {
                dummy += j;
            }
            ++items_processed;
        }

        std::cout << "  Thread " << thread_id << " processed "
            << items_processed << " items" << std::endl;
        };

    std::vector<std::thread> threads;
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(dynamic_worker, i);
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << R"(
    LOAD BALANCING STRATEGIES:
    ┌─────────────────────────────────────────────────────┐
    │ STATIC:                                            │
    │   • Pre-assign chunks to threads                    │
    │   • Fast to set up, no synchronization              │
    │   • BAD for uneven workloads                        │
    │   Use for: uniform work, small overhead matters     │
    │                                                     │
    │ DYNAMIC (atomic counter):                           │
    │   • Threads grab next piece when ready              │
    │   • Adapts to uneven workloads                      │
    │   • Small overhead from atomic operations           │
    │   Use for: irregular workloads                      │
    │                                                     │
    │ WORK STEALING:                                      │
    │   • Each thread has own queue                       │
    │   • Idle threads steal from busy threads            │
    │   • Best performance, most complex                  │
    │   Used in: TBB, Cilk, Java ForkJoinPool             │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}