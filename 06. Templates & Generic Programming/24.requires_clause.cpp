import std;
#include "templates_header.h"

// Basic requires clause
template <typename T>
    requires std::is_integral_v<T>
void ProcessIntegral(T value) {
    std::cout << "  Integral: " << value << std::endl;
}

void BasicRequiresClause() {
    std::cout << "=== requires Clause ===" << std::endl;

    std::cout << "Syntax options:" << std::endl;
    std::cout << "  1. template<typename T> requires Concept<T>" << std::endl;
    std::cout << "  2. template<typename T>" << std::endl;
    std::cout << "     void func(T) requires Concept<T>;" << std::endl;
    std::cout << "  3. template<Concept T>" << std::endl;

    ProcessIntegral(42);
    ProcessIntegral(100L);

    std::cout << "\nThis won't compile:" << std::endl;
    std::cout << "  ProcessIntegral(3.14);  // ERROR" << std::endl;
}

// requires with multiple conditions
template <typename T>
    requires std::is_arithmetic_v<T> && (sizeof(T) >= 4)
void ProcessNumber2(T value) {
    std::cout << "  Number: " << value << " (" << sizeof(T) << " bytes)" << std::endl;
}

void MultipleConditionsRequires() {
    std::cout << "\n=== Multiple Conditions ===" << std::endl;

    ProcessNumber2(42);     // int: OK
    ProcessNumber2(3.14);   // double: OK
    ProcessNumber2(100L);   // long: OK

    std::cout << "\nThese won't compile:" << std::endl;
    std::cout << "  ProcessNumber2(short(1));  // Too small" << std::endl;
    std::cout << "  ProcessNumber2(\"str\");    // Not arithmetic" << std::endl;
}

// requires expression
template <typename T>
void ProcessContainer2(T c)
    requires requires(T t) {
    t.begin();
    t.end();
    t.size();
}
{
    std::cout << "  Container with " << c.size() << " elements" << std::endl;
}

void RequiresExpression() {
    std::cout << "\n=== requires Expression ===" << std::endl;

    std::cout << "Inline requirements:" << std::endl;
    std::cout << "  void func(T c) requires requires(T t) {" << std::endl;
    std::cout << "      t.begin();" << std::endl;
    std::cout << "      t.end();" << std::endl;
    std::cout << "  }" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    ProcessContainer2(vec);
}

// requires with compound requirements
template <typename T>
void ValidateType(T value)
    requires requires(T t) {
        { t + t } -> std::same_as<T>;           // Return type must be T
        { t* t } -> std::convertible_to<T>;    // Convertible to T
        { -t } noexcept;                        // Must not throw
}
{
    std::cout << "  Valid type: " << value << std::endl;
}

void CompoundRequirements() {
    std::cout << "\n=== Compound Requirements ===" << std::endl;

    std::cout << "Types of requirements:" << std::endl;
    std::cout << "  1. Simple: t.begin()" << std::endl;
    std::cout << "  2. Type: { t.size() } -> size_t" << std::endl;
    std::cout << "  3. Compound: { t + t } -> std::same_as<T>" << std::endl;
    std::cout << "  4. Noexcept: { -t } noexcept" << std::endl;

    ValidateType(42);
    ValidateType(3.14);
}

// requires in class templates
template <typename T>
    requires std::is_default_constructible_v<T>
class Storage2 {
    T value_{};

public:
    T get() const { return value_; }
    void set(T v) { value_ = v; }
};

void RequiresInClass() {
    std::cout << "\n=== requires in Class Templates ===" << std::endl;

    Storage2<int> int_storage;
    int_storage.set(42);
    std::cout << "  Stored: " << int_storage.get() << std::endl;
}