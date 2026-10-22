/*
 * ============================================================================
 * PRODUCER-CONSUMER PATTERN - THE CLASSIC CONCURRENT STRUCTURE
 * ============================================================================
 *
 * THE PATTERN:
 * ============
 * - Producers: Create items and put them in a buffer
 * - Consumers: Take items from the buffer and process them
 * - Buffer: Shared queue with limited capacity
 *
 * THE COORDINATION CHALLENGES:
 * ============================
 * 1. Consumer must wait when buffer is EMPTY
 * 2. Producer must wait when buffer is FULL (bounded case)
 * 3. Multiple producers/consumers must not corrupt the buffer
 * 4. Graceful shutdown when producers are done
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>
#include "concurrency_header.h"

void CompleteProducerConsumer() {
    std::cout << "=== Complete Producer-Consumer Implementation ===" << std::endl;

    /*
     * BOUNDED BUFFER with MULTIPLE producers and consumers:
     * ================================================================
     */

    template <typename T>
    class BoundedQueue {
    private:
        std::queue<T> queue_;
        mutable std::mutex mtx_;
        std::condition_variable not_empty_;  // Signaled when items added
        std::condition_variable not_full_;   // Signaled when space available
        size_t capacity_;
        bool closed_ = false;

    public:
        explicit BoundedQueue(size_t cap) : capacity_(cap) {}

        // Producer calls this - blocks if full
        bool push(T item) {
            std::unique_lock<std::mutex> lock(mtx_);

            // Wait until not full OR closed
            not_full_.wait(lock, [this] {
                return queue_.size() < capacity_ || closed_;
                           });

            if(closed_) {
                return false;  // Can't push to closed queue
            }

            queue_.push(std::move(item));
            not_empty_.notify_one();  // Tell one waiting consumer
            return true;
        }

        // Consumer calls this - blocks if empty
        // Returns false when queue is closed AND empty (no more items)
        bool pop(T& item) {
            std::unique_lock<std::mutex> lock(mtx_);

            // Wait until not empty OR (closed AND empty = done)
            not_empty_.wait(lock, [this] {
                return !queue_.empty() || (closed_ && queue_.empty());
                            });

            // If closed and empty, we're done
            if(queue_.empty() && closed_) {
                return false;
            }

            item = std::move(queue_.front());
            queue_.pop();
            not_full_.notify_one();  // Tell one waiting producer
            return true;
        }

        // Signal that no more items will be produced
        void close() {
            std::lock_guard<std::mutex> lock(mtx_);
            closed_ = true;
            not_empty_.notify_all();  // Wake ALL consumers
            not_full_.notify_all();   // Wake ALL producers
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(mtx_);
            return queue_.size();
        }
    };

    // Create a bounded queue with capacity 10:
    BoundedQueue<int> queue(10);

    // MULTIPLE PRODUCERS:
    auto producer = [&queue](int id, int num_items) {
        for(int i = 0; i < num_items; ++i) {
            int item = id * 1000 + i;
            bool success = queue.push(item);

            if(!success) {
                std::cout << "  Producer " << id << ": queue closed, stopping" << std::endl;
                return;
            }

            std::cout << "  [Producer " << id << "] produced " << item << std::endl;

            // Simulate variable production time:
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i % 5) * 5));
        }
        };

    // MULTIPLE CONSUMERS:
    auto consumer = [&queue](int id) {
        int items_consumed = 0;
        int total = 0;

        while(true) {
            int item;
            if(!queue.pop(item)) {
                // Queue closed and empty - we're done
                break;
            }

            ++items_consumed;
            total += item;

            std::cout << "  [Consumer " << id << "] consumed " << item << std::endl;

            // Simulate variable processing time:
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        std::cout << "  [Consumer " << id << "] done: consumed "
            << items_consumed << " items, total = " << total << std::endl;
        };

    // Launch 2 producers and 3 consumers:
    std::vector<std::thread> threads;

    threads.emplace_back(producer, 1, 10);  // Producer 1: 10 items
    threads.emplace_back(producer, 2, 8);   // Producer 2: 8 items

    for(int i = 0; i < 3; ++i) {
        threads.emplace_back(consumer, i);
    }

    // Wait for producers to finish:
    threads[0].join();
    threads[1].join();

    // All producers done - close the queue:
    queue.close();

    // Wait for consumers to finish:
    for(size_t i = 2; i < threads.size(); ++i) {
        threads[i].join();
    }

    std::cout << "\n  All producers done, queue closed, consumers finished!" << std::endl;
    std::cout << R"(
    THE COMPLETE FLOW:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Producers push items (block if full)             │
    │ 2. Consumers pop items (block if empty)             │
    │ 3. Producers finish → close() the queue             │
    │ 4. Consumers drain remaining items                  │
    │ 5. Consumers see "closed and empty" → exit          │
    │                                                     │
    │ KEY INVARIANTS:                                     │
    │ - Queue size never exceeds capacity                 │
    │ - Every produced item is consumed exactly once      │
    │ - No consumer waits forever after close()           │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}