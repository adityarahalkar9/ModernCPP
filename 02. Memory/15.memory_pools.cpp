#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>
#include "memory_header.h"

// Simple fixed-size memory pool
class FixedPool {
public:
    explicit FixedPool(size_t object_size, size_t count)
        : object_size_(object_size),
        total_size_(object_size* count) {
        // Allocate raw memory
        memory_ = std::malloc(total_size_);
        if(!memory_) throw std::bad_alloc();

        // Build free list
        free_list_ = nullptr;
        for(size_t i = count; i > 0; --i) {
            void* slot = static_cast<char*>(memory_) + ((i - 1) * object_size_);
            auto* node = static_cast<FreeNode*>(slot);
            node->next = free_list_;
            free_list_ = node;
        }
    }

    ~FixedPool() {
        std::free(memory_);
    }

    void* allocate() {
        if(!free_list_) return nullptr;  // Out of memory
        void* result = free_list_;
        free_list_ = free_list_->next;
        return result;
    }

    void deallocate(void* ptr) {
        if(!ptr) return;
        auto* node = static_cast<FreeNode*>(ptr);
        node->next = free_list_;
        free_list_ = node;
    }

    size_t available() const {
        size_t count = 0;
        FreeNode* current = free_list_;
        while(current) {
            ++count;
            current = current->next;
        }
        return count;
    }

private:
    struct FreeNode {
        FreeNode* next;
    };

    size_t object_size_;
    size_t total_size_;
    void* memory_;
    FreeNode* free_list_;
};

void FixedPoolDemo() {
    std::cout << "=== Fixed-Size Memory Pool ===" << std::endl;

    constexpr size_t object_size = sizeof(double);
    constexpr size_t pool_count = 10;

    FixedPool pool(object_size, pool_count);

    std::cout << "Initial available: " << pool.available() << std::endl;

    // Allocate some objects
    std::vector<void*> allocations;
    for(int i = 0; i < 5; ++i) {
        void* mem = pool.allocate();
        if(mem) {
            new (mem) double(i * 1.5);
            allocations.push_back(mem);
        }
    }

    std::cout << "After 5 allocations, available: " << pool.available() << std::endl;

    // Use objects
    std::cout << "Values: ";
    for(void* mem : allocations) {
        auto* d = static_cast<double*>(mem);
        std::cout << *d << " ";
        d->~double();  // Cleanup
    }
    std::cout << std::endl;

    // Return to pool
    for(void* mem : allocations) {
        pool.deallocate(mem);
    }

    std::cout << "After deallocation, available: " << pool.available() << std::endl;
}

// Region/arena allocator
class Arena {
public:
    explicit Arena(size_t size) : size_(size), used_(0) {
        memory_ = std::malloc(size);
        if(!memory_) throw std::bad_alloc();
    }

    ~Arena() {
        std::free(memory_);
    }

    void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)) {
        // Align pointer
        uintptr_t current = reinterpret_cast<uintptr_t>(memory_) + used_;
        uintptr_t aligned = (current + alignment - 1) & ~(alignment - 1);

        size_t padding = aligned - current;
        if(used_ + padding + bytes > size_) {
            return nullptr;  // Out of memory
        }

        used_ += padding + bytes;
        return reinterpret_cast<void*>(aligned);
    }

    // Note: No individual deallocation - entire arena freed at once
    void reset() {
        used_ = 0;
    }

    size_t used() const { return used_; }
    size_t total() const { return size_; }

private:
    size_t size_;
    size_t used_;
    void* memory_;
};

void ArenaDemo() {
    std::cout << "\n=== Arena/Region Allocator ===" << std::endl;

    Arena arena(1024);

    std::cout << "Initial usage: " << arena.used() << "/" << arena.total() << std::endl;

    // Allocate various objects
    int* a = new (arena.allocate(sizeof(int))) int(42);
    double* b = new (arena.allocate(sizeof(double))) double(3.14);
    char* c = new (arena.allocate(10)) char[10];

    std::cout << "After allocations: " << arena.used() << "/" << arena.total() << std::endl;
    std::cout << "a = " << *a << ", b = " << *b << std::endl;

    // Cleanup: destructors must be called manually
    a->~int();
    b->~double();

    // Or reset the whole arena
    arena.reset();
    std::cout << "After reset: " << arena.used() << "/" << arena.total() << std::endl;

    std::cout << "\nArena advantages:" << std::endl;
    std::cout << "  + Very fast allocation (just bump pointer)" << std::endl;
    std::cout << "  + Cache-friendly (sequential)" << std::endl;
    std::cout << "  + No fragmentation" << std::endl;
    std::cout << "  - Can't free individual objects" << std::endl;
}

// Object pool with automatic construction/destruction
template <typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t count) : pool_(sizeof(T), count) {}

    template <typename... Args>
    T* construct(Args&&... args) {
        void* mem = pool_.allocate();
        if(!mem) return nullptr;
        return new (mem) T(std::forward<Args>(args)...);
    }

    void destroy(T* obj) {
        if(!obj) return;
        obj->~T();
        pool_.deallocate(obj);
    }

private:
    FixedPool pool_;
};

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    int life;

    Particle(float x_, float y_, float z_)
        : x(x_), y(y_), z(z_), vx(0), vy(0), vz(0), life(100) {
    }

    ~Particle() {
        // Cleanup if needed
    }
};

void ObjectPoolDemo() {
    std::cout << "\n=== Object Pool Demo ===" << std::endl;

    ObjectPool<Particle> pool(100);

    std::vector<Particle*> particles;

    // Spawn particles
    for(int i = 0; i < 5; ++i) {
        auto* p = pool.construct(
            static_cast<float>(i),
            static_cast<float>(i * 2),
            static_cast<float>(i * 3)
        );
        if(p) particles.push_back(p);
    }

    std::cout << "Spawned " << particles.size() << " particles:" << std::endl;
    for(const auto* p : particles) {
        std::cout << "  (" << p->x << ", " << p->y << ", " << p->z << ")" << std::endl;
    }

    // Kill particles
    for(auto* p : particles) {
        pool.destroy(p);
    }

    std::cout << "All particles destroyed" << std::endl;
}

// Comparison of allocation strategies
void AllocationComparison() {
    std::cout << "\n=== Allocation Strategy Comparison ===" << std::endl;

    std::cout << "1. new/delete (general purpose):" << std::endl;
    std::cout << "   + Flexible sizes" << std::endl;
    std::cout << "   + Works everywhere" << std::endl;
    std::cout << "   - Slow (system calls, fragmentation)" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Memory pools (fixed size):" << std::endl;
    std::cout << "   + Very fast for same-size objects" << std::endl;
    std::cout << "   + No fragmentation" << std::endl;
    std::cout << "   - Only one size" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Arenas (bump allocator):" << std::endl;
    std::cout << "   + Fastest allocation" << std::endl;
    std::cout << "   + Great for temporaries" << std::endl;
    std::cout << "   - No individual free" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Stack (automatic storage):" << std::endl;
    std::cout << "   + Fastest possible" << std::endl;
    std::cout << "   + Automatic cleanup" << std::endl;
    std::cout << "   - Limited size" << std::endl;
}