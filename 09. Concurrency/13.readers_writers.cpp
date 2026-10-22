/*
 * ============================================================================
 * READERS-WRITERS PATTERN - SHARED READ, EXCLUSIVE WRITE
 * ============================================================================
 *
 * THE PROBLEM:
 * ============
 * - Many threads want to READ shared data (safe to do concurrently)
 * - Few threads want to WRITE (need exclusive access)
 * - Reading is much more common than writing
 *
 * THE SOLUTION: std::shared_mutex (C++17)
 * ======================================
 * - shared_lock: Multiple readers can hold this simultaneously
 * - unique_lock: Only ONE writer, blocks all readers
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "concurrency_header.h"

void ReadersWritersPattern() {
    std::cout << "=== Readers-Writers Pattern ===" << std::endl;

    /*
     * SHARED DATA CONFIGURATION:
     * ================================================================
     */

    class Configuration {
    private:
        mutable std::shared_mutex mtx_;
        std::string server_address_;
        int port_;
        int timeout_ms_;
        int read_count_ = 0;
        int write_count_ = 0;

    public:
        Configuration()
            : server_address_("localhost"), port_(8080), timeout_ms_(5000) {
        }

        // READ operation - multiple threads can call simultaneously
        std::string get_config() const {
            std::shared_lock<std::shared_mutex> lock(mtx_);
            ++read_count_;
            return "addr=" + server_address_ +
                ", port=" + std::to_string(port_) +
                ", timeout=" + std::to_string(timeout_ms_) + "ms";
        }

        // WRITE operation - exclusive access required
        void update_config(const std::string& addr, int port, int timeout) {
            std::unique_lock<std::shared_mutex> lock(mtx_);
            ++write_count_;
            server_address_ = addr;
            port_ = port;
            timeout_ms_ = timeout;
        }

        std::pair<int, int> get_stats() const {
            std::shared_lock<std::shared_mutex> lock(mtx_);
            return {read_count_, write_count_};
        }
    };

    Configuration config;

    // READER threads (many, frequent reads):
    auto reader = [&config](int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 50);

        for(int i = 0; i < 20; ++i) {
            std::string value = config.get_config();
            // Note: Multiple readers can access simultaneously!

            std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        }
        };

    // WRITER thread (rare, exclusive writes):
    auto writer = [&config]() {
        std::vector<std::string> addresses = {
            "server1.example.com", "server2.example.com",
            "backup.example.com", "localhost"
        };

        for(int i = 0; i < 5; ++i) {
            config.update_config(addresses[i % addresses.size()],
                                 8080 + i, 3000 + i * 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        };

    // Launch 4 readers and 1 writer:
    std::vector<std::thread> threads;

    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(reader, i);
    }
    threads.emplace_back(writer);

    for(auto& t : threads) t.join();

    auto [reads, writes] = config.get_stats();
    std::cout << "  Total reads: " << reads << std::endl;
    std::cout << "  Total writes: " << writes << std::endl;
    std::cout << "  Final config: " << config.get_config() << std::endl;

    std::cout << R"(
    HOW SHARED_MUTEX WORKS:
    ┌─────────────────────────────────────────────────────┐
    │ READING (shared_lock):                             │
    │   Reader 1: [────────────────────────]              │
    │   Reader 2:   [────────────────────]                │
    │   Reader 3:     [────────────────]                  │
    │   ↑ All reading SIMULTANEOUSLY (safe!)              │
    │                                                     │
    │ WRITING (unique_lock):                             │
    │   Writer:   [────────]                              │
    │   Reader 1:    [──────]  ← waits for writer         │
    │   Reader 2:    [──────]  ← waits for writer         │
    │   ↑ Writer has EXCLUSIVE access                     │
    │                                                     │
    │ PERFECT FOR:                                        │
    │ • Configuration data (read often, write rarely)     │
    │ • Caches (read frequently, invalidate occasionally) │
    │ • Databases (many queries, rare updates)            │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}