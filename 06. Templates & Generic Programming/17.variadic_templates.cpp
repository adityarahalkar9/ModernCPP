import std;
#include "templates_header.h"

// Basic variadic template
template <typename... Args>
void CountArgs(Args... args) {
    std::cout << "  Number of args: " << sizeof...(args) << std::endl;
}

void BasicVariadic() {
    std::cout << "=== Variadic Templates ===" << std::endl;

    CountArgs();
    CountArgs(1);
    CountArgs(1, 2, 3);
    CountArgs(1, 2.5, "three", '4');
}

// Recursive processing (pre-C++17)
void PrintRecursive() {
    std::cout << std::endl;  // Base case: no args
}

template <typename First, typename... Rest>
void PrintRecursive(First first, Rest... rest) {
    std::cout << first;
    if constexpr(sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    PrintRecursive(rest...);
}

void RecursiveProcessing() {
    std::cout << "\n=== Recursive Processing ===" << std::endl;

    std::cout << "  ";
    PrintRecursive(1, 2.5, "three", '4');
}

// C++17: if constexpr + fold expressions
template <typename... Args>
void PrintModern(Args... args) {
    std::cout << "  ";
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

void ModernVariadic() {
    std::cout << "\n=== Modern Variadic (C++17) ===" << std::endl;

    PrintModern(1, 2.5, "three");
    PrintModern("single");
}

// Sum with fold expression
template <typename... Args>
auto Sum(Args... args) {
    return (args + ... + 0);  // Fold with initial value
}

void VariadicSum() {
    std::cout << "\n=== Variadic Sum ===" << std::endl;

    auto result1 = Sum(1, 2, 3, 4, 5);
    auto result2 = Sum(1.5, 2.5, 3.5);

    std::cout << "  Sum(1,2,3,4,5) = " << result1 << std::endl;
    std::cout << "  Sum(1.5,2.5,3.5) = " << result2 << std::endl;
}

// Variadic class templates
template <typename... Types>
class Tuple3 {
public:
    static constexpr size_t size = sizeof...(Types);

    void describe() const {
        std::cout << "  Tuple with " << size << " types" << std::endl;
    }
};

void VariadicClass() {
    std::cout << "\n=== Variadic Class Templates ===" << std::endl;

    Tuple3<int, double, std::string> t1;
    t1.describe();

    Tuple3<> empty;
    empty.describe();
}

// Perfect forwarding with variadic
template <typename... Args>
auto MakeTuple(Args&&... args) {
    return std::make_tuple(std::forward<Args>(args)...);
}

void VariadicForwarding() {
    std::cout << "\n=== Variadic Forwarding ===" << std::endl;

    auto tup = MakeTuple(1, 2.5, "three");
    std::cout << "  Created tuple" << std::endl;

    std::string str = "moved";
    auto tup2 = MakeTuple(std::move(str));
    std::cout << "  Original string empty: " << str.empty() << std::endl;
}