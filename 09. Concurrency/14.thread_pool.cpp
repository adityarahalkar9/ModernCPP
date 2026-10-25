/*
 * ============================================================================
 * THREAD POOL - PRODUCTION-QUALITY IMPLEMENTATION
 * ============================================================================
 *
 * WHY A THREAD POOL?
 * =================
 * Creating/destroying threads is expensive (~10-100μs each).
 * For many small tasks, the overhead dominates actual work.
 *
 * A pool creates threads ONCE and reuses them:
 *   - No creation cost per task
 *   - Bounded parallelism (don't oversubscribe CPU)
 *   - Natural load balancing
 *
 * DESIGN DECISIONS:
 * =================
 * 1. How many threads? (usually hardware_concurrency)
 * 2. Queue type? (FIFO is standard)
 * 3. Task submission? (futures for return values)
 * 4. Shutdown behavior? (drain queue vs abandon)
 *

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
#include <chrono>
#include "concurrency_header.h"

class ThreadPool2 {
public:
    explicit ThreadPool2(size_t num_threads)
        : stop_(false), active_tasks_(0) {

        if(num_threads == 0) {
            num_threads = std::thread::hardware_concurrency();
        }

        for(size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    ~ThreadPool2() {
        shutdown();
    }

    // Submit a task and get a future for its result
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>> {

        using return_type = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;

        // Wrap in shared_ptr so we can put it in a std::function
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(queue_mutex_);

            if(stop_) {
                throw std::runtime_error("Cannot submit to stopped pool");
            }

            tasks_.emplace([task]() { (*task)(); });
        }

        condition_.notify_one();
        return result;
    }

    // Graceful shutdown: wait for all queued tasks to complete
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if(stop_) return;
            stop_ = true;
        }

        condition_.notify_all();

        for(auto& worker : workers_) {
            if(worker.joinable()) {
                worker.join();
            }
        }
    }

    // Wait until all tasks are done (both queue empty and workers idle)
    void wait_for_all() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        done_condition_.wait(lock, [this] {
            return tasks_.empty() && active_tasks_ == 0;
                             });
    }

    size_t pending_tasks() const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }

    size_t thread_count() const { return workers_.size(); }

private:
    void worker_loop() {
        while(true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(queue_mutex_);

                condition_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                                });

                if(stop_ && tasks_.empty()) {
                    return;  // Pool stopped and queue drained
                }

                if(!tasks_.empty()) {
                    task = std::move(tasks_.front());
                    tasks_.pop();
                    ++active_tasks_;
                }
            }

            if(task) {
                task();  // Execute outside the lock

                {
                    std::lock_guard<std::mutex> lock(queue_mutex_);
                    --active_tasks_;
                    if(tasks_.empty() && active_tasks_ == 0) {
                        done_condition_.notify_all();
                    }
                }
            }
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    mutable std::mutex queue_mutex_;
    std::condition_variable condition_;      // For workers waiting for tasks
    std::condition_variable done_condition_; // For wait_for_all()

    bool stop_;
    size_t active_tasks_;  // Tasks currently being executed
};

void ThreadPoolImplementation() {
    std::cout << "=== Production Thread Pool ===" << std::endl;

    ThreadPool2 pool(4);
    std::cout << "  Pool with " << pool.thread_count() << " threads" << std::endl;

    // Submit various tasks:
    std::vector<std::future<int>> results;

    // CPU-bound tasks:
    for(int i = 1; i <= 10; ++i) {
        results.push_back(pool.submit([i]() {
            // Simulate work
            int result = 0;
            for(int j = 0; j < i * 100000; ++j) {
                result += j % 7;
            }
            return result;
                                      }));
    }

    // I/O-bound task:
    auto io_result = pool.submit([]() -> std::string {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return "IO complete";
                                 });

    // Collect results:
    std::cout << "  CPU results: ";
    for(auto& f : results) {
        std::cout << f.get() << " ";
    }
    std::cout << std::endl;

    std::cout << "  IO result: " << io_result.get() << std::endl;

    // Wait for everything to complete:
    pool.wait_for_all();
    std::cout << "  All tasks completed!" << std::endl;

    std::cout << R"(
    THREAD POOL ANATOMY:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │  [Task Queue] ← submit() calls add here             │
    │       │                                             │
    │       ▼                                             │
    │  ┌─────────┐                                         │
    │  │ Worker 1 │ ← takes task, executes, takes next     │
    │  ├─────────┤                                         │
    │  │ Worker 2 │ ← takes task, executes, takes next     │
    │  ├─────────┤                                         │
    │  │ Worker 3 │ ← ...                                  │
    │  ├─────────┤                                         │
    │  │ Worker 4 │ ← ...                                  │
    │  └─────────┘                                         │
    │       │                                             │
    │       ▼                                             │
    │  [Futures] → results returned to callers            │
    │                                                     │
    │ KEY COMPONENTS:                                     │
    │ 1. Task queue (protected by mutex)                 │
    │ 2. Condition variable (workers sleep when no work) │
    │ 3. Stop flag (for graceful shutdown)               │
    │ 4. Active task counter (for wait_for_all)          │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}