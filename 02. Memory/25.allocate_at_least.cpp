#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "memory_header.h"

// C++23: std::allocate_at_least
// Returns a struct with ptr and count
void AllocateAtLeastBasics() {
    std::cout << "=== std::allocate_at_least (C++23) ===" << std::endl;

    // Old way: std::allocator<T>::allocate(n)
    // Returns exactly n objects

    // C++23: allocate_at_least(n)
    // May return MORE than n if beneficial (page alignment, etc.)
    // Caller can use the extra space if desired

    std::allocator<int> alloc;

    // Request at least 10 ints
    // auto result = std::allocate_at_least(alloc, 10);
    // result.ptr - pointer to memory
    // result.count - actual count (>= 10)

    // Simulated demonstration (since C++23 support varies):
    std::cout << "Concept:" << std::endl;
    std::cout << "  auto result = std::allocate_at_least(alloc, 10);" << std::endl;
    std::cout << "  // result.ptr = pointer to at least 10 ints" << std::endl;
    std::cout << "  // result.count = actual number available (>= 10)" << std::endl;

    // Manual demonstration of the pattern
    size_t requested = 10;
    size_t actual = requested;

    // Simulate allocator giving more (e.g., for alignment)
    size_t page_size = 4096;
    size_t bytes_needed = requested * sizeof(int);
    size_t pages = (bytes_needed + page_size - 1) / page_size;
    size_t actual_bytes = pages * page_size;
    size_t possible_count = actual_bytes / sizeof(int);

    if(possible_count > requested) {
        actual = possible_count;
        std::cout << "\nAllocator could provide " << actual
            << " instead of " << requested << std::endl;
    }
}

// Use case: vector optimization
void VectorOptimization() {
    std::cout << "\n=== Vector Optimization ===" << std::endl;

    std::cout << "Problem with current vector:" << std::endl;
    std::cout << "  vector<int> v;" << std::endl;
    std::cout << "  v.reserve(1000);" << std::endl;
    std::cout << "  // Allocates exactly 1000 ints" << std::endl;
    std::cout << "  // But allocator might have 1024 available cheaply" << std::endl;

    std::cout << "\nWith allocate_at_least:" << std::endl;
    std::cout << "  // Could get 1024 for same cost" << std::endl;
    std::cout << "  // Use extra capacity if beneficial" << std::endl;

    // Demonstration of capacity usage
    std::vector<int> vec;
    vec.reserve(10);

    std::cout << "\nCurrent vector capacity: " << vec.capacity() << std::endl;
    std::cout << "(Implementation may already over-allocate)" << std::endl;
}

// Custom allocator with allocate_at_least
template <typename T>
struct MinimumAllocator {
    using value_type = T;

    MinimumAllocator() = default;

    template <typename U>
    MinimumAllocator(const MinimumAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) noexcept {
        ::operator delete(p);
    }

    // C++23: allocate_at_least
    struct AllocationResult {
        T* ptr;
        size_t count;
    };

    AllocationResult allocate_at_least(size_t n) {
        // Round up to next power of 2 or page boundary
        size_t better = n;
        if(better < 16) better = 16;
        else {
            // Round up to power of 2
            size_t pow = 1;
            while(pow < better) pow <<= 1;
            better = pow;
        }

        T* p = allocate(better);
        return {p, better};
    }

    template <typename U>
    bool operator==(const MinimumAllocator<U>&) const noexcept { return true; }
    template <typename U>
    bool operator!=(const MinimumAllocator<U>&) const noexcept { return false; }
};

void CustomAllocatorDemo() {
    std::cout << "\n=== Custom Allocator with allocate_at_least ===" << std::endl;

    MinimumAllocator<int> alloc;

    std::cout << "Requesting 10 ints..." << std::endl;
    auto result = alloc.allocate_at_least(10);

    std::cout << "Got pointer to " << result.count << " ints" << std::endl;
    std::cout << "Can use more than requested!" << std::endl;

    // Use all available space
    for(size_t i = 0; i < result.count; ++i) {
        result.ptr[i] = static_cast<int>(i);
    }

    std::cout << "Filled all " << result.count << " slots" << std::endl;

    // Must free ALL that was allocated
    alloc.deallocate(result.ptr, result.count);
}