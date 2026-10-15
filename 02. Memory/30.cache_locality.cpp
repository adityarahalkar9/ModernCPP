#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include "memory_header.h"

// Structure of Arrays vs Array of Structures
struct Particle {
    float x, y, z;        // Position
    float vx, vy, vz;     // Velocity
    float mass;           // Mass
    int id;               // ID
};

// SoA layout
struct ParticleSystem {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;
    std::vector<int> id;

    void resize(size_t n) {
        x.resize(n); y.resize(n); z.resize(n);
        vx.resize(n); vy.resize(n); vz.resize(n);
        mass.resize(n); id.resize(n);
    }
};

void SoAvsAoS() {
    std::cout << "=== SoA vs AoS ===" << std::endl;

    std::cout << "AoS (Array of Structures):" << std::endl;
    std::cout << "  struct Particle { x, y, z, vx, vy, vz, mass, id };" << std::endl;
    std::cout << "  std::vector<Particle> particles;" << std::endl;
    std::cout << std::endl;
    std::cout << "  Good for: accessing all fields of one particle" << std::endl;
    std::cout << "  Bad for: processing same field across many particles" << std::endl;

    std::cout << "\nSoA (Structure of Arrays):" << std::endl;
    std::cout << "  struct System { vector<float> x, y, z; ... };" << std::endl;
    std::cout << std::endl;
    std::cout << "  Good for: SIMD operations on single field" << std::endl;
    std::cout << "  Bad for: accessing all fields of one particle" << std::endl;
}

// Cache-friendly iteration
void CacheFriendlyIteration() {
    std::cout << "\n=== Cache-Friendly Iteration ===" << std::endl;

    std::vector<float> positions(10000);
    std::iota(positions.begin(), positions.end(), 0.0f);

    // GOOD: Sequential access (cache-friendly)
    auto start = std::chrono::high_resolution_clock::now();
    float sum1 = 0;
    for(size_t i = 0; i < positions.size(); ++i) {
        sum1 += positions[i];
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Sequential sum: " << sum1 << " in "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << std::endl;

    // BAD: Strided access (cache-unfriendly)
    start = std::chrono::high_resolution_clock::now();
    float sum2 = 0;
    for(size_t i = 0; i < positions.size(); i += 64) {  // Skip 64 elements
        sum2 += positions[i];
    }
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Strided sum: " << sum2 << " in "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << std::endl;

    std::cout << "\nSequential access is much faster!" << std::endl;
}

// False sharing demonstration
struct alignas(64) PaddedCounter {
    std::atomic<int> value{0};
    char padding[64 - sizeof(std::atomic<int>)];
};

struct UnpaddedCounter {
    std::atomic<int> value{0};
};

void FalseSharingDemo() {
    std::cout << "\n=== False Sharing ===" << std::endl;

    std::cout << "Problem: Multiple threads writing to variables" << std::endl;
    std::cout << "that share the same cache line" << std::endl;
    std::cout << std::endl;
    std::cout << "CPU cache works in 64-byte lines" << std::endl;
    std::cout << "If two threads write different variables on same line:" << std::endl;
    std::cout << "  - Cache line bounces between cores" << std::endl;
    std::cout << "  - Massive performance loss" << std::endl;

    std::cout << "\nSolution: alignas(64) to separate cache lines" << std::endl;

    std::cout << "sizeof(UnpaddedCounter): " << sizeof(UnpaddedCounter) << std::endl;
    std::cout << "sizeof(PaddedCounter): " << sizeof(PaddedCounter) << std::endl;

    std::cout << "\nWith padding, each counter gets its own cache line" << std::endl;
}

// Memory prefetching
void PrefetchingDemo() {
    std::cout << "\n=== Memory Prefetching ===" << std::endl;

    std::cout << "CPU automatically prefetches sequential memory" << std::endl;
    std::cout << "Pattern access defeats prefetcher" << std::endl;

    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 0);

    // Sequential access (prefetcher friendly)
    auto process_sequential = [&data]() {
        long long sum = 0;
        for(size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
        return sum;
        };

    // Random access (defeats prefetcher)
    std::vector<size_t> indices(data.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{});

    auto process_random = [&data, &indices]() {
        long long sum = 0;
        for(size_t idx : indices) {
            sum += data[idx];
        }
        return sum;
        };

    auto start = std::chrono::high_resolution_clock::now();
    auto seq_result = process_sequential();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Sequential: " << seq_result << " in "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " us" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto rand_result = process_random();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Random: " << rand_result << " in "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " us" << std::endl;

    std::cout << "\nRandom access is 10-100x slower!" << std::endl;
}

// Data layout optimization
void DataLayoutOptimization() {
    std::cout << "\n=== Data Layout Optimization ===" << std::endl;

    std::cout << "1. Order struct members by size (largest first)" << std::endl;
    std::cout << "2. Group related data together" << std::endl;
    std::cout << "3. Avoid mixing hot and cold data" << std::endl;
    std::cout << "4. Use separate arrays for different access patterns" << std::endl;

    // Example: game entity
    struct BadEntity {
        std::string name;      // Cold: rarely accessed
        int health;            // Hot: accessed every frame
        std::string description;  // Cold: almost never accessed
        float x, y, z;        // Hot: physics every frame
        std::vector<int> inventory;  // Cold: only when opening UI
    };

    // Better: split into hot and cold
    struct HotEntity {
        float x, y, z;
        int health;
    };

    struct ColdEntity {
        std::string name;
        std::string description;
        std::vector<int> inventory;
    };

    std::cout << "\nsizeof(BadEntity): " << sizeof(BadEntity) << std::endl;
    std::cout << "sizeof(HotEntity): " << sizeof(HotEntity) << std::endl;

    std::cout << "\nHot data is compact and cache-friendly" << std::endl;
    std::cout << "Cold data doesn't pollute cache" << std::endl;
}

// Memory access patterns
void AccessPatterns() {
    std::cout << "\n=== Memory Access Patterns ===" std::endl;

    std::cout << "Best to worst (for performance):" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Sequential (best)" << std::endl;
    std::cout << "   for (i = 0; i < n; ++i) a[i]..." << std::endl;
    std::cout << std::endl;
    std::cout << "2. Strided with small stride" << std::endl;
    std::cout << "   for (i = 0; i < n; i += 2) a[i]..." << std::endl;
    std::cout << std::endl;
    std::cout << "3. Blocked/tiled (good for 2D)" << std::endl;
    std::cout << "   for (block...) for (i in block) for (j in block)..." << std::endl;
    std::cout << std::endl;
    std::cout << "4. Random (worst)" << std::endl;
    std::cout << "   for (idx : shuffled_indices) a[idx]..." << std::endl;

    std::cout << "\nRules:" << std::endl;
    std::cout << "  - Access memory in order when possible" << std::endl;
    std::cout << "  - Use contiguous containers (vector)" << std::endl;
    std::cout << "  - Avoid linked lists for iteration" << std::endl;
    std::cout << "  - Consider SoA for bulk operations" << std::endl;
}