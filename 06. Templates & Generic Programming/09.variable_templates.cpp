import std;
#include "templates_header.h"

// Basic variable template
template <typename T>
constexpr T Pi = T(3.14159265358979);

void BasicVariableTemplate() {
    std::cout << "=== Variable Templates ===" << std::endl;

    std::cout << "Pi<double> = " << Pi<double> << std::endl;
    std::cout << "Pi<float> = " << Pi<float> << std::endl;
    std::cout << "Pi<int> = " << Pi<int> << " (truncated)" << std::endl;

    static_assert(Pi<int> == 3);
}

// Specialized variable templates
template <typename T>
constexpr bool IsIntegral = false;

template <>
constexpr bool IsIntegral<int> = true;

template <>
constexpr bool IsIntegral<long> = true;

template <>
constexpr bool IsIntegral<short> = true;

void SpecializedVariables() {
    std::cout << "\n=== Specialized Variable Templates ===" << std::endl;

    std::cout << "IsIntegral<int> = " << IsIntegral<int> << std::endl;
    std::cout << "IsIntegral<double> = " << IsIntegral<double> << std::endl;
    std::cout << "IsIntegral<std::string> = " << IsIntegral<std::string> << std::endl;
}

// Variable templates with computation
template <int N>
constexpr int Factorial = N * Factorial<N - 1>;

template <>
constexpr int Factorial<0> = 1;

void ComputedVariables() {
    std::cout << "\n=== Computed Variable Templates ===" << std::endl;

    std::cout << "Factorial<5> = " << Factorial<5> << std::endl;
    std::cout << "Factorial<10> = " << Factorial<10> << std::endl;

    static_assert(Factorial<5> == 120);
}

// Type traits as variable templates (C++14 style)
// Pre-C++14: std::is_integral<T>::value
// C++14:     std::is_integral_v<T>

template <typename T>
constexpr bool IsPointerV = std::is_pointer<T>::value;

void VariableTemplatesForTraits() {
    std::cout << "\n=== Variable Templates for Traits ===" << std::endl;

    std::cout << "C++14 style: _v suffix" << std::endl;
    std::cout << "  std::is_integral_v<T>  vs  std::is_integral<T>::value" << std::endl;

    std::cout << "\nIsPointerV<int> = " << IsPointerV<int> << std::endl;
    std::cout << "IsPointerV<int*> = " << IsPointerV<int*> << std::endl;

    // Standard library provides _v versions
    std::cout << "\nstd::is_integral_v<int> = " << std::is_integral_v<int> << std::endl;
    std::cout << "std::is_pointer_v<double*> = " << std::is_pointer_v<double*> << std::endl;
}

// Variable templates with multiple parameters
template <typename T, typename U>
constexpr bool IsSameV = std::is_same<T, U>::value;

void MultipleParameterVariables() {
    std::cout << "\n=== Multi-Parameter Variable Templates ===" << std::endl;

    std::cout << "IsSameV<int, int> = " << IsSameV<int, int> << std::endl;
    std::cout << "IsSameV<int, double> = " << IsSameV<int, double> << std::endl;

    // Standard library version
    std::cout << "\nstd::is_same_v<int, int32_t> = "
        << std::is_same_v<int, int32_t> << std::endl;
}

// Practical example: compile-time configuration
template <typename T>
constexpr size_t Alignment = alignof(T);

template <typename T>
constexpr size_t Size = sizeof(T);

void PracticalVariables() {
    std::cout << "\n=== Practical Examples ===" << std::endl;

    std::cout << "Type properties as variables:" << std::endl;
    std::cout << "  Alignment<int> = " << Alignment<int> << std::endl;
    std::cout << "  Alignment<double> = " << Alignment<double> << std::endl;
    std::cout << "  Size<int> = " << Size<int> << std::endl;
    std::cout << "  Size<double> = " << Size<double> << std::endl;

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  Cleaner syntax than ::value" << std::endl;
    std::cout << "  More readable" << std::endl;
    std::cout << "  Consistent with constexpr variables" << std::endl;
}