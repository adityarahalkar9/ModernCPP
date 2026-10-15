#include <iostream>
#include <string>
#include <cstring>
#include "memory_header.h"

// Basic void pointer usage
void BasicVoidPointers() {
    std::cout << "=== Basic Void Pointers ===" << std::endl;

    int i = 42;
    double d = 3.14;
    char c = 'A';

    // void* can point to any type
    void* vp1 = &i;
    void* vp2 = &d;
    void* vp3 = &c;

    std::cout << "void* pointing to int:    " << vp1 << std::endl;
    std::cout << "void* pointing to double: " << vp2 << std::endl;
    std::cout << "void* pointing to char:   " << vp3 << std::endl;

    // Cannot dereference void* directly
    // std::cout << *vp1;  // Error: cannot dereference void*

    // Must cast back to original type
    std::cout << "Value: " << *static_cast<int*>(vp1) << std::endl;
    std::cout << "Value: " << *static_cast<double*>(vp2) << std::endl;
}

// Generic functions with void*
void PrintAnything(const void* data, const char* type) {
    if(strcmp(type, "int") == 0) {
        std::cout << *static_cast<const int*>(data) << std::endl;
    }
    else if(strcmp(type, "double") == 0) {
        std::cout << *static_cast<const double*>(data) << std::endl;
    }
    else if(strcmp(type, "string") == 0) {
        std::cout << *static_cast<const std::string*>(data) << std::endl;
    }
    else {
        std::cout << "Unknown type" << std::endl;
    }
}

void GenericFunctions() {
    std::cout << "\n=== Generic Functions with void* ===" << std::endl;

    int i = 42;
    double d = 3.14;
    std::string s = "hello";

    PrintAnything(&i, "int");
    PrintAnything(&d, "double");
    PrintAnything(&s, "string");

    std::cout << "\nThis is how C-style generic code works." << std::endl;
    std::cout << "In modern C++, prefer templates instead!" << std::endl;
}

// Void pointers in C-style callbacks
struct Context {
    int call_count = 0;
    std::string message;
};

void CallbackWithContext(void* user_data) {
    auto* ctx = static_cast<Context*>(user_data);
    ++ctx->call_count;
    std::cout << "Callback #" << ctx->call_count
        << ": " << ctx->message << std::endl;
}

void ContextCallbackDemo() {
    std::cout << "\n=== Context in Callbacks ===" << std::endl;

    Context ctx{0, "Hello from context"};
    void* user_data = &ctx;

    CallbackWithContext(user_data);
    CallbackWithContext(user_data);
    CallbackWithContext(user_data);

    std::cout << "Total calls: " << ctx.call_count << std::endl;
}

// Dangers of void pointers
void VoidPointerDangers() {
    std::cout << "\n=== Void Pointer Dangers ===" << std::endl;

    int i = 42;
    void* vp = &i;

    // DANGER: Casting to wrong type is undefined behavior
    // double* wrong = static_cast<double*>(vp);  // UB!
    // std::cout << *wrong;  // Garbage or crash

    std::cout << "void* has NO type information" << std::endl;
    std::cout << "You must remember what type it points to" << std::endl;
    std::cout << "Casting to wrong type = undefined behavior" << std::endl;

    std::cout << "\nSafer alternatives in modern C++:" << std::endl;
    std::cout << "  - Templates (compile-time type safety)" << std::endl;
    std::cout << "  - std::any (type-safe erased storage)" << std::endl;
    std::cout << "  - std::variant (type-safe union)" << std::endl;
    std::cout << "  - Overloading (compile-time dispatch)" << std::endl;
}

// void* in memory allocation (malloc style)
void VoidPointersInAllocation() {
    std::cout << "\n=== void* in Allocation ===" << std::endl;

    // malloc returns void*
    void* raw = std::malloc(sizeof(int) * 5);

    // Must cast to use
    int* arr = static_cast<int*>(raw);
    for(int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }

    std::cout << "Allocated array: ";
    for(int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::free(raw);  // free takes void* too

    std::cout << "\nNote: In C++, prefer new/delete or smart pointers" << std::endl;
}

// Comparing void pointers
void ComparingVoidPointers() {
    std::cout << "\n=== Comparing void Pointers ===" << std::endl;

    int x = 10, y = 20;
    void* px = &x;
    void* py = &y;
    void* px2 = &x;

    std::cout << "px == px2: " << (px == px2) << std::endl;
    std::cout << "px == py:  " << (px == py) << std::endl;
    std::cout << "px != py:  " << (px != py) << std::endl;

    // Can check for null
    void* null_vp = nullptr;
    std::cout << "null_vp == nullptr: " << (null_vp == nullptr) << std::endl;
}