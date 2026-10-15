#include <iostream>
#include <vector>
#include <string>
#include "memory_header.h"

// Memory segments: code, data, BSS, heap, stack
int global_initialized = 42;          // Data segment
int global_uninitialized;              // BSS segment
const int global_const = 100;         // Read-only data (usually)
static int static_var = 7;            // Data segment (internal linkage)

void MemorySegments() {
    int stack_var = 10;               // Stack
    static int local_static = 20;     // Data segment (not stack!)
    int* heap_var = new int(30);      // Heap

    std::cout << "=== Memory Segments ===" << std::endl;
    std::cout << "Address of global_initialized (data): " << &global_initialized << std::endl;
    std::cout << "Address of global_uninitialized (BSS): " << &global_uninitialized << std::endl;
    std::cout << "Address of global_const (rodata):     " << &global_const << std::endl;
    std::cout << "Address of static_var (data):         " << &static_var << std::endl;
    std::cout << "Address of stack_var (stack):         " << &stack_var << std::endl;
    std::cout << "Address of local_static (data):       " << &local_static << std::endl;
    std::cout << "Address of heap_var (heap):           " << heap_var << std::endl;

    // Note the address patterns:
    // - Stack addresses typically HIGH and grow DOWN
    // - Heap addresses typically LOW and grow UP
    // - Static/global addresses are in a fixed region

    delete heap_var;
}

// Stack memory demonstration
void StackMemory() {
    std::cout << "\n=== Stack Memory ===" << std::endl;

    // Stack variables are automatically managed
    int a = 1;
    int b = 2;
    int c = 3;

    std::cout << "a at: " << &a << std::endl;
    std::cout << "b at: " << &b << std::endl;
    std::cout << "c at: " << &c << std::endl;

    // Stack grows downward (typically): &a > &b > &c on most systems
    // Note: compilers may reorder, and this is platform-specific

    // Stack size is limited (typically 1-8 MB per thread)
    std::cout << "Stack variables are fast to allocate/deallocate" << std::endl;
    std::cout << "Scope-based lifetime: variables die when scope exits" << std::endl;
}

// Stack overflow demonstration (commented to avoid crashing!)
void StackOverflowRisk() {
    std::cout << "\n=== Stack Overflow Risk ===" << std::endl;

    // DANGER: This would cause stack overflow!
    // int large_array[10'000'000];  // ~40 MB on stack - CRASH!

    // Instead, use heap for large data:
    // auto large = std::make_unique<int[]>(10'000'000);  // OK: on heap

    std::cout << "Default stack size: typically 1-8 MB" << std::endl;
    std::cout << "Large arrays must go on the heap" << std::endl;
    std::cout << "Recursion depth is also limited by stack size" << std::endl;
}

// Heap memory demonstration
void HeapMemory() {
    std::cout << "\n=== Heap Memory ===" << std::endl;

    // Heap allocation with new/delete
    int* single = new int(42);
    int* array = new int[10];

    std::cout << "Single allocation at: " << single << std::endl;
    std::cout << "Array allocation at:   " << array << std::endl;

    // Must manually manage lifetime
    delete single;
    delete[] array;

    std::cout << "Heap requires manual cleanup (or smart pointers)" << std::endl;
    std::cout << "Heap is slower than stack but much larger" << std::endl;
    std::cout << "Heap can fragment over time" << std::endl;
}

// Stack vs Heap comparison
void StackVsHeapComparison() {
    std::cout << "\n=== Stack vs Heap ===" << std::endl;
    std::cout << "Stack:" << std::endl;
    std::cout << "  + Very fast allocation (just move stack pointer)" << std::endl;
    std::cout << "  + Automatic cleanup (scope-based)" << std::endl;
    std::cout << "  + Cache-friendly (contiguous)" << std::endl;
    std::cout << "  - Limited size (1-8 MB typically)" << std::endl;
    std::cout << "  - Lifetime tied to scope" << std::endl;
    std::cout << std::endl;
    std::cout << "Heap:" << std::endl;
    std::cout << "  + Large size (limited by RAM)" << std::endl;
    std::cout << "  + Flexible lifetime" << std::endl;
    std::cout << "  + Can be resized (realloc-style)" << std::endl;
    std::cout << "  - Slower allocation (allocator overhead)" << std::endl;
    std::cout << "  - Manual cleanup required" << std::endl;
    std::cout << "  - Can fragment" << std::endl;
}

// Structure padding and memory layout
struct PaddedStruct {
    char a;      // 1 byte + 3 padding
    int b;       // 4 bytes
    char c;      // 1 byte + 3 padding
    double d;    // 8 bytes
    // Total: 24 bytes due to padding
};

struct OptimizedStruct {
    double d;    // 8 bytes
    int b;       // 4 bytes
    char a;      // 1 byte
    char c;      // 1 byte
    // Total: 16 bytes (with 2 bytes tail padding)
};

void StructurePadding() {
    std::cout << "\n=== Structure Padding ===" << std::endl;

    std::cout << "sizeof(PaddedStruct):    " << sizeof(PaddedStruct) << std::endl;
    std::cout << "sizeof(OptimizedStruct): " << sizeof(OptimizedStruct) << std::endl;

    std::cout << "\nPaddedStruct layout:" << std::endl;
    std::cout << "  a: offset " << offsetof(PaddedStruct, a) << std::endl;
    std::cout << "  b: offset " << offsetof(PaddedStruct, b) << std::endl;
    std::cout << "  c: offset " << offsetof(PaddedStruct, c) << std::endl;
    std::cout << "  d: offset " << offsetof(PaddedStruct, d) << std::endl;

    std::cout << "\nOptimizedStruct layout:" << std::endl;
    std::cout << "  d: offset " << offsetof(OptimizedStruct, d) << std::endl;
    std::cout << "  b: offset " << offsetof(OptimizedStruct, b) << std::endl;
    std::cout << "  a: offset " << offsetof(OptimizedStruct, a) << std::endl;
    std::cout << "  c: offset " << offsetof(OptimizedStruct, c) << std::endl;

    std::cout << "\nRule: Order members from largest to smallest to minimize padding" << std::endl;
}

// Memory addresses and sizes of common types
void TypeSizesAndAddresses() {
    std::cout << "\n=== Type Sizes ===" << std::endl;
    std::cout << "sizeof(char):      " << sizeof(char) << " byte" << std::endl;
    std::cout << "sizeof(short):     " << sizeof(short) << " bytes" << std::endl;
    std::cout << "sizeof(int):       " << sizeof(int) << " bytes" << std::endl;
    std::cout << "sizeof(long):      " << sizeof(long) << " bytes" << std::endl;
    std::cout << "sizeof(long long): " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "sizeof(float):     " << sizeof(float) << " bytes" << std::endl;
    std::cout << "sizeof(double):    " << sizeof(double) << " bytes" << std::endl;
    std::cout << "sizeof(void*):     " << sizeof(void*) << " bytes" << std::endl;

    std::cout << "\n=== Alignment Requirements ===" << std::endl;
    std::cout << "alignof(char):      " << alignof(char) << std::endl;
    std::cout << "alignof(int):       " << alignof(int) << std::endl;
    std::cout << "alignof(double):    " << alignof(double) << std::endl;
    std::cout << "alignof(void*):     " << alignof(void*) << std::endl;
}

// Virtual memory concept
void VirtualMemoryConcept() {
    std::cout << "\n=== Virtual Memory ===" << std::endl;
    std::cout << "Each process sees its own virtual address space" << std::endl;
    std::cout << "Virtual addresses are translated to physical by MMU" << std::endl;
    std::cout << "Pages (typically 4KB) are the unit of mapping" << std::endl;
    std::cout << "This provides isolation and enables swap space" << std::endl;

    // Demonstrating that addresses we see are virtual
    int x = 42;
    std::cout << "This address is virtual: " << &x << std::endl;
    std::cout << "Two processes can use the same virtual address" << std::endl;
    std::cout << "without conflict (different physical memory)" << std::endl;
}