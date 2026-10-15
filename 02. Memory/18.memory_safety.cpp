#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "memory_header.h"

// Dangling pointer demonstration
void DanglingPointers() {
    std::cout << "=== Dangling Pointers ===" << std::endl;

    int* dangling = nullptr;

    {
        int local = 42;
        dangling = &local;
        std::cout << "Inside scope: " << *dangling << std::endl;
    }

    // 'dangling' now points to destroyed variable
    // Dereferencing is UNDEFINED BEHAVIOR
    // std::cout << *dangling;  // BAD!

    std::cout << "Pointer is dangling but we don't dereference it" << std::endl;
    std::cout << "Accessing it would be undefined behavior" << std::endl;

    // Same with heap memory
    int* heap_dangling = new int(100);
    delete heap_dangling;
    // heap_dangling is now dangling
    // *heap_dangling = 5;  // BAD!

    std::cout << "Heap pointer is dangling after delete" << std::endl;
}

// Double free demonstration
void DoubleFree() {
    std::cout << "\n=== Double Free ===" << std::endl;

    int* p = new int(42);
    delete p;

    // Second delete is UNDEFINED BEHAVIOR
    // delete p;  // BAD! Double free

    // Often crashes with heap corruption
    std::cout << "Double free causes heap corruption" << std::endl;
    std::cout << "Usually crashes but may silently corrupt memory" << std::endl;

    // Prevention: set pointer to null after delete
    p = nullptr;
    // delete p;  // Now safe (deleting nullptr is no-op)
    std::cout << "Setting to nullptr makes second delete safe" << std::endl;
}

// Use-after-free
void UseAfterFree() {
    std::cout << "\n=== Use-After-Free ===" << std::endl;

    std::string* str = new std::string("Hello");
    std::cout << "Before free: " << *str << std::endl;

    delete str;
    // Using after free is UNDEFINED BEHAVIOR
    // std::cout << *str;  // BAD!

    std::cout << "Cannot use after free" << std::endl;

    // This is one of the most common security vulnerabilities
    std::cout << "Leading cause of security bugs in C/C++" << std::endl;
}

// Buffer overflow
void BufferOverflow() {
    std::cout << "\n=== Buffer Overflow ===" << std::endl;

    int arr[5] = {1, 2, 3, 4, 5};

    // Writing beyond bounds is UNDEFINED BEHAVIOR
    // arr[10] = 42;  // BAD!
    // arr[-1] = 24;  // BAD!

    std::cout << "Array has " << (sizeof(arr) / sizeof(arr[0])) << " elements" << std::endl;
    std::cout << "Writing past the end is undefined behavior" << std::endl;

    // Prevention: use std::array with .at() for bounds checking
    std::array<int, 5> safe_arr = {1, 2, 3, 4, 5};
    try {
        safe_arr.at(10) = 42;  // Throws std::out_of_range
    }
    catch(const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

// Memory leaks
void MemoryLeaks() {
    std::cout << "\n=== Memory Leaks ===" << std::endl;

    // Leaking memory (no pointer saved, can't delete)
    // new int(42);  // BAD: immediate leak

    // More subtle leak
    void leaky_function() {
        int* p = new int(100);
        // Function exits without delete - leak!
    }

    // Exception safety leak
    try {
        int* p = new int(42);
        if(true) {
            throw std::runtime_error("Oops");  // p leaks
        }
        delete p;
    }
    catch(...) {
        std::cout << "Caught exception (memory leaked)" << std::endl;
    }

    std::cout << "Solutions:" << std::endl;
    std::cout << "  1. Use smart pointers (unique_ptr, shared_ptr)" << std::endl;
    std::cout << "  2. RAII classes" << std::endl;
    std::cout << "  3. Containers (vector, string)" << std::endl;
}

// Smart pointer solutions
void SmartPointerSolutions() {
    std::cout << "\n=== Smart Pointer Solutions ===" << std::endl;

    // unique_ptr: automatic cleanup, no leak possible
    {
        auto p = std::make_unique<int>(42);
        std::cout << "Value: " << *p << std::endl;
    }  // Automatically deleted, even if exception thrown

    // shared_ptr: shared ownership
    {
        auto p1 = std::make_shared<int>(100);
        auto p2 = p1;  // Both own the object
        std::cout << "Count: " << p1.use_count() << std::endl;
    }  // Deleted when last owner goes away

    // weak_ptr: non-owning reference
    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;

    if(auto locked = weak.lock()) {
        std::cout << "Value through weak_ptr: " << *locked << std::endl;
    }

    shared.reset();  // Object destroyed
    if(weak.expired()) {
        std::cout << "weak_ptr knows object is gone" << std::endl;
    }
}

// Iterator invalidation
void IteratorInvalidation() {
    std::cout << "\n=== Iterator Invalidation ===" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};

    // BAD: Invalidating iterator while using it
    // for (auto it = vec.begin(); it != vec.end(); ++it) {
    //     if (*it == 3) {
    //         vec.erase(it);  // Iterator is now invalid!
    //         // Continuing to use 'it' is UB
    //     }
    // }

    // CORRECT: Use return value of erase
    for(auto it = vec.begin(); it != vec.end();) {
        if(*it % 2 == 0) {
            it = vec.erase(it);  // Returns valid iterator to next element
        }
        else {
            ++it;
        }
    }

    std::cout << "After removing evens: ";
    for(int i : vec) std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\nRules for vector:" << std::endl;
    std::cout << "  - Insertion: invalidates all iterators if reallocation occurs" << std::endl;
    std::cout << "  - Deletion: invalidates iterators at/after deletion point" << std::endl;
    std::cout << "  - Reserve: prevents reallocation if capacity sufficient" << std::endl;
}

// Best practices summary
void BestPracticesSummary() {
    std::cout << "\n=== Memory Safety Best Practices ===" << std::endl;

    std::cout << "1. Prefer smart pointers over raw new/delete" << std::endl;
    std::cout << "2. Use containers instead of raw arrays" << std::endl;
    std::cout << "3. Initialize pointers (avoid uninitialized reads)" << std::endl;
    std::cout << "4. Check for null before dereferencing" << std::endl;
    std::cout << "5. Use RAII for resource management" << std::endl;
    std::cout << "6. Be careful with iterator invalidation" << std::endl;
    std::cout << "7. Use tools: AddressSanitizer, Valgrind" << std::endl;
    std::cout << "8. Consider using references when null isn't possible" << std::endl;
    std::cout << "9. Use std::string_view instead of const std::string&" << std::endl;
    std::cout << "10. Understand object lifetime rules" << std::endl;
}