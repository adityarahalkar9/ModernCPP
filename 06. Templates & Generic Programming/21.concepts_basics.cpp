import std;
#include "templates_header.h"

// Basic concept definition
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Using concept with template
template <Numeric T>
T AddNumbers(T a, T b) {
    return a + b;
}

void BasicConcepts() {
    std::cout << "=== Concepts Basics (C++20) ===" << std::endl;

    std::cout << "Concepts constrain template parameters:" << std::endl;
    std::cout << "  template <Numeric T>" << std::endl;
    std::cout << "  T Add(T a, T b);" << std::endl;

    auto result1 = AddNumbers(42, 58);      // OK: int is numeric
    auto result2 = AddNumbers(3.14, 2.86);  // OK: double is numeric

    std::cout << "  Add(42, 58) = " << result1 << std::endl;
    std::cout << "  Add(3.14, 2.86) = " << result2 << std::endl;

    std::cout << "\nThis won't compile:" << std::endl;
    std::cout << "  AddNumbers(\"hello\", \"world\");  // ERROR" << std::endl;
}

// Concept with requires expression
template <typename T>
concept HasSize = requires(T t) {
    t.size();
    { t.size() } -> std::convertible_to<size_t>;
};

void ConceptsWithRequires() {
    std::cout << "\n=== Concepts with requires ===" << std::endl;

    std::cout << "requires expression syntax:" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  concept HasSize = requires(T t) {" << std::endl;
    std::cout << "      t.size();" << std::endl;
    std::cout << "      { t.size() } -> std::convertible_to<size_t>;" << std::endl;
    std::cout << "  };" << std::endl;

    static_assert(HasSize<std::vector<int>>);
    static_assert(!HasSize<int>);

    std::cout << "\nvector<int> has size(): " << HasSize<std::vector<int>> << std::endl;
    std::cout << "int has size(): " << HasSize<int> << std::endl;
}

// Multiple requirements
template <typename T>
concept Iterable = requires(T t) {
    t.begin();
    t.end();
    typename T::value_type;
    typename T::iterator;
};

void MultipleRequirements() {
    std::cout << "\n=== Multiple Requirements ===" << std::endl;

    static_assert(Iterable<std::vector<int>>);
    static_assert(!Iterable<int>);

    std::cout << "vector satisfies Iterable: " << Iterable<std::vector<int>> << std::endl;
    std::cout << "int satisfies Iterable: " << Iterable<int> << std::endl;
}

// Combining concepts
template <typename T>
concept NumericAndIterable = Numeric<T> || Iterable<T>;

template <typename T>
concept NumericAndSmall = Numeric<T> && (sizeof(T) <= 4);

void CombiningConcepts() {
    std::cout << "\n=== Combining Concepts ===" << std::endl;

    std::cout << "Combine with &&, ||, !:" << std::endl;
    std::cout << "  concept A = B && C;" << std::endl;
    std::cout << "  concept A = B || C;" << std::endl;
    std::cout << "  concept A = !B;" << std::endl;

    static_assert(NumericAndIterable<std::vector<int>>);  // Iterable
    static_assert(NumericAndSmall<int>);                   // Numeric, small
    static_assert(!NumericAndSmall<double>);               // Numeric, but 8 bytes
}

// Concept as boolean
template <typename T>
void ProcessIfConcept(T value) {
    if constexpr(Numeric<T>) {
        std::cout << "  Numeric: " << value << std::endl;
    }
    else {
        std::cout << "  Not numeric: " << value << std::endl;
    }
}

void ConceptAsBoolean() {
    std::cout << "\n=== Concepts as Booleans ===" << std::endl;

    std::cout << "Concepts can be used in if constexpr:" << std::endl;

    ProcessIfConcept(42);
    ProcessIfConcept(3.14);
    ProcessIfConcept("string");
}