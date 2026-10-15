#include <iostream>
#include <vector>
#include <functional>
#include "memory_header.h"

// Basic function pointer syntax
int Add(int a, int b) { return a + b; }
int Multiply(int a, int b) { return a * b; }

void BasicFunctionPointers() {
    std::cout << "=== Basic Function Pointers ===" << std::endl;

    // Declaration: return_type (*name)(params)
    int (*fp)(int, int) = Add;

    // Call through pointer
    std::cout << "Add(3,4) = " << fp(3, 4) << std::endl;

    // Reassign to different function
    fp = Multiply;
    std::cout << "Multiply(3,4) = " << fp(3, 4) << std::endl;

    // Alternative call syntax (explicit dereference)
    std::cout << "(*fp)(5,6) = " << (*fp)(5, 6) << std::endl;

    // Function pointers can be nullptr
    int (*null_fp)(int, int) = nullptr;
    if(null_fp == nullptr) {
        std::cout << "Function pointer is null" << std::endl;
    }
}

// Using typedef/using for readability
using BinaryOp = int(*)(int, int);

void TypedefForFunctionPointers() {
    std::cout << "\n=== Using Aliases ===" << std::endl;

    BinaryOp op = Add;
    std::cout << "op(10,20) = " << op(10, 20) << std::endl;

    op = Multiply;
    std::cout << "op(10,20) = " << op(10, 20) << std::endl;
}

// Function pointers as parameters
int Apply(BinaryOp op, int x, int y) {
    return op(x, y);
}

void FunctionPointersAsParams() {
    std::cout << "\n=== Function Pointers as Parameters ===" << std::endl;

    std::cout << "Apply(Add, 5, 3) = " << Apply(Add, 5, 3) << std::endl;
    std::cout << "Apply(Multiply, 5, 3) = " << Apply(Multiply, 5, 3) << std::endl;
}

// Arrays of function pointers
void FunctionPointerArrays() {
    std::cout << "\n=== Arrays of Function Pointers ===" << std::endl;

    BinaryOp operations[] = {Add, Multiply};

    for(size_t i = 0; i < 2; ++i) {
        std::cout << "operations[" << i << "](4,5) = "
            << operations[i](4, 5) << std::endl;
    }
}

// Function pointers for callbacks
void PrintWithCallback(int value, void(*callback)(int)) {
    callback(value);
}

void MyCallback(int x) {
    std::cout << "Callback called with: " << x << std::endl;
}

void CallbackDemo() {
    std::cout << "\n=== Callbacks ===" << std::endl;
    PrintWithCallback(42, MyCallback);
}

// Member function pointers
class Calculator {
public:
    int Add(int a, int b) { return a + b; }
    int Multiply(int a, int b) { return a * b; }
    static int StaticAdd(int a, int b) { return a + b; }
};

void MemberFunctionPointers() {
    std::cout << "\n=== Member Function Pointers ===" << std::endl;

    Calculator calc;

    // Syntax: Return (Class::*name)(Params)
    int (Calculator:: * method)(int, int) = &Calculator::Add;

    // Call: (object.*method)(args)
    std::cout << "calc.Add(3,4) = " << (calc.*method)(3, 4) << std::endl;

    method = &Calculator::Multiply;
    std::cout << "calc.Multiply(3,4) = " << (calc.*method)(3, 4) << std::endl;

    // Through pointer to object:
    Calculator* calcPtr = &calc;
    std::cout << "calcPtr->Add(5,6) = " << (calcPtr->*method)(5, 6) << std::endl;

    // Static member functions are like regular functions
    int (*staticMethod)(int, int) = &Calculator::StaticAdd;
    std::cout << "StaticAdd(7,8) = " << staticMethod(7, 8) << std::endl;
}

// std::function - more flexible than raw function pointers
void StdFunctionDemo() {
    std::cout << "\n=== std::function ===" << std::endl;

    // Can store function pointers, lambdas, functors
    std::function<int(int, int)> f = Add;
    std::cout << "f(3,4) = " << f(3, 4) << std::endl;

    f = Multiply;
    std::cout << "f(3,4) = " << f(3, 4) << std::endl;

    // Lambda
    f = [](int a, int b) { return a - b; };
    std::cout << "f(10,4) = " << f(10, 4) << std::endl;

    // Functor
    struct Functor {
        int operator()(int a, int b) { return a / b; }
    };
    f = Functor{};
    std::cout << "f(20,5) = " << f(20, 5) << std::endl;

    // Trade-off: std::function has overhead (type erasure)
    // Raw function pointers are faster but less flexible
}

// Function pointers vs std::function vs templates
void ComparisonDemo() {
    std::cout << "\n=== Comparison ===" << std::endl;
    std::cout << "Raw function pointers:" << std::endl;
    std::cout << "  + Fast (direct call)" << std::endl;
    std::cout << "  + Simple" << std::endl;
    std::cout << "  - Only work with matching function signature" << std::endl;
    std::cout << std::endl;
    std::cout << "std::function:" << std::endl;
    std::cout << "  + Works with anything callable" << std::endl;
    std::cout << "  + Can store state (lambdas with captures)" << std::endl;
    std::cout << "  - Slower (indirection)" << std::endl;
    std::cout << "  - May allocate" << std::endl;
    std::cout << std::endl;
    std::cout << "Templates (functors):" << std::endl;
    std::cout << "  + Fastest (inlined at compile time)" << std::endl;
    std::cout << "  - Code bloat (each instantiation is separate)" << std::endl;
}

// Practical example: dispatch table
void DispatchTableDemo() {
    std::cout << "\n=== Dispatch Table ===" << std::endl;

    auto operations = std::vector<std::pair<const char*, int(*)(int, int)>>{
        {"add", Add},
        {"multiply", Multiply}
    };

    std::string cmd = "multiply";
    int x = 6, y = 7;

    for(const auto& [name, func] : operations) {
        if(name == cmd) {
            std::cout << cmd << "(" << x << "," << y << ") = " << func(x, y) << std::endl;
            break;
        }
    }
}