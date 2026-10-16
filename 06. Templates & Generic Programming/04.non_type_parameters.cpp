import std;
#include "templates_header.h"

// Basic non-type template parameter
template <int N>
struct IntWrapper {
    static constexpr int value = N;
};

void BasicNonType() {
    std::cout << "=== Non-Type Template Parameters ===" << std::endl;

    std::cout << "IntWrapper<5>::value = " << IntWrapper<5>::value << std::endl;
    std::cout << "IntWrapper<10>::value = " << IntWrapper<10>::value << std::endl;

    // Each N creates a different type
    static_assert(!std::is_same_v<IntWrapper<5>, IntWrapper<10>>);
    std::cout << "IntWrapper<5> and IntWrapper<10> are DIFFERENT types" << std::endl;
}

// Fixed-size array using non-type parameter
template <typename T, size_t N>
class FixedArray {
    std::array<T, N> data_{};

public:
    constexpr size_t size() const { return N; }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    T* begin() { return data_.data(); }
    T* end() { return data_.data() + N; }
    const T* begin() const { return data_.data(); }
    const T* end() const { return data_.data() + N; }

    void fill(const T& value) {
        for(auto& elem : data_) elem = value;
    }
};

void NonTypeArray() {
    std::cout << "\n=== Fixed-Size Array ===" << std::endl;

    FixedArray<int, 5> arr;
    arr.fill(42);

    std::cout << "Array contents: ";
    for(const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "Size: " << arr.size() << " (compile-time constant)" << std::endl;
}

// Using non-type for compile-time computation
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

void CompileTimeComputation() {
    std::cout << "\n=== Compile-Time Computation ===" << std::endl;

    constexpr auto fact5 = Factorial<5>::value;
    static_assert(fact5 == 120);

    std::cout << "5! = " << fact5 << " (computed at compile time)" << std::endl;
    std::cout << "10! = " << Factorial<10>::value << std::endl;
}

// Non-type with different types
template <auto Value>  // C++17: auto non-type parameter
struct AutoValue {
    static constexpr auto value = Value;
};

void AutoNonType() {
    std::cout << "\n=== auto Non-Type Parameters (C++17) ===" << std::endl;

    std::cout << "C++17: 'auto' as non-type parameter:" << std::endl;
    std::cout << "  template <auto Value>" << std::endl;
    std::cout << "  Deduces the type of the value" << std::endl;

    std::cout << "\nAutoValue<42>::value = " << AutoValue<42>::value << std::endl;
    std::cout << "AutoValue<3.14>::value = " << AutoValue<3.14>::value << std::endl;
    std::cout << "AutoValue<'c'>::value = " << AutoValue<'c'>::value << std::endl;
}

// Constraints on non-type parameters
enum class Color { Red, Green, Blue };

template <Color C>
struct ColorTrait {
    static constexpr const char* name = "unknown";
};

template <>
struct ColorTrait<Color::Red> {
    static constexpr const char* name = "Red";
};

template <>
struct ColorTrait<Color::Green> {
    static constexpr const char* name = "Green";
};

template <>
struct ColorTrait<Color::Blue> {
    static constexpr const char* name = "Blue";
};

void EnumNonType() {
    std::cout << "\n=== Enum as Non-Type Parameter ===" << std::endl;

    std::cout << "ColorTrait<Color::Red>::name = "
        << ColorTrait<Color::Red>::name << std::endl;
    std::cout << "ColorTrait<Color::Green>::name = "
        << ColorTrait<Color::Green>::name << std::endl;
    std::cout << "ColorTrait<Color::Blue>::name = "
        << ColorTrait<Color::Blue>::name << std::endl;
}

// C++20: floating-point and class-type non-type parameters
// (Requires C++20)
struct Point2 {
    int x, y;
    constexpr bool operator==(const Point2& other) const {
        return x == other.x && y == other.y;
    }
};

void Cxx20NonType() {
    std::cout << "\n=== C++20 Non-Type Extensions ===" << std::endl;

    std::cout << "C++20 allows:" << std::endl;
    std::cout << "  - Floating-point non-type parameters" << std::endl;
    std::cout << "  - Class-type non-type parameters" << std::endl;
    std::cout << "  (if they have constexpr comparison)" << std::endl;

    // C++20:
    // template <double D> struct FloatWrapper { ... };
    // template <Point2 P> struct PointWrapper { ... };
}