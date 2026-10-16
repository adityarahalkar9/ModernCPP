#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include "templates_header.h"

// Basic SFINAE example
template <typename T>
auto GetSize(const T& container)
-> decltype(container.size())  // If T has size(), this works
{
    return container.size();
}

// Overload for types without size()
template <typename T>
size_t GetSize(...) {  // Fallback
    return 0;
}

void BasicSFINAE() {
    std::cout << "=== SFINAE Basics ===" << std::endl;

    std::cout << "SFINAE = Substitution Failure Is Not An Error" << std::endl;
    std::cout << "  If substitution fails, remove from overload set" << std::endl;
    std::cout << "  Not a hard error!" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    int number = 42;

    std::cout << "Size of vector: " << GetSize(vec) << std::endl;
    std::cout << "Size of int: " << GetSize(number) << std::endl;
}

// SFINAE with return type
template <typename T>
typename T::value_type GetFirst(const T& container) {
    return *container.begin();
}

// This would fail for T without value_type
// SFINAE removes it from consideration

void ReturnTypeSFINAE() {
    std::cout << "\n=== Return Type SFINAE ===" << std::endl;

    std::vector<int> vec = {10, 20, 30};
    std::cout << "First element: " << GetFirst(vec) << std::endl;

    std::cout << "\nIf T::value_type doesn't exist:" << std::endl;
    std::cout << "  Function removed from overload set" << std::endl;
    std::cout << "  Not a compilation error (if alternative exists)" << std::endl;
}

// SFINAE with parameter types
template <typename T>
void Process(T value, typename T::iterator* = nullptr) {
    std::cout << "  Processing container: " << value.size() << " elements" << std::endl;
}

template <typename T>
void Process(T value, ...) {
    std::cout << "  Processing scalar: " << value << std::endl;
}

void ParameterSFINAE() {
    std::cout << "\n=== Parameter SFINAE ===" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    Process(vec, nullptr);  // Container version
    Process(42);            // Scalar version
}

// SFINAE for detecting member functions
template <typename T>
class HasSize {
    template <typename U, size_t(U::*)() const>
    struct Check;

    template <typename U>
    static std::true_type Test(Check<U, &U::size>*);

    template <typename U>
    static std::false_type Test(...);

public:
    static constexpr bool value = decltype(Test<T>(nullptr))::value;
};

void DetectingMembers() {
    std::cout << "\n=== Detecting Member Functions ===" << std::endl;

    static_assert(HasSize<std::vector<int>>::value);
    static_assert(!HasSize<int>::value);

    std::cout << "vector<int> has size(): " << HasSize<std::vector<int>>::value << std::endl;
    std::cout << "int has size(): " << HasSize<int>::value << std::endl;
}

// Modern alternative: void_t (C++17)
template <typename, typename = void>
struct HasSizeModern : std::false_type {};

template <typename T>
struct HasSizeModern<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {
};

void ModernSFINAE() {
    std::cout << "\n=== Modern SFINAE: void_t ===" << std::endl;

    std::cout << "C++17 void_t makes detection easier:" << std::endl;
    std::cout << std::endl;
    std::cout << "template <typename, typename = void>" << std::endl;
    std::cout << "struct HasSizeModern : std::false_type {};" << std::endl;
    std::cout << "template <typename T>" << std::endl;
    std::cout << "struct HasSizeModern<T, std::void_t<decltype(...)>>" << std::endl;
    std::cout << "    : std::true_type {};" << std::endl;

    static_assert(HasSizeModern<std::vector<int>>::value);
    std::cout << "\nvector<int> has size(): " << HasSizeModern<std::vector<int>>::value << std::endl;
}

// SFINAE limitations
void SFINAELimitations() {
    std::cout << "\n=== SFINAE Limitations ===" << std::endl;

    std::cout << "SFINAE issues:" << std::endl;
    std::cout << "  1. Hard to read and understand" << std::endl;
    std::cout << "  2. Error messages are terrible" << std::endl;
    std::cout << "  3. Can only remove from overload set" << std::endl;
    std::cout << "  4. Limited to substitution contexts" << std::endl;

    std::cout << "\nC++20 Concepts solve these:" << std::endl;
    std::cout << "  - Clear syntax" << std::endl;
    std::cout << "  - Better error messages" << std::endl;
    std::cout << "  - More expressive constraints" << std::endl;
}