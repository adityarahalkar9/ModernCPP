/*
 * ============================================================================
 * THREAD POOL - COMPLETE IMPLEMENTATION
 * ============================================================================
 *
 * WHAT IS A THREAD POOL?
 * =====================
 * A fixed set of worker threads that pull tasks from a queue.
 * Instead of creating/destroying threads per task (expensive!),
 * you reuse existing threads.
 *
 * WHY NOT CREATE THREADS PER TASK?
 * =================================
 * Creating a thread costs ~10-100 microseconds.
 * For 1 million tasks, that's 10-100 seconds JUST creating threads!
 * A pool creates threads ONCE, then reuses them.
 *
 * THE ARCHITECTURE:
 * =================
 *
 *   [Task Queue] ←── submit() from clients
 *        │
 *        ▼
 *   ┌─────────────────────────┐
 *   │ Worker 1  Worker 2  ... │  ← Fixed pool of threads
 *   └─────────────────────────┘
 *        │
 *        ▼
 *   [Results via futures]
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
#include <vector>
#include <memory>
#include <type_traits>
#include "parallelism_header.h"

class ThreadPool {
public:
    // Constructor: spawn the workers
    explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency())
        : stop_(false) {

        for(size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    // Destructor: stop everything
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_ = true;  // Signal workers to exit
        }
        condition_.notify_all();  // Wake all workers

        for(auto& worker : workers_) {
            if(worker.joinable()) {
                worker.join();
            }
        }
    }

    // Submit a task, get a future for its result
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>> {

        using return_type = std::invoke_result_t<F, Args...>;

        // Wrap the callable in a packaged_task (connects to future)
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        // Get the future BEFORE moving the task
        std::future<return_type> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if(stop_) {
                throw std::runtime_error("submit on stopped ThreadPool");
            }

            // Wrap in a type-erased function
            tasks_.emplace([task]() { (*task)(); });
        }

        condition_.notify_one();  // Wake up one worker
        return result;
    }

    size_t size() const { return workers_.size(); }

private:
    void worker_loop() {
        while(true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(queue_mutex_);

                // Wait for a task OR the stop signal
                condition_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                                });

                // If stopped AND no more tasks, exit
                if(stop_ && tasks_.empty()) {
                    return;
                }

                // Get a task from the queue
                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // Execute the task (outside the lock!)
            task();
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

void ThreadPoolDemo() {
    std::cout << "=== Thread Pool in Action ===" << std::endl;

    /*
     * USING THE POOL:
     * ================================================================
     */

    ThreadPool pool(4);  // 4 worker threads
    std::cout << "  Pool created with " << pool.size() << " workers" << std::endl;

    // Submit tasks that return values:
    std::vector<std::future<int>> results;

    for(int i = 1; i <= 8; ++i) {
        results.push_back(pool.submit([i]() {
            // Simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return i * i;  // Square
                                      }));
    }

    // Collect results:
    std::cout << "  Squares: ";
    for(auto& f : results) {
        std::cout << f.get() << " ";
    }
    std::cout << std::endl;

    std::cout << R"(
    THE FLOW:
    ┌─────────────────────────────────────────────────────┐
    │ Main Thread:                                        │
    │   future1 = pool.submit(task1);  // non-blocking    │
    │   future2 = pool.submit(task2);                     │
    │   ... do other work ...                             │
    │   result1 = future1.get();  // blocks if needed     │
    │                                                     │
    │ Worker Threads:                                     │
    │   loop {                                            │
    │       wait_for_task();                              │
    │       execute_task();  // sets promise              │
    │   }                                                 │
    └─────────────────────────────────────────────────────┘

    BENEFITS OVER RAW THREADS:
    ┌─────────────────────────────────────────────────────┐
    │ 1. No thread creation cost per task                 │
    │ 2. Bounded concurrency (don't oversubscribe)       │
    │ 3. Natural load balancing (workers pull work)      │
    │ 4. Easy to get results (futures)                   │
    │ 5. Reusable infrastructure                        │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}