import std;
#include "templates_header.h"

// Basic function template
template <typename T>
T Max(T a, T b) {
    return (a > b) ? a : b;
}

void BasicFunctionTemplate() {
    std::cout << "=== Basic Function Templates ===" << std::endl;

    // Explicit template arguments
    int int_max = Max<int>(3, 7);
    double dbl_max = Max<double>(3.14, 2.71);

    // Template argument deduction
    auto deduced = Max(10, 20);  // T deduced as int

    std::cout << "Max<int>(3, 7) = " << int_max << std::endl;
    std::cout << "Max<double>(3.14, 2.71) = " << dbl_max << std::endl;
    std::cout << "Max(10, 20) = " << deduced << " (deduced)" << std::endl;
}

// Multiple type parameters
template <typename T, typename U>
void PrintPair(T first, U second) {
    std::cout << "  (" << first << ", " << second << ")" << std::endl;
}

void MultipleTypeParameters() {
    std::cout << "\n=== Multiple Type Parameters ===" << std::endl;

    PrintPair(42, "hello");       // T=int, U=const char*
    PrintPair(3.14, 100);         // T=double, U=int
    PrintPair<std::string, int>("answer", 42);  // Explicit
}

// Template with return type deduction
template <typename T, typename U>
auto Add(T a, U b) -> decltype(a + b) {
    return a + b;
}

void ReturnTypeDeduction() {
    std::cout << "\n=== Return Type Deduction ===" << std::endl;

    auto result1 = Add(3, 4.5);      // Returns double
    auto result2 = Add(2.5f, 100L);  // Returns long

    std::cout << "Add(3, 4.5) = " << result1 << " (double)" << std::endl;
    std::cout << "Add(2.5f, 100L) = " << result2 << " (long)" << std::endl;
}

// Overloading function templates
template <typename T>
void Describe(T value) {
    std::cout << "  Generic: " << value << std::endl;
}

// Non-template overload (preferred for exact match)
void Describe(int value) {
    std::cout << "  Int specific: " << value << std::endl;
}

// More specialized template
template <typename T>
void Describe(T* value) {
    std::cout << "  Pointer to: " << *value << std::endl;
}

void TemplateOverloading() {
    std::cout << "\n=== Template Overloading ===" << std::endl;

    Describe(42);           // Calls non-template (exact match)
    Describe(3.14);         // Calls template
    Describe("hello");      // Calls template

    int x = 10;
    Describe(&x);           // Calls pointer template

    std::cout << "\nOverload resolution rules:" << std::endl;
    std::cout << "  1. Non-template preferred over template" << std::endl;
    std::cout << "  2. More specialized template preferred" << std::endl;
    std::cout << "  3. Exact match beats conversions" << std::endl;
}

// Templates in headers (must be visible)
// Templates are typically defined in headers
// because compiler needs full definition to instantiate

void HeaderDiscussion() {
    std::cout << "\n=== Templates in Headers ===" << std::endl;

    std::cout << "Templates MUST be defined in headers:" << std::endl;
    std::cout << "  - Compiler needs full definition to instantiate" << std::endl;
    std::cout << "  - Separate .cpp compilation doesn't work" << std::endl;
    std::cout << std::endl;
    std::cout << "Alternatives:" << std::endl;
    std::cout << "  1. Define in header (most common)" << std::endl;
    std::cout << "  2. Explicit instantiation in .cpp" << std::endl;
    std::cout << "  3. Export templates (never implemented)" << std::endl;
}

// Explicit instantiation
template <typename T>
class ExplicitDemo {
public:
    void method() { std::cout << "  ExplicitDemo<T>::method" << std::endl; }
};

// Explicit instantiation declaration
extern template class ExplicitDemo<int>;
// Explicit instantiation definition (in one .cpp)
// template class ExplicitDemo<int>;

void ExplicitInstantiation() {
    std::cout << "\n=== Explicit Instantiation ===" << std::endl;

    std::cout << "Explicit instantiation:" << std::endl;
    std::cout << "  extern template class Explicit<int>;" << std::endl;
    std::cout << "  (declares: 'instantiated elsewhere')" << std::endl;
    std::cout << std::endl;
    std::cout << "Benefits:" << std::endl;
    std::cout << "  - Reduced compile times" << std::endl;
    std::cout << "  - Reduced code bloat" << std::endl;
}