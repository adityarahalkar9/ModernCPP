import std;
#include "class_header.h"

// C++23: static operator()
class StaticCallable {
public:
    // Traditional: non-static operator()
    int operator()(int x) const {
        return x * 2;
    }

    // C++23: static operator()
    static int static_call(int x) {
        return x * 3;
    }
};

void StaticOperatorBasics() {
    std::cout << "=== static operator (C++23) ===" << std::endl;

    StaticCallable sc;

    std::cout << "  Non-static: " << sc(5) << std::endl;
    // C++23 allows static operator():
    // std::cout << "  Static: " << StaticCallable()(5) << std::endl;
}

// Use case: stateless functors
class StatelessComparator {
public:
    // C++23: Could be static
    static bool less_than(int a, int b) {
        return a < b;
    }

    // Traditional way (needs instance)
    bool operator()(int a, int b) const {
        return less_than(a, b);
    }
};

void StatelessFunctorDemo() {
    std::cout << "\n=== Stateless Functors ===" << std::endl;

    std::cout << "Stateless functors don't need 'this':" << std::endl;
    std::cout << "  Can be static in C++23" << std::endl;

    StatelessComparator comp;
    std::cout << "  3 < 5: " << comp(3, 5) << std::endl;
    std::cout << "  5 < 3: " << comp(5, 3) << std::endl;
}

// Static operator[] (C++23)
class StaticArray {
    static inline int data[10] = {};

public:
    // C++23: static operator[]
    static int& get(size_t index) {
        return data[index];
    }

    // Traditional instance version
    int& operator[](size_t index) {
        return data[index];
    }
};

void StaticSubscript() {
    std::cout << "\n=== static operator[] (C++23) ===" << std::endl;

    std::cout << "Access without instance:" << std::endl;

    StaticArray::get(0) = 42;
    std::cout << "  StaticArray[0] = " << StaticArray::get(0) << std::endl;

    StaticArray sa;
    sa[1] = 100;
    std::cout << "  sa[1] = " << sa[1] << std::endl;
}

// Benefits
void StaticOperatorBenefits() {
    std::cout << "\n=== static operator Benefits ===" << std::endl;

    std::cout << "1. Clearer intent:" << std::endl;
    std::cout << "   No state = no need for 'this'" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Less boilerplate:" << std::endl;
    std::cout << "   Don't need instance for stateless operations" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Better optimization:" << std::endl;
    std::cout << "   Compiler knows there's no this pointer" << std::endl;
}