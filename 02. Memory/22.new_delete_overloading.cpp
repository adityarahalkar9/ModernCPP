#include <iostream>
#include <cstdlib>
#include <new>
#include <string>
#include <vector>
#include "memory_header.h"

// Global operator new/delete overloading
// (Note: This affects ALL allocations in the program)
// Uncomment to see effect, but be careful in real code!

/*
static size_t total_allocated = 0;

void* operator new(size_t size) {
    std::cout << "  [NEW] " << size << " bytes" << std::endl;
    total_allocated += size;
    void* p = std::malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept {
    std::cout << "  [DELETE] at " << p << std::endl;
    std::free(p);
}
*/

// Class-specific operator new/delete
class Tracked {
public:
    Tracked() {
        std::cout << "  Tracked constructed at " << this << std::endl;
    }
    ~Tracked() {
        std::cout << "  Tracked destroyed at " << this << std::endl;
    }

    // Class-specific operator new
    static void* operator new(size_t size) {
        std::cout << "  [Tracked::new] " << size << " bytes" << std::endl;
        void* p = std::malloc(size);
        if(!p) throw std::bad_alloc();
        return p;
    }

    // Class-specific operator delete
    static void operator delete(void* p) noexcept {
        std::cout << "  [Tracked::delete] at " << p << std::endl;
        std::free(p);
    }

    // Array versions
    static void* operator new[](size_t size) {
        std::cout << "  [Tracked::new[]] " << size << " bytes" << std::endl;
        void* p = std::malloc(size);
        if(!p) throw std::bad_alloc();
        return p;
    }

    static void operator delete[](void* p) noexcept {
        std::cout << "  [Tracked::delete[]] at " << p << std::endl;
        std::free(p);
    }
};

void ClassSpecificNewDelete() {
    std::cout << "=== Class-Specific new/delete ===" << std::endl;

    std::cout << "Creating single Tracked:" << std::endl;
    Tracked* t = new Tracked();
    delete t;

    std::cout << "\nCreating array of Tracked:" << std::endl;
    Tracked* arr = new Tracked[3];
    delete[] arr;
}

// Placement new/delete (for advanced scenarios)
class PlacementTracked {
public:
    PlacementTracked(int v) : value_(v) {
        std::cout << "  PlacementTracked(" << v << ") at " << this << std::endl;
    }
    ~PlacementTracked() {
        std::cout << "  ~PlacementTracked(" << value_ << ")" << std::endl;
    }

private:
    int value_;
};

// Custom placement new
void* operator new(size_t size, void* buffer) noexcept {
    std::cout << "  [placement new] " << size << " bytes at " << buffer << std::endl;
    return buffer;  // Just return the provided buffer
}

void PlacementNewDemo() {
    std::cout << "\n=== Placement New/Delete ===" << std::endl;

    alignas(PlacementTracked) unsigned char buffer[sizeof(PlacementTracked)];

    // Use placement new
    PlacementTracked* obj = new (buffer) PlacementTracked(42);

    std::cout << "Object at: " << obj << " (same as buffer: " << static_cast<void*>(buffer) << ")" << std::endl;

    // Must manually call destructor
    obj->~PlacementTracked();

    std::cout << "Note: No delete - memory is on stack" << std::endl;
}

// Aligned new/delete (C++17)
struct alignas(32) Aligned {
    double data[4];
};

void AlignedNewDelete() {
    std::cout << "\n=== Aligned new/delete (C++17) ===" << std::endl;

    std::cout << "alignof(Aligned): " << alignof(Aligned) << std::endl;

    // C++17: aligned new is automatically used
    Aligned* a = new Aligned;
    std::cout << "Address: " << a << std::endl;
    std::cout << "Is 32-aligned: " << (reinterpret_cast<uintptr_t>(a) % 32 == 0) << std::endl;
    delete a;
}

// Sized delete (C++14)
class SizedDelete {
public:
    SizedDelete() { std::cout << "  SizedDelete created" << std::endl; }
    ~SizedDelete() { std::cout << "  SizedDelete destroyed" << std::endl; }

    static void operator delete(void* p, size_t size) noexcept {
        std::cout << "  [sized delete] " << size << " bytes at " << p << std::endl;
        std::free(p);
    }
};

void SizedDeleteDemo() {
    std::cout << "\n=== Sized Delete (C++14) ===" << std::endl;

    std::cout << "sizeof(SizedDelete): " << sizeof(SizedDelete) << std::endl;

    SizedDelete* sd = new SizedDelete();
    delete sd;  // Calls sized delete with correct size
}

// When to overload new/delete
void UseCases() {
    std::cout << "\n=== Use Cases ===" << std::endl;

    std::cout << "1. Debugging/Memory tracking" << std::endl;
    std::cout << "   - Log all allocations" << std::endl;
    std::cout << "   - Detect leaks" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Custom allocators" << std::endl;
    std::cout << "   - Pools, arenas" << std::endl;
    std::cout << "   - Game engine allocators" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Performance optimization" << std::endl;
    std::cout << "   - Avoid system allocator overhead" << std::endl;
    std::cout << "   - Better cache locality" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Embedded systems" << std::endl;
    std::cout << "   - No heap available" << std::endl;
    std::cout << "   - Static memory pools" << std::endl;
}

// Custom allocator example
template <typename T>
class LoggingAllocator {
public:
    using value_type = T;

    LoggingAllocator() = default;

    template <typename U>
    LoggingAllocator(const LoggingAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        std::cout << "  [alloc] " << n << " x " << sizeof(T) << std::endl;
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        std::cout << "  [dealloc] " << n << " x " << sizeof(T) << std::endl;
        std::free(p);
    }

    template <typename U>
    bool operator==(const LoggingAllocator<U>&) const noexcept { return true; }
    template <typename U>
    bool operator!=(const LoggingAllocator<U>&) const noexcept { return false; }
};

void CustomAllocatorDemo() {
    std::cout << "\n=== Custom Allocator ===" std::endl;

    std::cout << "Creating vector with logging allocator:" << std::endl;
    std::vector<int, LoggingAllocator<int>> vec;

    std::cout << "Pushing elements:" << std::endl;
    for(int i = 0; i < 5; ++i) {
        vec.push_back(i);
    }
    std::cout << "Vector destroyed (deallocations happen):" << std::endl;
}