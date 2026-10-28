/*
 * ============================================================================
 * CACHE-CONSCIOUS PROGRAMMING - DESIGNING FOR THE MEMORY HIERARCHY
 * ============================================================================
 *
 * THE MEMORY HIERARCHY:
 * =====================
 *   Registers:     <1ns    (bytes)
 *   L1 Cache:      ~1ns    (32-64 KB)
 *   L2 Cache:      ~4ns    (256KB-1MB)
 *   L3 Cache:      ~10ns   (several MB, shared)
 *   Main Memory:   ~100ns  (GBs)
 *   Disk:          ~ms     (TBs)
 *
 * THE IMPLICATION:
 * ================
 * A cache miss (100ns) is 100x slower than a cache hit (1ns).
 * Your algorithm might be O(n) but if it's cache-unfriendly,
 * it's 100x slower in practice.
 *
 * GUIDELINES:
 * ===========
 * 1. Sequential access > random access
 * 2. Keep related data together (locality)
 * 3. Avoid pointer chasing (linked lists are cache-hostile)
 * 4. Use arrays when possible
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>
#include <list>
#include "parallelism_header.h"

void SequentialVsRandomAccess() {
    std::cout << "=== Sequential vs Random Access ===" << std::endl;

    /*
     * THE DIFFERENCE BETWEEN ACCESS PATTERNS:
     * ================================================================
     */

    constexpr size_t SIZE = 10'000'000;
    std::vector<int> data(SIZE);
    std::iota(data.begin(), data.end(), 0);

    // Create a random permutation for "random access":
    std::vector<size_t> random_order(SIZE);
    std::iota(random_order.begin(), random_order.end(), 0);
    std::shuffle(random_order.begin(), random_order.end(), std::mt19937{42});

    // SEQUENTIAL ACCESS (cache-friendly):
    auto start = std::chrono::steady_clock::now();
    volatile long long seq_sum = 0;
    for(size_t i = 0; i < SIZE; ++i) {
        seq_sum += data[i];  // Next element is likely in cache!
    }
    auto end = std::chrono::steady_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // RANDOM ACCESS (cache-hostile):
    start = std::chrono::steady_clock::now();
    volatile long long rand_sum = 0;
    for(size_t i = 0; i < SIZE; ++i) {
        rand_sum += data[random_order[i]];  // Each access is a cache miss!
    }
    end = std::chrono::steady_clock::now();
    auto rand_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Sequential: " << seq_ms << "ms" << std::endl;
    std::cout << "  Random:     " << rand_ms << "ms" << std::endl;
    std::cout << "  Random is " << (rand_ms / static_cast<double>(seq_ms)) << "x slower!" << std::endl;

    std::cout << R"(
    WHY THIS HAPPENS:
    ┌─────────────────────────────────────────────────────┐
    │ SEQUENTIAL:                                         │
    │   Access element i                                  │
    │   → CPU loads entire cache line (64 bytes)         │
    │   → Elements i+1, i+2, i+3... are NOW IN CACHE     │
    │   → Subsequent accesses are cache HITS (~1ns)       │
    │                                                     │
    │ RANDOM:                                             │
    │   Access element X (random location)                │
    │   → Cache miss (~100ns)                             │
    │   → Load cache line for X                           │
    │   → Next access is different location → miss again │
    │   → EVERY access is a cache miss!                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void ArrayVsLinkedList() {
    std::cout << "\n=== Array vs Linked List (Cache Perspective) ===" << std::endl;

    /*
     * ARRAYS ARE CACHE-FRIENDLY, LISTS ARE NOT:
     * ================================================================
     */

    constexpr size_t SIZE = 1'000'000;

    // Array (contiguous memory):
    std::vector<int> array_data(SIZE);
    std::iota(array_data.begin(), array_data.end(), 1);

    // Linked list (scattered in memory):
    std::list<int> list_data;
    for(int i = 1; i <= SIZE; ++i) {
        list_data.push_back(i);
    }

    // Sum the array:
    auto start = std::chrono::steady_clock::now();
    volatile long long array_sum = 0;
    for(int v : array_data) {
        array_sum += v;
    }
    auto end = std::chrono::steady_clock::now();
    auto array_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Sum the list:
    start = std::chrono::steady_clock::now();
    volatile long long list_sum = 0;
    for(int v : list_data) {
        list_sum += v;
    }
    end = std::chrono::steady_clock::now();
    auto list_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "  Array sum: " << array_ms << "ms" << std::endl;
    std::cout << "  List sum:  " << list_ms << "ms" << std::endl;
    std::cout << "  Array is " << (list_ms / static_cast<double>(array_ms)) << "x faster!" << std::endl;

    std::cout << R"(
    MEMORY LAYOUT:
    ┌─────────────────────────────────────────────────────┐
    │ ARRAY (contiguous):                                 │
    │ [1][2][3][4][5][6][7][8]...                         │
    │  ↑ All in one place → great cache locality!         │
    │                                                     │
    │ LINKED LIST (scattered):                            │
    │ [1]──→[2]──→[7]──→[3]──→[9]──→[4]...              │
    │         ↑ Nodes allocated wherever heap had space  │
    │           → each 'next' is a cache miss!            │
    │                                                     │
    │ PRACTICAL RULE:                                     │
    │ For traversal-heavy workloads, use arrays/vectors  │
    │ even if "algorithmically" a list seems equivalent   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void StructureOfArrays() {
    std::cout << "\n=== SoA vs AoS (Structure of Arrays) ===" << std::endl;

    /*
     * DATA LAYOUT MATTERS FOR PARALLELISM:
     * ================================================================
     */

     // AoS (Array of Structures) - natural but cache-unfriendly:
    struct Particle {
        float x, y, z;      // Position (12 bytes)
        float vx, vy, vz;   // Velocity (12 bytes)
        float mass;         // 4 bytes
        int id;             // 4 bytes
    };  // Total: 32 bytes per particle

    std::vector<Particle> particles_aos(1'000'000);

    // SoA (Structure of Arrays) - cache-friendly for specific operations:
    struct ParticleSoA {
        std::vector<float> x, y, z;
        std::vector<float> vx, vy, vz;
        std::vector<float> mass;
        std::vector<int> id;
    };

    ParticleSoA particles_soa;
    particles_soa.x.resize(1'000'000);
    // ... etc

    // Operation: Update all X positions
    // AoS: Must load entire 32-byte particle, use only 4 bytes (x)
    // SoA: Load 16 x-values per cache line (64 bytes / 4 bytes = 16)

    // Measure AoS update:
    auto start = std::chrono::steady_clock::now();
    float dt = 0.016f;
    for(auto& p : particles_aos) {
        p.x += p.vx * dt;  // Only using x and vx (8 of 32 bytes!)
    }
    auto end = std::chrono::steady_clock::now();
    auto aos_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Measure SoA update:
    start = std::chrono::steady_clock::now();
    for(size_t i = 0; i < particles_soa.x.size(); ++i) {
        particles_soa.x[i] += particles_soa.vx[i] * dt;
    }
    end = std::chrono::steady_clock::now();
    auto soa_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "  AoS update: " << aos_ms << "μs" << std::endl;
    std::cout << "  SoA update: " << soa_ms << "μs" << std::endl;

    std::cout << R"(
    AoS vs SoA CACHE USAGE:
    ┌─────────────────────────────────────────────────────┐
    │ AoS (Array of Structures):                         │
    │ [x,y,z,vx,vy,vz,m,id][x,y,z,vx,vy,vz,m,id][...]   │
    │  ↑ Loading a cache line gets 2 particles           │
    │    But we only need x and vx (25% of data)         │
    │    → 75% of cache bandwidth WASTED                 │
    │                                                     │
    │ SoA (Structure of Arrays):                         │
    │ x:  [x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,...]  │
    │ vx: [vx1,vx2,vx3,...]                              │
    │  ↑ Loading x cache line gets 16 values            │
    │    All of them are USED!                           │
    │    → 100% cache efficiency                         │
    │                                                     │
    │ WHEN TO USE SoA:                                   │
    │ • Same operation on many elements                  │
    │ • Only accessing a subset of fields                │
    │ • Performance-critical loops (games, physics)      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}