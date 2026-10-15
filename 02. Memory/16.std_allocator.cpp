#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "memory_header.h"

// std::allocator is the default allocator
void DefaultAllocator() {
    std::cout << "=== std::allocator Basics ===" << std::endl;

    // std::allocator<T> is used by containers by default
    std::allocator<int> alloc;

    // Allocate memory for 5 ints
    int* memory = alloc.allocate(5);

    std::cout << "Allocated 5 ints at: " << memory << std::endl;

    // Construct objects
    for(size_t i = 0; i < 5; ++i) {
        alloc.construct(&memory[i], static_cast<int>(i * 10));
    }

    // Use objects
    std::cout << "Values: ";
    for(size_t i = 0; i < 5; ++i) {
        std::cout << memory[i] << " ";
    }
    std::cout << std::endl;

    // Destroy objects
    for(size_t i = 0; i < 5; ++i) {
        alloc.destroy(&memory[i]);
    }

    // Deallocate memory
    alloc.deallocate(memory, 5);

    std::cout << "Note: C++17 deprecated construct/destroy" << std::endl;
    std::cout << "Use placement new and manual destructor calls instead" << std::endl;
}

// Custom allocator with tracking
template <typename T>
class TrackingAllocator {
public:
    using value_type = T;

    TrackingAllocator() = default;

    template <typename U>
    TrackingAllocator(const TrackingAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        std::cout << "  Allocating " << n << " x " << sizeof(T)
            << " bytes (" << n * sizeof(T) << " total)" << std::endl;
        ++allocation_count_;
        total_bytes_ += n * sizeof(T);
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        std::cout << "  Deallocating " << n << " x " << sizeof(T)
            << " bytes (" << n * sizeof(T) << " total)" << std::endl;
        --allocation_count_;
        ::operator delete(p);
    }

    // For C++20 and earlier:
    template <typename U>
    bool operator==(const TrackingAllocator<U>&) const noexcept { return true; }

    template <typename U>
    bool operator!=(const TrackingAllocator<U>&) const noexcept { return false; }

    // Statistics
    static size_t allocation_count() { return allocation_count_; }
    static size_t total_bytes_allocated() { return total_bytes_; }

private:
    inline static size_t allocation_count_ = 0;
    inline static size_t total_bytes_ = 0;
};

void CustomAllocatorDemo() {
    std::cout << "\n=== Custom Allocator ===" << std::endl;

    std::cout << "Creating vector with tracking allocator:" << std::endl;
    {
        std::vector<int, TrackingAllocator<int>> vec;

        std::cout << "Pushing elements..." << std::endl;
        for(int i = 0; i < 5; ++i) {
            vec.push_back(i);
        }

        std::cout << "Vector size: " << vec.size() << std::endl;
    }  // Vector destroyed - allocations freed

    std::cout << "\nFinal stats:" << std::endl;
    std::cout << "Allocations outstanding: " << TrackingAllocator<int>::allocation_count() << std::endl;
    std::cout << "Total bytes allocated: " << TrackingAllocator<int>::total_bytes_allocated() << std::endl;
}

// Allocator with memory pool
template <typename T>
class PoolAllocator {
public:
    using value_type = T;

    // Pool shared among allocators of same type
    struct Pool {
        void* memory;
        size_t size;
        size_t used;
        std::vector<void*> free_list;

        Pool(size_t total_size) : size(total_size), used(0) {
            memory = std::malloc(total_size);
        }

        ~Pool() {
            std::free(memory);
        }

        void* allocate(size_t bytes) {
            if(!free_list.empty()) {
                void* result = free_list.back();
                free_list.pop_back();
                return result;
            }

            if(used + bytes > size) return nullptr;
            void* result = static_cast<char*>(memory) + used;
            used += bytes;
            return result;
        }

        void deallocate(void* p) {
            free_list.push_back(p);
        }
    };

    PoolAllocator() {
        static Pool pool(1024 * 1024);  // 1MB pool
        pool_ = &pool;
    }

    template <typename U>
    PoolAllocator(const PoolAllocator<U>& other) noexcept
        : pool_(other.get_pool()) {
    }

    T* allocate(size_t n) {
        void* mem = pool_->allocate(n * sizeof(T));
        if(!mem) throw std::bad_alloc();
        return static_cast<T*>(mem);
    }

    void deallocate(T* p, size_t) noexcept {
        pool_->deallocate(p);
    }

    Pool* get_pool() const { return pool_; }

    template <typename U>
    bool operator==(const PoolAllocator<U>&) const noexcept { return true; }

    template <typename U>
    bool operator!=(const PoolAllocator<U>&) const noexcept { return false; }

private:
    Pool* pool_;
};

void PoolAllocatorDemo() {
    std::cout << "\n=== Pool Allocator ===" << std::endl;

    {
        std::vector<int, PoolAllocator<int>> vec;
        for(int i = 0; i < 10; ++i) {
            vec.push_back(i);
        }
        std::cout << "Vector with pool allocator: ";
        for(int i : vec) std::cout << i << " ";
        std::cout << std::endl;
    }

    std::cout << "Memory returned to pool (not freed to OS)" << std::endl;
}

// Allocator requirements (C++ concepts)
void AllocatorRequirements() {
    std::cout << "\n=== Allocator Requirements ===" << std::endl;

    std::cout << "An allocator must provide:" << std::endl;
    std::cout << "  - value_type" << std::endl;
    std::cout << "  - allocate(size_t n)" << std::endl;
    std::cout << "  - deallocate(T* p, size_t n)" << std::endl;
    std::cout << "  - Copy constructor (rebind)" << std::endl;
    std::cout << "  - operator== and operator!=" << std::endl;
    std::cout << std::endl;
    std::cout << "Optional (C++17):" << std::endl;
    std::cout << "  - construct/destruct (now discouraged)" << std::endl;
    std::cout << "  - is_always_equal" << std::endl;
    std::cout << "  - propagate_on_container_copy_assignment" << std::endl;
    std::cout << "  - propagate_on_container_move_assignment" << std::endl;
    std::cout << "  - propagate_on_container_swap" << std::endl;
}