#include <iostream>
#include <memory>
#include <string>
#include "memory_header.h"

// C++23: std::out_ptr - for C API interop
// Simulated C-style API that writes through pointer
int C_API_GetValue(void** out_value) {
    if(out_value) {
        *out_value = new int(42);  // C API allocates
        return 0;  // Success
    }
    return -1;  // Error
}

void OutPtrBasics() {
    std::cout << "=== std::out_ptr (C++23) ===" << std::endl;

    // OLD WAY: Manual management
    void* raw_result = nullptr;
    if(C_API_GetValue(&raw_result) == 0) {
        // Must remember to free
        // Also, how to know it's an int*?
        auto* result = static_cast<int*>(raw_result);
        std::cout << "Old way: " << *result << std::endl;
        delete result;
    }

    // C++23 WAY: With smart pointer + out_ptr
    std::unique_ptr<int> smart_result;
    if(C_API_GetValue(std::out_ptr(smart_result)) == 0) {
        // Smart pointer now owns the memory
        std::cout << "New way: " << *smart_result << std::endl;
        std::cout << "Automatic cleanup guaranteed!" << std::endl;
    }
}

// inout_ptr for APIs that take and return
int C_API_ModifyValue(void** io_value) {
    if(io_value && *io_value) {
        auto* val = static_cast<int*>(*io_value);
        *val *= 2;
        // Maybe reallocate or keep same
        return 0;
    }
    return -1;
}

void InOutPtrBasics() {
    std::cout << "\n=== std::inout_ptr (C++23) ===" << std::endl;

    // For APIs that both read and write through the pointer
    std::unique_ptr<int> value = std::make_unique<int>(21);

    std::cout << "Before: " << *value << std::endl;

    // C++23: inout_ptr
    if(C_API_ModifyValue(std::inout_ptr(value)) == 0) {
        std::cout << "After: " << *value << std::endl;
        std::cout << "Smart pointer still manages memory" << std::endl;
    }
}

// Practical example with FILE-like API
// Simulating a C library function
struct Handle {
    int id;
    bool valid;
};

extern "C" {
    int create_handle(void** out_handle) {
        static int next_id = 1;
        if(out_handle) {
            *out_handle = new Handle{next_id++, true};
            return 0;
        }
        return -1;
    }

    void destroy_handle(void* handle) {
        if(handle) {
            delete static_cast<Handle*>(handle);
        }
    }
}

// Custom deleter for Handle
struct HandleDeleter {
    void operator()(Handle* h) const {
        std::cout << "  Destroying handle " << h->id << std::endl;
        destroy_handle(h);
    }
};

void PracticalExample() {
    std::cout << "\n=== Practical Example ===" << std::endl;

    std::cout << "Creating handle with smart pointer:" << std::endl;
    {
        std::unique_ptr<Handle, HandleDeleter> handle;
        if(create_handle(std::out_ptr(handle)) == 0) {
            std::cout << "  Handle ID: " << handle->id << std::endl;
            std::cout << "  Valid: " << handle->valid << std::endl;
        }
    }  // Handle automatically destroyed

    std::cout << "Handle cleaned up automatically" << std::endl;
}

// Comparison of approaches
void Comparison() {
    std::cout << "\n=== Comparison ===" << std::endl;

    std::cout << "Approach 1: Raw pointer" << std::endl;
    std::cout << "  void* result;" << std::endl;
    std::cout << "  C_API(&result);" << std::endl;
    std::cout << "  // Must remember to free" << std::endl;
    std::cout << "  // Exception unsafe" << std::endl;

    std::cout << "\nApproach 2: Smart pointer + reset" << std::endl;
    std::cout << "  std::unique_ptr<int> p;" << std::endl;
    std::cout << "  void* temp;" << std::endl;
    std::cout << "  C_API(&temp);" << std::endl;
    std::cout << "  p.reset(static_cast<int*>(temp));" << std::endl;
    std::cout << "  // Works but clunky" << std::endl;

    std::cout << "\nApproach 3: std::out_ptr (C++23)" << std::endl;
    std::cout << "  std::unique_ptr<int> p;" << std::endl;
    std::cout << "  C_API(std::out_ptr(p));" << std::endl;
    std::cout << "  // Clean, safe, automatic" << std::endl;
}