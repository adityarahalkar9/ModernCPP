import std;
#include "templates_header.h"

// Overloading with concepts
template <std::integral T>
void Describe(T value) {
    std::cout << "  Integral: " << value << std::endl;
}

template <std::floating_point T>
void Describe(T value) {
    std::cout << "  Floating point: " << value << std::endl;
}

void ConceptOverloading() {
    std::cout << "=== Overloading with Concepts ===" << std::endl;

    std::cout << "Different implementations based on type:" << std::endl;

    Describe(42);      // Calls integral version
    Describe(3.14);    // Calls floating_point version
}

// Priority with concepts
template <typename T>
    requires std::is_arithmetic_v<T>
void Process(T value) {
    std::cout << "  Generic arithmetic: " << value << std::endl;
}

// More specific overload (preferred)
template <typename T>
    requires std::is_arithmetic_v<T> && (sizeof(T) == 4)
void Process(T value) {
    std::cout << "  4-byte arithmetic: " << value << std::endl;
}
void ConceptPriority() {
    std::cout << "\n=== Concept Priority ===" << std::endl;

    std::cout << "More specific constraints preferred:" << std::endl;

    Process(42);        // Calls 4-byte version (int is 4 bytes)
    Process(3.14);      // Calls generic (double is 8 bytes)
    Process(100L);      // Calls generic (long is 8 bytes)
}

// Concepts with auto
void Process2(std::integral auto value) {
    std::cout << "  Integral with auto: " << value << std::endl;
}

void Process2(std::floating_point auto value) {
    std::cout << "  Floating with auto: " << value << std::endl;
}

void ConceptsWithAuto() {
    std::cout << "\n=== Concepts with auto ===" << std::endl;

    std::cout << "C++20 abbreviated function templates:" << std::endl;
    std::cout << "  void process(std::integral auto value)" << std::endl;

    Process2(42);
    Process2(3.14);
}

// Disambiguation with concepts
template <typename T>
concept Container2 = requires(T t) {
    t.begin();
    t.end();
    t.size();
};

template <Container2 C>
void Handle(C c) {
    std::cout << "  Handle container: " << c.size() << " elements" << std::endl;
}

template <typename T>
    requires !Container2<T>
void Handle(T value) {
    std::cout << "  Handle non-container: " << value << std::endl;
}

void ConceptDisambiguation() {
    std::cout << "\n=== Concept Disambiguation ===" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    Handle(vec);    // Container version
    Handle(42);     // Non-container version
    Handle("str");  // Non-container version
}