#include "memory_header.h"
import std;

// Basic placement new
class TrackedObject {
public:
    TrackedObject(int value) : value_(value) {
        std::cout << "  TrackedObject(" << value << ") constructed at " << this << std::endl;
    }

    ~TrackedObject() {
        std::cout << "  TrackedObject(" << value_ << ") destroyed at " << this << std::endl;
    }

    int value() const { return value_; }

private:
    int value_;
};

void BasicPlacementNew() {
    std::cout << "=== Basic Placement New ===" << std::endl;

    // Allocate raw memory
    alignas(TrackedObject) unsigned char buffer[sizeof(TrackedObject)];

    std::cout << "Buffer address: " << static_cast<void*>(buffer) << std::endl;

    // Construct object in the buffer
    TrackedObject* obj = new (buffer) TrackedObject(42);

    std::cout << "Object value: " << obj->value() << std::endl;

    // Must manually call destructor (no delete!)
    obj->~TrackedObject();

    std::cout << "\nNote: NO delete call - memory is on stack" << std::endl;
}

// Placement new with heap memory
void PlacementNewOnHeap() {
    std::cout << "\n=== Placement New on Heap ===" << std::endl;

    // Allocate raw heap memory
    void* memory = ::operator new(sizeof(TrackedObject));

    std::cout << "Raw memory at: " << memory << std::endl;

    // Construct object
    TrackedObject* obj = new (memory) TrackedObject(100);

    std::cout << "Object value: " << obj->value() << std::endl;

    // Destroy object
    obj->~TrackedObject();

    // Free raw memory
    ::operator delete(memory);
}

// Array placement new
void ArrayPlacementNew() {
    std::cout << "\n=== Array Placement New ===" << std::endl;

    const size_t count = 3;
    alignas(TrackedObject) unsigned char buffer[sizeof(TrackedObject) * count];

    // Construct array elements
    TrackedObject* objects = reinterpret_cast<TrackedObject*>(buffer);
    for(size_t i = 0; i < count; ++i) {
        new (&objects[i]) TrackedObject(static_cast<int>(i * 10));
    }

    // Use objects
    for(size_t i = 0; i < count; ++i) {
        std::cout << "objects[" << i << "] = " << objects[i].value() << std::endl;
    }

    // Destroy in reverse order
    for(size_t i = count; i > 0; --i) {
        objects[i - 1].~TrackedObject();
    }
}

// Memory pool using placement new
class MemoryPool {
public:
    explicit MemoryPool(size_t object_size, size_t count)
        : object_size_(object_size),
        capacity_(count),
        used_(0) {
        pool_ = ::operator new(object_size * count);
    }

    ~MemoryPool() {
        ::operator delete(pool_);
    }

    template <typename T, typename... Args>
    T* construct(Args&&... args) {
        if(used_ >= capacity_) return nullptr;

        void* slot = static_cast<char*>(pool_) + (used_ * object_size_);
        ++used_;
        return new (slot) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void destroy(T* obj) {
        if(!obj) return;
        obj->~T();
        // In real pool: return slot to free list
    }

private:
    size_t object_size_;
    size_t capacity_;
    size_t used_;
    void* pool_;
};

void MemoryPoolDemo() {
    std::cout << "\n=== Memory Pool Demo ===" << std::endl;

    MemoryPool pool(sizeof(TrackedObject), 5);

    std::vector<TrackedObject*> objects;
    for(int i = 1; i <= 3; ++i) {
        auto* obj = pool.construct<TrackedObject>(i * 100);
        if(obj) {
            objects.push_back(obj);
        }
    }

    std::cout << "\nObjects in pool:" << std::endl;
    for(const auto* obj : objects) {
        std::cout << "  " << obj->value() << std::endl;
    }

    std::cout << "\nDestroying objects:" << std::endl;
    for(auto* obj : objects) {
        pool.destroy(obj);
    }
}

// When to use placement new
void UseCases() {
    std::cout << "\n=== Use Cases ===" << std::endl;

    std::cout << "1. Custom memory allocators" << std::endl;
    std::cout << "   - Pools, arenas, etc." << std::endl;
    std::cout << std::endl;
    std::cout << "2. Object lifetime control" << std::endl;
    std::cout << "   - Construct when needed, destroy when done" << std::endl;
    std::cout << "   - Separate allocation from initialization" << std::endl;
    std::cout << std::endl;
    std::cout << "3. Shared memory" << std::endl;
    std::cout << "   - Construct objects in OS-provided memory" << std::endl;
    std::cout << std::endl;
    std::cout << "4. Hardware/memory-mapped I/O" << std::endl;
    std::cout << "   - Place objects at specific addresses" << std::endl;
    std::cout << std::endl;
    std::cout << "5. std::optional, std::variant implementations" << std::endl;
    std::cout << "   - Delayed construction of contained objects" << std::endl;
}

// Dangers of placement new
void Dangers() {
    std::cout << "\n=== Dangers ===" << std::endl;

    std::cout << "1. Must manually call destructor" << std::endl;
    std::cout << "   - Forget = memory leaks (for non-trivial types)" << std::endl;
    std::cout << std::endl;
    std::cout << "2. Must ensure proper alignment" << std::endl;
    std::cout << "   - Misaligned = undefined behavior" << std::endl;
    std::cout << std::endl;
    std::cout << "3. Cannot use regular delete" << std::endl;
    std::cout << "   - delete obj;  // WRONG: calls operator delete on buffer" << std::endl;
    std::cout << std::endl;
    std::cout << "4. Exception safety" << std::endl;
    std::cout << "   - If constructor throws, memory leaks" << std::endl;
    std::cout << "   - Need try/catch around placement new" << std::endl;
}