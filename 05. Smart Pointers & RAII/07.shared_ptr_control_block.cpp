import std;
#include "smart_pointers_header.h"

class ControlBlockDemo {
public:
    ControlBlockDemo() {
        std::cout << "  ControlBlockDemo constructed" << std::endl;
    }

    ~ControlBlockDemo() {
        std::cout << "  ControlBlockDemo destroyed" << std::endl;
    }
};

void ControlBlockBasics() {
    std::cout << "=== shared_ptr Control Block ===" << std::endl;

    std::cout << "shared_ptr layout:" << std::endl;
    std::cout << "  [ptr to object] [ptr to control block]" << std::endl;
    std::cout << std::endl;
    std::cout << "Control block contains:" << std::endl;
    std::cout << "  - strong reference count" << std::endl;
    std::cout << "  - weak reference count" << std::endl;
    std::cout << "  - deleter (if custom)" << std::endl;
    std::cout << "  - allocator (if custom)" << std::endl;
    std::cout << "  - object itself (if make_shared)" << std::endl;

    auto ptr = std::make_shared<ControlBlockDemo>();
    std::cout << "\nControl block alive while shared_ptr exists" << std::endl;
}

void MakeSharedVsNew() {
    std::cout << "\n=== make_shared vs new ===" << std::endl;

    std::cout << "make_shared advantages:" << std::endl;
    std::cout << "  1. Single allocation (object + control block together)" << std::endl;
    std::cout << "  2. Better cache locality" << std::endl;
    std::cout << "  3. Exception safe" << std::endl;
    std::cout << "  4. Less code" << std::endl;

    std::cout << "\nmake_shared disadvantage:" << std::endl;
    std::cout << "  Memory for object not freed until ALL weak_ptrs gone" << std::endl;
    std::cout << "  (Even if shared_ptr count hits 0)" << std::endl;

    std::cout << "\nComparison:" << std::endl;
    std::cout << "  std::shared_ptr<T> a(new T);  // 2 allocations" << std::endl;
    std::cout << "  auto b = std::make_shared<T>();  // 1 allocation" << std::endl;

    // Demonstrate
    auto optimized = std::make_shared<ControlBlockDemo>();
    std::cout << "  make_shared: 1 allocation" << std::endl;

    std::shared_ptr<ControlBlockDemo> unoptimized(new ControlBlockDemo());
    std::cout << "  new + shared_ptr: 2 allocations" << std::endl;
}

void ControlBlockLifetime() {
    std::cout << "\n=== Control Block Lifetime ===" << std::endl;

    std::cout << "Control block lives until:" << std::endl;
    std::cout << "  - strong count reaches 0 → object destroyed" << std::endl;
    std::cout << "  - weak count reaches 0 → control block destroyed" << std::endl;

    std::weak_ptr<ControlBlockDemo> weak;

    std::cout << "\nCreating shared_ptr:" << std::endl;
    {
        auto shared = std::make_shared<ControlBlockDemo>();
        weak = shared;

        std::cout << "  strong count: " << shared.use_count() << std::endl;
        std::cout << "  weak expired: " << weak.expired() << std::endl;
    }

    std::cout << "After shared_ptr gone:" << std::endl;
    std::cout << "  Object destroyed (strong count 0)" << std::endl;
    std::cout << "  Control block still alive (for weak_ptr)" << std::endl;
    std::cout << "  weak expired: " << weak.expired() << std::endl;
}

void StrongVsWeakCounts() {
    std::cout << "\n=== Strong vs Weak Counts ===" std::endl;

    auto shared = std::make_shared<ControlBlockDemo>();

    std::cout << "Initial state:" << std::endl;
    std::cout << "  use_count (strong): " << shared.use_count() << std::endl;

    // Create weak_ptr
    std::weak_ptr<ControlBlockDemo> weak1 = shared;
    std::weak_ptr<ControlBlockDemo> weak2 = shared;
    std::weak_ptr<ControlBlockDemo> weak3 = shared;

    std::cout << "\nAfter 3 weak_ptrs:" << std::endl;
    std::cout << "  use_count (strong): " << shared.use_count() << std::endl;
    std::cout << "  (weak count not directly queryable)" << std::endl;

    // Copy shared_ptr
    auto shared_copy = shared;
    std::cout << "\nAfter shared copy:" << std::endl;
    std::cout << "  use_count: " << shared.use_count() << std::endl;

    std::cout << "\nDestroying strong refs:" << std::endl;
    shared_copy.reset();
    shared.reset();

    std::cout << "  All strong refs gone, object destroyed" << std::endl;
    std::cout << "  But weak_ptrs still reference control block" << std::endl;
    std::cout << "  weak1 expired: " << weak1.expired() << std::endl;
}

void CustomAllocatorControlBlock() {
    std::cout << "\n=== Control Block with Custom Allocator ===" << std::endl;

    std::cout << "Control block can use custom allocator:" << std::endl;

    // Just demonstrating the concept
    std::cout << "  std::allocate_shared<T>(alloc, args...)" << std::endl;
    std::cout << "  Uses allocator for control block + object" << std::endl;

    // Standard allocators work transparently
    std::allocator<ControlBlockDemo> alloc;
    auto ptr = std::allocate_shared<ControlBlockDemo>(alloc);

    std::cout << "  allocated with custom allocator" << std::endl;
    std::cout << "  use_count: " << ptr.use_count() << std::endl;
}