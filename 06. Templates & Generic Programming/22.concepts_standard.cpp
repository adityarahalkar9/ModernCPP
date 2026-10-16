import std;
#include "templates_header.h"

// Standard concepts: equality
template <std::equality_comparable T>
bool AreEqual(T a, T b) {
    return a == b;
}

void EqualityConcepts() {
    std::cout << "=== Standard Concepts: Equality ===" << std::endl;

    std::cout << "std::equality_comparable:" << std::endl;

    bool result1 = AreEqual(42, 42);
    bool result2 = AreEqual("hello", "hello");

    std::cout << "  AreEqual(42, 42) = " << result1 << std::endl;
    std::cout << "  AreEqual(\"hello\", \"hello\") = " << result2 << std::endl;
}

// Standard concepts: ordering
template <std::totally_ordered T>
T Max3(T a, T b) {
    return (a > b) ? a : b;
}

void OrderingConcepts() {
    std::cout << "\n=== Standard Concepts: Ordering ===" << std::endl;

    std::cout << "std::totally_ordered:" << std::endl;
    std::cout << "  Requires <, <=, >, >=, ==" << std::endl;

    auto result = Max3(42, 24);
    std::cout << "  Max3(42, 24) = " << result << std::endl;
}

// Standard concepts: arithmetic
template <std::integral T>
T SumIntegers(T a, T b) {
    return a + b;
}

template <std::floating_point T>
T SumFloating(T a, T b) {
    return a + b;
}

void ArithmeticConcepts() {
    std::cout << "\n=== Standard Concepts: Arithmetic ===" << std::endl;

    std::cout << "std::integral, std::floating_point:" << std::endl;

    auto int_result = SumIntegers(42, 58);
    auto float_result = SumFloating(3.14, 2.86);

    std::cout << "  SumIntegers(42, 58) = " << int_result << std::endl;
    std::cout << "  SumFloating(3.14, 2.86) = " << float_result << std::endl;
}

// Standard concepts: callable
template <std::invocable F>
auto CallFunction(F f) {
    return f();
}

void CallableConcepts() {
    std::cout << "\n=== Standard Concepts: Callable ===" << std::endl;

    std::cout << "std::invocable:" << std::endl;

    auto result = CallFunction([]() { return 42; });
    std::cout << "  Called function returned: " << result << std::endl;
}

// Standard concepts: convertible
template <typename T>
    requires std::convertible_to<T, std::string>
std::string MakeString(T value) {
    return value;
}

void ConvertibleConcepts() {
    std::cout << "\n=== Standard Concepts: Convertible ===" << std::endl;

    std::cout << "std::convertible_to:" << std::endl;

    auto s1 = MakeString(std::string("direct"));
    auto s2 = MakeString("literal");

    std::cout << "  Made strings: " << s1 << ", " << s2 << std::endl;
}

// Standard concepts: ranges
template <std::ranges::range R>
size_t CountElements(R&& r) {
    return std::ranges::distance(r);
}

void RangeConcepts() {
    std::cout << "\n=== Standard Concepts: Ranges ===" << std::endl;

    std::cout << "std::ranges::range:" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};
    size_t count = CountElements(vec);

    std::cout << "  Count: " << count << std::endl;
}

// List of important standard concepts
void StandardConceptsList() {
    std::cout << "\n=== Standard Concepts List ===" << std::endl;

    std::cout << "Core language concepts:" << std::endl;
    std::cout << "  std::same_as" << std::endl;
    std::cout << "  std::derived_from" << std::endl;
    std::cout << "  std::convertible_to" << std::endl;
    std::cout << "  std::common_with" << std::endl;
    std::cout << std::endl;

    std::cout << "Comparison concepts:" << std::endl;
    std::cout << "  std::equality_comparable" << std::endl;
    std::cout << "  std::totally_ordered" << std::endl;
    std::cout << std::endl;

    std::cout << "Object concepts:" << std::endl;
    std::cout << "  std::constructible_from" << std::endl;
    std::cout << "  std::default_initializable" << std::endl;
    std::cout << "  std::move_constructible" << std::endl;
    std::cout << "  std::copy_constructible" << std::endl;

    std::cout << "Callable concepts:" << std::endl;
    std::cout << "  std::invocable" << std::endl;
    std::cout << "  std::predicate" << std::endl;

    std::cout << "Numeric concepts:" << std::endl;
    std::cout << "  std::integral" << std::endl;
    std::cout << "  std::floating_point" << std::endl;
    std::cout << "  std::assignable_from" << std::endl;
}