import std;
#include "templates_header.h"

// Basic fold expressions (C++17)
template <typename... Args>
auto SumAll(Args... args) {
    return (args + ...);  // Unary right fold: (a + (b + (c + d)))
}

template <typename... Args>
auto SumLeft(Args... args) {
    return (... + args);  // Unary left fold: (((a + b) + c) + d)
}

void BasicFoldExpressions() {
    std::cout << "=== Fold Expressions (C++17) ===" << std::endl;

    auto result1 = SumAll(1, 2, 3, 4);
    auto result2 = SumLeft(1, 2, 3, 4);

    std::cout << "  Right fold: " << result1 << std::endl;
    std::cout << "  Left fold: " << result2 << std::endl;
}

// Binary fold with initial value
template <typename... Args>
auto SumWithInit(Args... args) {
    return (args + ... + 100);  // (a + (b + (c + 100)))
}

template <typename... Args>
auto SumLeftWithInit(Args... args) {
    return (100 + ... + args);  // (((100 + a) + b) + c)
}

void BinaryFoldExpressions() {
    std::cout << "\n=== Binary Fold Expressions ===" << std::endl;

    auto result1 = SumWithInit(1, 2, 3);
    auto result2 = SumLeftWithInit(1, 2, 3);

    std::cout << "  Right fold with init: " << result1 << std::endl;
    std::cout << "  Left fold with init: " << result2 << std::endl;
}

// Fold with different operators
template <typename... Args>
auto MultiplyAll(Args... args) {
    return (args * ...);
}

template <typename... Args>
bool AllTrue(Args... args) {
    return (args && ...);
}

template <typename... Args>
bool AnyTrue(Args... args) {
    return (args || ...);
}

void DifferentOperators() {
    std::cout << "\n=== Fold with Different Operators ===" << std::endl;

    std::cout << "  Multiply: " << MultiplyAll(2, 3, 4) << std::endl;
    std::cout << "  All true: " << AllTrue(true, true, true) << std::endl;
    std::cout << "  Any true: " << AnyTrue(false, true, false) << std::endl;
}

// Fold with comma operator
template <typename... Args>
void ExecuteAll(Args... args) {
    (args(), ...);  // Call each function
}

void FoldWithComma() {
    std::cout << "\n=== Fold with Comma Operator ===" << std::endl;

    auto func1 = []() { std::cout << "  Function 1" << std::endl; };
    auto func2 = []() { std::cout << "  Function 2" << std::endl; };
    auto func3 = []() { std::cout << "  Function 3" << std::endl; };

    ExecuteAll(func1, func2, func3);
}

// Fold for printing
template <typename... Args>
void PrintFold(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

void FoldForPrinting() {
    std::cout << "\n=== Fold for Printing ===" << std::endl;

    PrintFold(1, 2.5, "three", '4');
}

// Fold with containers
template <typename Container, typename... Args>
void PushAll(Container& c, Args... args) {
    (c.push_back(args), ...);
}

void FoldWithContainers() {
    std::cout << "\n=== Fold with Containers ===" << std::endl;

    std::vector<int> vec;
    PushAll(vec, 1, 2, 3, 4, 5);

    std::cout << "  Vector: ";
    for(int v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

// Empty pack handling
template <typename... Args>
auto SumEmpty(Args... args) {
    return (args + ... + 0);  // 0 handles empty pack
}

void EmptyPackHandling() {
    std::cout << "\n=== Empty Pack Handling ===" << std::endl;

    auto result = SumEmpty();  // Returns 0
    std::cout << "  Empty sum: " << result << std::endl;

    std::cout << "Rules:" << std::endl;
    std::cout << "  Binary fold with init: empty pack returns init" << std::endl;
    std::cout << "  Unary fold: empty pack is error (except &&, ||, ,)" << std::endl;
}