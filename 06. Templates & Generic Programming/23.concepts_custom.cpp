import std;
#include "templates_header.h"

// Custom concept for containers
template <typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    t.begin();
    t.end();
    t.size();
    { t.size() } -> std::convertible_to<size_t>;
};

void CustomContainerConcept() {
    std::cout << "=== Custom Container Concept ===" << std::endl;

    static_assert(Container<std::vector<int>>);
    static_assert(!Container<int>);

    std::cout << "vector<int> is Container: " << Container<std::vector<int>> << std::endl;
    std::cout << "int is Container: " << Container<int> << std::endl;
}

// Custom concept for printable types
template <typename T>
concept Printable = requires(const T & t, std::ostream & os) {
    { os << t } -> std::same_as<std::ostream&>;
};

void CustomPrintableConcept() {
    std::cout << "\n=== Custom Printable Concept ===" << std::endl;

    static_assert(Printable<int>);
    static_assert(Printable<std::string>);
    static_assert(!Printable<std::vector<int>>);  // No operator<<

    std::cout << "int is Printable: " << Printable<int> << std::endl;
    std::cout << "vector<int> is Printable: " << Printable<std::vector<int>> << std::endl;
}

// Concept with complex requirements
template <typename T>
concept Serializable = requires(const T & t) {
    // Must have serialize method
    { t.serialize() } -> std::convertible_to<std::string>;

    // Must have deserialize static method
    { T::deserialize(std::string{}) } -> std::same_as<T>;
};

void ComplexConcept() {
    std::cout << "\n=== Complex Concept Requirements ===" << std::endl;

    std::cout << "Multiple requirements in one concept:" << std::endl;
    std::cout << "  - Member function with specific return" << std::endl;
    std::cout << "  - Static member function" << std::endl;

    // Example class that satisfies the concept
    struct Data {
        std::string serialize() const { return "data"; }
        static Data deserialize(const std::string&) { return {}; }
    };

    static_assert(Serializable<Data>);
    std::cout << "Data satisfies Serializable: " << Serializable<Data> << std::endl;
}

// Concept for arithmetic operations
template <typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a* b } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;
};

void ArithmeticConcept() {
    std::cout << "\n=== Custom Arithmetic Concept ===" << std::endl;

    static_assert(Arithmetic<int>);
    static_assert(Arithmetic<double>);
    static_assert(!Arithmetic<std::string>);  // No / operator

    std::cout << "int satisfies Arithmetic: " << Arithmetic<int> << std::endl;
    std::cout << "string satisfies Arithmetic: " << Arithmetic<std::string> << std::endl;
}

// Using custom concepts
template <Container C>
void PrintContainer(const C& c) {
    std::cout << "  Container with " << c.size() << " elements" << std::endl;
}

void UsingCustomConcepts() {
    std::cout << "\n=== Using Custom Concepts ===" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    PrintContainer(vec);

    // This won't compile:
    // PrintContainer(42);  // ERROR: int doesn't satisfy Container
}