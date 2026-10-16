import std;
#include "templates_header.h"

// Primary template
template <typename T>
class TypeInfo {
public:
    static constexpr const char* name = "unknown";
    static constexpr size_t size = sizeof(T);

    static void print() {
        std::cout << "  " << name << " (size: " << size << ")" << std::endl;
    }
};

// Full specialization for int
template <>
class TypeInfo<int> {
public:
    static constexpr const char* name = "int";
    static constexpr size_t size = sizeof(int);

    static void print() {
        std::cout << "  " << name << " (size: " << size << ")" << std::endl;
    }

    // Can add extra functionality
    static constexpr bool is_signed = true;
    static constexpr int min = INT_MIN;
    static constexpr int max = INT_MAX;

    static void print_range() {
        std::cout << "  Range: [" << min << ", " << max << "]" << std::endl;
    }
};

// Specialization for double
template <>
class TypeInfo<double> {
public:
    static constexpr const char* name = "double";
    static constexpr size_t size = sizeof(double);

    static void print() {
        std::cout << "  " << name << " (size: " << size << ")" << std::endl;
    }

    static constexpr bool is_floating = true;
    static constexpr double epsilon = std::numeric_limits<double>::epsilon();

    static void print_epsilon() {
        std::cout << "  Epsilon: " << epsilon << std::endl;
    }
};

void FullSpecialization() {
    std::cout << "=== Full Template Specialization ===" << std::endl;

    std::cout << "Primary template (generic):" << std::endl;
    TypeInfo<char>::print();

    std::cout << "\nSpecialization for int:" << std::endl;
    TypeInfo<int>::print();
    TypeInfo<int>::print_range();  // Extra functionality

    std::cout << "\nSpecialization for double:" << std::endl;
    TypeInfo<double>::print();
    TypeInfo<double>::print_epsilon();  // Extra functionality
}

// Function template specialization
template <typename T>
T Max3(T a, T b, T c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

// Specialization for const char* (string comparison)
template <>
const char* Max3<const char*>(const char* a, const char* b, const char* c) {
    return (std::strcmp(a, b) > 0) ?
        ((std::strcmp(a, c) > 0) ? a : c) :
        ((std::strcmp(b, c) > 0) ? b : c);
}

void FunctionSpecialization() {
    std::cout << "\n=== Function Template Specialization ===" << std::endl;

    // Generic version
    std::cout << "Max3(1, 5, 3) = " << Max3(1, 5, 3) << std::endl;

    // Specialized version for strings
    const char* result = Max3("apple", "banana", "cherry");
    std::cout << "Max3(\"apple\", \"banana\", \"cherry\") = " << result << std::endl;
}

// When to specialize
template <typename T>
bool IsEqual(const T& a, const T& b) {
    return a == b;
}

// Specialization for floating-point (epsilon comparison)
template <>
bool IsEqual<double>(const double& a, const double& b) {
    return std::abs(a - b) < std::numeric_limits<double>::epsilon() * 100;
}

void SpecializationForSemantics() {
    std::cout << "\n=== Specialization for Different Semantics ===" << std::endl;

    std::cout << "IsEqual(1, 1) = " << IsEqual(1, 1) << std::endl;
    std::cout << "IsEqual(1.0, 1.0) = " << IsEqual(1.0, 1.0) << std::endl;

    // Nearly equal doubles
    std::cout << "IsEqual(0.1+0.2, 0.3) = " << IsEqual(0.1 + 0.2, 0.3) << std::endl;
    std::cout << "  (With epsilon comparison)" << std::endl;
}

// Specialization rules
void SpecializationRules() {
    std::cout << "\n=== Specialization Rules ===" << std::endl;

    std::cout << "Full specialization requirements:" << std::endl;
    std::cout << "  1. Must match primary template exactly" << std::endl;
    std::cout << "  2. 'template <>' prefix required" << std::endl;
    std::cout << "  3. All parameters specified" << std::endl;
    std::endl;

    std::cout << "Syntax:" << std::endl;
    std::cout << "  template <>" << std::endl;
    std::cout << "  class TypeInfo<int> { ... };" << std::endl;
    std::endl;

    std::cout << "Specialization can:" << std::endl;
    std::cout << "  - Change implementation completely" << std::endl;
    std::cout << "  - Add/modify members" << std::endl;
    std::cout << "  - Have different interface" << std::endl;
}