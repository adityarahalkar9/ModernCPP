#include <iostream>
#include <string>
#include <memory>
#include "basic_header.h"

// Basic scope demonstration
void ScopeBasics(){
    int outer = 1;

    {   
        // New scope
        int inner = 2;
        std::cout << "Outer: " << outer << ", Inner: " << inner << std::endl;

        {   
            // Nested scope
            int deepest = 3;
            std::cout << "All accessible: " << outer << ", "
                << inner << ", " << deepest << std::endl;
        }
        // deepest is out of scope here
        // std::cout << deepest;  // Error!
    }
    // inner is out of scope here
    // std::cout << inner;  // Error!
}


// Variable shadowing
void VariableShadowing(){
    int x = 10;

    {
        int x = 20;  // Shadows outer x
        std::cout << "Inner x: " << x << std::endl;  // 20
    }

    std::cout << "Outer x: " << x << std::endl;  // 10

    // Shadowing in for loops
    for(int i = 0; i < 3; ++i) {
        std::cout << "Loop i: " << i << std::endl;
        // Each iteration has its own i
    }
    // i is out of scope here
}


// Object lifetime and destructors
class ScopedObject{
public:
    ScopedObject(std::string name) : name_(name){
        std::cout << "ScopedObject('" << name_ << "') constructed" << std::endl;
    }
    ~ScopedObject(){
        std::cout << "ScopedObject('" << name_ << "') destroyed" << std::endl;
    }
private:
    std::string name_{};
};
void ObjectLifetime(){
    std::cout << "Entering function" << std::endl;

    ScopedObject outer("outer");
    {
        std::cout << "Entering inner scope" << std::endl;
        ScopedObject inner("inner");
        std::cout << "Leaving inner scope" << std::endl;
    }  // inner destroyed here

    std::cout << "Back in outer scope" << std::endl;
}  // outer destroyed here


// Static local variables
void StaticLocalVariables(){
    for(int i = 0; i < 3; ++i){
        static int counter = 0;  // Initialized only once
        ++counter;
        std::cout << "Static counter: " << counter << std::endl;
    }
    // Note: counter persists between calls to this function
}


// RAII (Resource Acquisition Is Initialization)
class FileHandler{
public:
    FileHandler(const std::string& filename) : filename_(filename){
        std::cout << "Opening file: " << filename_ << std::endl;
        // Simulated file open
        is_open_ = true;
    }
    ~FileHandler(){
        if(is_open_){
            std::cout << "Closing file: " << filename_ << std::endl;
            is_open_ = false;
        }
    }
    void write(const std::string& data){
        if(is_open_){
            std::cout << "Writing to " << filename_ << ": " << data << std::endl;
        }
    }

    // Delete copy to prevent double-free
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    // Allow move
    FileHandler(FileHandler&& other) noexcept
        : filename_(std::move(other.filename_)), is_open_(other.is_open_) {
        other.is_open_ = false;
    }
private:
    std::string filename_{};
    bool is_open_ = false;
};
void RAIIDemo(){
    {
        FileHandler file("example.txt");
        file.write("Hello, RAII!");
        // File automatically closed when scope ends
    }  // Destructor called here

    std::cout << "File already closed" << std::endl;
}


// Lifetime extension of temporaries
void LifetimeExtension(){
    // Temporary bound to const reference has lifetime extended
    const std::string& temp_ref = std::string("temporary");
    std::cout << "Temp ref: " << temp_ref << std::endl;
    // temp_ref is valid until end of scope

    // This does NOT work with non-const references
    // std::string& bad_ref = std::string("temp");  // Error!

    // C++23: Lifetime extension in range-based for loops
    // for (auto& x : get_temporary_container()) { ... }
    // Now safe in C++23
}


// Dangling references and pointers
void DanglingReferences(){
    int* ptr{};

    {
        int local = 42;
        ptr = &local;
        std::cout << "Inside scope: " << *ptr << std::endl;
    }
    // ptr is now dangling - accessing *ptr is undefined behavior
    // std::cout << *ptr;  // UNDEFINED BEHAVIOR!

    // Same with references (though harder to create)
    // auto& ref = []() -> int& { int x = 5; return x; }();  // Dangling!

    std::cout << "Pointer is dangling (not dereferencing it)" << std::endl;
}


// Dynamic allocation lifetime
void DynamicLifetime(){
    // new/delete - manual lifetime management
    int* heap_int = new int(42);
    std::cout << "Heap value: " << *heap_int << std::endl;
    delete heap_int;  // Must manually delete

    // Array allocation
    int* heap_array = new int[5]{1, 2, 3, 4, 5};
    std::cout << "Heap array: ";
    for(int i = 0; i < 5; ++i) std::cout << heap_array[i] << " ";
    std::cout << std::endl;
    delete[] heap_array;  // Must use delete[] for arrays

    // Modern C++: prefer smart pointers
    auto smart_int = std::make_unique<int>(100);
    std::cout << "Smart pointer value: " << *smart_int << std::endl;
    // Automatically deleted when smart_int goes out of scope
}


// Thread storage duration
void ThreadStorageDemo(){
    // thread_local variables - each thread has its own copy
    // (Demonstrated conceptually; actual threading shown in Concurrency folder)

    auto lambda = [](){
            thread_local int thread_counter = 0;
            ++thread_counter;
            return thread_counter;
        };

    std::cout << "First call: " << lambda() << std::endl;
    std::cout << "Second call: " << lambda() << std::endl;
}