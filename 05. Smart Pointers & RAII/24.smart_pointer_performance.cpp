import std;
#include "smart_pointers_header.h"

class PerfDemo {
public:
    PerfDemo() = default;
    ~PerfDemo() = default;

    int value = 0;
};

void UniquePtrOverhead() {
    std::cout << "=== unique_ptr Performance ===" << std::endl;

    std::cout << "unique_ptr has ZERO overhead:" << std::endl;
    std::cout << "  Same size as raw pointer" << std::endl;
    std::cout << "  Same speed as raw pointer" << std::endl;
    std::cout << "  Can be optimized away entirely" << std::endl;

    std::cout << "\nSize comparison:" << std::endl;
    std::cout << "  sizeof(void*): " << sizeof(void*) << std::endl;
    std::cout << "  sizeof(unique_ptr<T>): " << sizeof(std::unique_ptr<PerfDemo>) << std::endl;
}

void SharedPtrOverhead() {
    std::cout << "\n=== shared_ptr Performance ===" << std::endl;

    std::cout << "shared_ptr has REAL overhead:" << std::endl;
    std::cout << "  Larger (2 pointers)" << std::endl;
    std::cout << "  Atomic reference counting" << std::endl;
    std::cout << "  Heap allocation for control block" << std::endl;

    std::cout << "\nSize comparison:" << std::endl;
    std::cout << "  sizeof(void*): " << sizeof(void*) << std::endl;
    std::cout << "  sizeof(shared_ptr<T>): " << sizeof(std::shared_ptr<PerfDemo>) << std::endl;

    std::cout << "\nAllocation comparison:" << std::endl;
    std::cout << "  new T + shared_ptr: 2 allocations" << std::endl;
    std::cout << "  make_shared<T>: 1 allocation (better!)" << std::endl;
}

void BenchmarkCreation() {
    std::cout << "\n=== Creation Benchmark ===" << std::endl;

    constexpr int iterations = 1'000'000;

    // unique_ptr creation
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        auto p = std::make_unique<PerfDemo>();
        p->value = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto unique_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // shared_ptr creation
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        auto p = std::make_shared<PerfDemo>();
        p->value = i;
    }
    end = std::chrono::high_resolution_clock::now();
    auto shared_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Raw new/delete
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        auto* p = new PerfDemo();
        p->value = i;
        delete p;
    }
    end = std::chrono::high_resolution_clock::now();
    auto raw_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Results for " << iterations << " iterations:" << std::endl;
    std::cout << "  Raw new/delete: " << raw_time << " μs" << std::endl;
    std::cout << "  unique_ptr:     " << unique_time << " μs" << std::endl;
    std::cout << "  shared_ptr:     " << shared_time << " μs" << std::endl;

    std::cout << "\nunique_ptr ≈ raw pointer" << std::endl;
    std::cout << "shared_ptr has measurable overhead" << std::endl;
}

void BenchmarkCopying() {
    std::cout << "\n=== Copy Benchmark ===" << std::endl;

    constexpr int iterations = 1'000'000;

    auto shared = std::make_shared<PerfDemo>();

    // unique_ptr "copy" (move - very cheap)
    auto unique = std::make_unique<PerfDemo>();

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        auto moved = std::move(unique);
        unique = std::move(moved);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto unique_move_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // shared_ptr copy (atomic increment)
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        auto copied = shared;  // Atomic ref count increment
    }
    end = std::chrono::high_resolution_clock::now();
    auto shared_copy_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Results for " << iterations << " iterations:" << std::endl;
    std::cout << "  unique_ptr move: " << unique_move_time << " μs" << std::endl;
    std::cout << "  shared_ptr copy: " << shared_copy_time << " μs" << std::endl;

    std::cout << "\nshared_ptr copy involves atomic operation" << std::endl;
    std::cout << "unique_ptr move is just pointer copy" << std::endl;
}

void PerformanceGuidelines() {
    std::cout << "\n=== Performance Guidelines ===" << std::endl;

    std::cout << "1. Prefer unique_ptr:" << std::endl;
    std::cout << "   Zero cost abstraction" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Use make_shared:" << std::endl;
    std::cout << "   Single allocation" << std::endl;
    std::cout << "   Better cache locality" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Avoid unnecessary shared_ptr:" << std::endl;
    std::cout << "   Atomic counting has cost" << std::endl;
    std::cout << "   Especially in tight loops" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Pass by reference when possible:" << std::endl;
    std::cout << "   const T& instead of shared_ptr<T>" << std::endl;
    std::cout << "   Avoids ref count increment" << std::endl;
    std::cout << std::endl;

    std::cout << "5. Don't optimize prematurely:" << std::endl;
    std::cout << "   Measure first" << std::endl;
    std::cout << "   Smart pointers usually 'fast enough'" << std::endl;
}