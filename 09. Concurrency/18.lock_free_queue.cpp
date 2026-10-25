/*
 * ============================================================================
 * LOCK-FREE QUEUE - SINGLE-PRODUCER SINGLE-CONSUMER (SPSC)
 * ============================================================================
 *
 * WHAT IS LOCK-FREE?
 * ==================
 * Algorithms that don't use locks (mutexes).
 * Instead, they use atomic operations.
 *
 * BENEFITS:
 * =========
 * - No deadlock possible
 * - No priority inversion
 * - Better performance (usually)
 * - Works in contexts where locks are forbidden
 *
 * THIS EXAMPLE: SPSC Queue
 * ========================
 * Single Producer, Single Consumer.
 * The simplest useful lock-free structure.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include "concurrency_header.h"

void SPSCQueue() {
    std::cout << "=== Lock-Free SPSC Queue ===" << std::endl;

    /*
     * SINGLE-PRODUCER SINGLE-CONSUMER QUEUE:
     * ================================================================
     * Only ONE thread writes, only ONE thread reads.
     * This simplifies the synchronization enormously.
     */

    constexpr size_t QUEUE_SIZE = 1024;  // Must be power of 2
    std::vector<int> buffer(QUEUE_SIZE);

    // Each side has its own index:
    std::atomic<size_t> write_index{0};  // Only producer modifies
    std::atomic<size_t> read_index{0};   // Only consumer modifies

    // Producer writes at write_index, then advances it
    auto producer = [&buffer, &write_index, &read_index](int num_items) {
        for(int i = 0; i < num_items; ++i) {
            size_t current_write = write_index.load(std::memory_order_relaxed);
            size_t current_read = read_index.load(std::memory_order_acquire);

            // Check if queue is full:
            if(current_write - current_read >= QUEUE_SIZE) {
                // Queue full, wait (spin or yield)
                std::this_thread::yield();
                --i;  // Retry this item
                continue;
            }

            // Write the item:
            buffer[current_write % QUEUE_SIZE] = i;

            // Make the write visible to consumer:
            write_index.store(current_write + 1, std::memory_order_release);
        }
        };

    // Consumer reads at read_index, then advances it
    auto consumer = [&buffer, &write_index, &read_index](int expected_items,
                                                         std::atomic<int>& consumed_count) {
                                                             while(consumed_count.load() < expected_items) {
                                                                 size_t current_read = read_index.load(std::memory_order_relaxed);
                                                                 size_t current_write = write_index.load(std::memory_order_acquire);

                                                                 // Check if queue is empty:
                                                                 if(current_read == current_write) {
                                                                     // Queue empty, wait
                                                                     std::this_thread::yield();
                                                                     continue;
                                                                 }

                                                                 // Read the item:
                                                                 int item = buffer[current_read % QUEUE_SIZE];
                                                                 consumed_count.fetch_add(1, std::memory_order_relaxed);

                                                                 // Advance read index:
                                                                 read_index.store(current_read + 1, std::memory_order_release);
                                                             }
        };

    std::atomic<int> total_consumed{0};
    constexpr int NUM_ITEMS = 100000;

    std::thread prod(producer, NUM_ITEMS);
    std::thread cons(consumer, NUM_ITEMS, std::ref(total_consumed));

    prod.join();
    cons.join();

    std::cout << "  Produced and consumed " << total_consumed << " items" << std::endl;

    std::cout << R"(
    HOW LOCK-FREE SPSC WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ Producer:                                          │
    │   1. Read write_index (own index - relaxed)         │
    │   2. Read read_index (consumer's - acquire)         │
    │   3. Check space available                         │
    │   4. Write item at buffer[write % SIZE]            │
    │   5. store write_index+1 (release)                 │
    │                                                     │
    │ Consumer:                                          │
    │   1. Read read_index (own index - relaxed)          │
    │   2. Read write_index (producer's - acquire)        │
    │   3. Check items available                          │
    │   4. Read item at buffer[read % SIZE]               │
    │   5. store read_index+1 (release)                   │
    │                                                     │
    │ MEMORY ORDERING IS CRITICAL:                       │
    │ • Release on write makes prior writes visible       │
    │ • Acquire on read sees all released writes         │
    │ • Wrong ordering = subtle, rare bugs!              │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}