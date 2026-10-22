/*
 * ============================================================================
 * TASK PARALLELISM - DIFFERENT OPERATIONS, POSSIBLY DIFFERENT DATA
 * ============================================================================
 *
 * WHAT IS TASK PARALLELISM?
 * =========================
 * Running DIFFERENT tasks simultaneously.
 * The parallelism comes from the OPERATIONS, not the data.
 *
 * CONTRAST WITH DATA PARALLELISM:
 * ===============================
 * Data parallel:  Same operation × different data
 * Task parallel:  Different operations (possibly × different data)
 *
 * EXAMPLE:
 * =========
 * While waiting for a file download:
 *   - Thread 1: Download file (I/O bound)
 *   - Thread 2: Update progress bar (UI)
 *   - Thread 3: Prepare disk space (I/O)
 *   - Thread 4: Play "loading" animation (UI)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include "parallelism_header.h"

void TaskParallelismDemo() {
    std::cout << "=== Task Parallelism ===" << std::endl;

    /*
     * RUNNING DIFFERENT TASKS SIMULTANEOUSLY:
     * ================================================================
     */

    auto start = std::chrono::steady_clock::now();

    // Task A: Simulated download (I/O bound)
    auto download_future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        return "file_data";
                                      });

    // Task B: Simulated processing (CPU bound)
    auto compute_future = std::async(std::launch::async, []() {
        long long result = 0;
        for(int i = 0; i < 100'000'000; ++i) {
            result += i;
        }
        return result;
                                     });

    // Task C: Simulated UI update
    auto ui_future = std::async(std::launch::async, []() {
        for(int i = 0; i < 10; ++i) {
            std::cout << "  [UI] Progress: " << (i * 10) << "%" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        return "UI_done";
                                });

    // Wait for all tasks:
    auto download_result = download_future.get();
    auto compute_result = compute_future.get();
    auto ui_result = ui_future.get();

    auto end = std::chrono::steady_clock::now();
    auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Download: " << download_result << std::endl;
    std::cout << "  Computed: " << compute_result << std::endl;
    std::cout << "  Total time: " << total_ms << "ms (if sequential: ~600ms)" << std::endl;
}

void PipelineOfTasks() {
    std::cout << "\n=== Task Pipeline (Each stage different) ===" << std::endl;

    /*
     * PIPELINE: Different stages, overlapping execution
     * ================================================================
     */

     // Stage 1: Fetch
    auto fetch = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return "raw_data";
        };

    // Stage 2: Process
    auto process = [](const std::string& input) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return "processed_" + input;
        };

    // Stage 3: Save
    auto save = [](const std::string& input) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return "saved_" + input;
        };

    // SEQUENTIAL pipeline:
    auto start = std::chrono::steady_clock::now();
    std::string result1 = save(process(fetch()));
    auto end = std::chrono::steady_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Sequential: " << seq_ms << "ms, result: " << result1 << std::endl;

    // For multiple items, we can overlap stages:
    auto pipelined = [&]() {
        std::vector<std::future<std::string>> results;

        for(int i = 0; i < 3; ++i) {
            results.push_back(std::async(std::launch::async,
                                         [fetch, process, save, i]() {
                                             auto data = fetch();
                                             auto processed = process(data);
                                             return save(processed);
                                         }));
        }

        std::vector<std::string> final_results;
        for(auto& f : results) {
            final_results.push_back(f.get());
        }
        return final_results;
        };

    start = std::chrono::steady_clock::now();
    auto results = pipelined();
    end = std::chrono::steady_clock::now();

    std::cout << "  Pipelined (3 items): "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << "ms" << std::endl;
}