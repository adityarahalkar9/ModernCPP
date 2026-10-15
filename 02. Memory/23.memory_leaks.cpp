#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "memory_header.h"

// Basic memory leak
void BasicLeak() {
    std::cout << "=== Basic Memory Leak ===" << std::endl;

    // BAD: Memory leaked (no pointer to delete later)
    // new int(42);  // Allocated but never freed

    std::cout << "This would leak 4 bytes" << std::endl;
    std::cout << "No pointer saved - can't free it later" << std::endl;

    // GOOD: Automatic cleanup
    auto p = std::make_unique<int>(42);
    std::cout << "Value: " << *p << std::endl;
}  // p automatically freed

// Leak via lost pointer
void LostPointerLeak() {
    std::cout << "\n=== Lost Pointer Leak ===" << std::endl;

    int* p = new int(100);
    p = new int(200);  // First allocation leaked!

    std::cout << "First allocation (100) is now unreachable" << std::endl;
    std::cout << "Memory leaked: sizeof(int) bytes" << std::endl;

    delete p;  // Only frees second allocation
}

// Leak via exception
void ExceptionLeak() {
    std::cout << "\n=== Exception Leak ===" << std::endl;

    try {
        int* p = new int(42);

        if(true) {
            throw std::runtime_error("Something went wrong");
        }

        delete p;  // Never reached due to exception
    }
    catch(const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
        std::cout << "Memory leaked due to exception!" << std::endl;
    }

    // SOLUTION: Use RAII
    try {
        auto p = std::make_unique<int>(42);

        if(true) {
            throw std::runtime_error("Something went wrong");
        }
    }
    catch(const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
        std::cout << "Memory NOT leaked (unique_ptr cleaned up)" << std::endl;
    }
}

// Leak in container
void ContainerLeak() {
    std::cout << "\n=== Container Leak ===" << std::endl;

    // BAD: Container of raw owning pointers
    {
        std::vector<int*> vec;
        for(int i = 0; i < 5; ++i) {
            vec.push_back(new int(i * 10));
        }
        // Must manually delete all elements
        // If forgotten, all 5 allocations leak

        for(int* p : vec) {
            delete p;  // Must remember this
        }
        // If exception thrown before this, all leak!
    }

    // GOOD: Container of unique_ptr
    {
        std::vector<std::unique_ptr<int>> vec;
        for(int i = 0; i < 5; ++i) {
            vec.push_back(std::make_unique<int>(i * 10));
        }
        // Automatic cleanup, exception safe
    }

    std::cout << "unique_ptr container is exception safe" << std::endl;
}

// Detecting leaks (conceptual)
void LeakDetection() {
    std::cout << "\n=== Leak Detection ===" << std::endl;

    std::cout << "Tools for detecting leaks:" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Valgrind (memcheck)" << std::endl;
    std::cout << "   valgrind --leak-check=full ./program" << std::endl;
    std::cout << std::endl;
    std::cout << "2. AddressSanitizer (ASan)" << std::endl;
    std::cout << "   g++ -fsanitize=address -g program.cpp" << std::endl;
    std::cout << std::endl;
    std::cout << "3. LeakSanitizer (LSan)" << std::endl;
    std::cout << "   g++ -fsanitize=leak -g program.cpp" << std::endl;
    std::cout << std::endl;
    std::cout << "4. Visual Studio: CRT Debug Heap" << std::endl;
    std::cout << "   _CrtDumpMemoryLeaks();" << std::endl;

    std::cout << "\nCommon leak patterns:" << std::endl;
    std::cout << "  - Forgetting delete" << std::endl;
    std::cout << "  - Exception before delete" << std::endl;
    std::cout << "  - Lost pointer (reassignment)" << std::endl;
    std::cout << "  - Container of raw owning pointers" << std::endl;
    std::cout << "  - Circular shared_ptr references" << std::endl;
}

// RAII solution
class RAIIExample {
public:
    explicit RAIIExample(int value) : value_(new int(value)) {
        std::cout << "  RAIIExample(" << value << ") created" << std::endl;
    }

    ~RAIIExample() {
        std::cout << "  RAIIExample destroyed (memory freed)" << std::endl;
        delete value_;
    }

    int value() const { return *value_; }

    // Rule of Three/Five
    RAIIExample(const RAIIExample& other)
        : value_(new int(*other.value_)) {
        std::cout << "  RAIIExample copy-constructed" << std::endl;
    }

    RAIIExample& operator=(const RAIIExample& other) {
        if(this != &other) {
            *value_ = *other.value_;
        }
        return *this;
    }

    RAIIExample(RAIIExample&& other) noexcept : value_(other.value_) {
        other.value_ = nullptr;
        std::cout << "  RAIIExample move-constructed" << std::endl;
    }

private:
    int* value_;
};

void RAIISolution() {
    std::cout << "\n=== RAII Solution ===" << std::endl;

    std::cout << "Creating RAII object:" << std::endl;
    {
        RAIIExample obj(42);
        std::cout << "  Value: " << obj.value() << std::endl;
    }  // Automatically destroyed

    std::cout << "No memory leak possible with RAII" << std::endl;
    std::cout << "Even if exception is thrown" << std::endl;
}

// Shared_ptr cycles
struct CycleNode {
    std::string name;
    std::shared_ptr<CycleNode> next;

    explicit CycleNode(std::string n) : name(std::move(n)) {
        std::cout << "  CycleNode('" << name << "') created" << std::endl;
    }
    ~CycleNode() {
        std::cout << "  CycleNode('" << name << "') destroyed" << std::endl;
    }
};

void SharedPtrCycle() {
    std::cout << "\n=== shared_ptr Cycle Problem ===" << std::endl;

    std::cout << "Creating cycle (BAD - will leak):" << std::endl;
    {
        auto node1 = std::make_shared<CycleNode>("node1");
        auto node2 = std::make_shared<CycleNode>("node2");

        node1->next = node2;  // node1 -> node2
        node2->next = node1;  // node2 -> node1 (CYCLE!)

        std::cout << "  node1 count: " << node1.use_count() << std::endl;
        std::cout << "  node2 count: " << node2.use_count() << std::endl;
    }
    // Note: Neither node is destroyed - they keep each other alive!
    std::cout << "  (Nodes leaked - they reference each other)" << std::endl;

    std::cout << "\nSolution: Use weak_ptr for one direction" << std::endl;
}