import std;
#include "templates_header.h"

// Basic enable_if usage
template <typename T,
    typename std::enable_if<std::is_integral_v<T>, int>::type = 0>
void OnlyIntegral(T value) {
    std::cout << "  Integral: " << value << std::endl;
}

template <typename T,
    typename std::enable_if<std::is_floating_point_v<T>, int>::type = 0>
void OnlyFloating(T value) {
    std::cout << "  Floating point: " << value << std::endl;
}

void BasicEnableIf() {
    std::cout << "=== std::enable_if ===" << std::endl;

    OnlyIntegral(42);        // OK: int is integral
    OnlyIntegral(100L);      // OK: long is integral
    OnlyFloating(3.14);      // OK: double is floating point
    OnlyFloating(2.5f);      // OK: float is floating point

    std::cout << "\nWrong types won't compile:" << std::endl;
    std::cout << "  OnlyIntegral(3.14);  // ERROR" << std::endl;
    std::cout << "  OnlyFloating(42);    // ERROR" << std::endl;
}

// enable_if in return type
template <typename T>
typename std::enable_if<std::is_pointer_v<T>, T>::type
GetPointer(T ptr) {
    return ptr;
}

template <typename T>
typename std::enable_if<!std::is_pointer_v<T>, T*>::type
GetPointer(T& value) {
    return &value;
}

void EnableIfReturnType() {
    std::cout << "\n=== enable_if in Return Type ===" << std::endl;

    int value = 42;
    int* ptr = &value;

    auto result1 = GetPointer(value);  // Returns int* (address of)
    auto result2 = GetPointer(ptr);    // Returns int* (identity)

    std::cout << "  Result1: " << result1 << " (address)" << std::endl;
    std::cout << "  Result2: " << result2 << " (identity)" << std::endl;
}

// Conditional class template
template <typename T, typename = void>
class Storage {
    // Primary template (fallback)
public:
    void store(T value) {
        std::cout << "  Generic storage: " << value << std::endl;
    }
};

// Specialization for containers
template <typename T>
class Storage<T, std::enable_if_t<HasSizeModern<T>::value>> {
public:
    void store(T container) {
        std::cout << "  Container storage: " << container.size() << " elements" << std::endl;
    }
};

void ConditionalClass() {
    std::cout << "\n=== Conditional Class Templates ===" << std::endl;

    Storage<int> int_storage;
    int_storage.store(42);

    Storage<std::vector<int>> vec_storage;
    vec_storage.store(std::vector<int>{1, 2, 3});
}

// enable_if with multiple conditions
template <typename T,
    typename = std::enable_if_t<
    std::is_arithmetic_v<T> &&
    !std::is_same_v<T, bool> &&
    (sizeof(T) >= 4)>>
    void ProcessNumber(T value) {
    std::cout << "  Processing number: " << value << " ("
        << sizeof(T) << " bytes)" << std::endl;
}

void MultipleConditions() {
    std::cout << "\n=== Multiple Conditions ===" << std::endl;

    ProcessNumber(42);       // int: 4 bytes ✓
    ProcessNumber(3.14);     // double: 8 bytes ✓
    ProcessNumber(100L);     // long: 8 bytes ✓

    std::cout << "\nThese won't compile:" << std::endl;
    std::cout << "  ProcessNumber(true);      // bool excluded" << std::endl;
    std::cout << "  ProcessNumber(short(1));  // < 4 bytes" << std::endl;
    std::cout << "  ProcessNumber(\"string\");  // not arithmetic" << std::endl;
}

// C++14: _t alias
template <typename T>
using EnableIfIntegral = std::enable_if_t<std::is_integral_v<T>, int>;

template <typename T, EnableIfIntegral<T> = 0>
void UsingAlias(T value) {
    std::cout << "  Using alias: " << value << std::endl;
}

void UsingAliasVersion() {
    std::cout << "\n=== C++14 _t Alias ===" << std::endl;

    std::cout << "C++14 provides _t versions:" << std::endl;
    std::cout << "  std::enable_if_t<Cond, T>" << std::endl;
    std::cout << "  (instead of std::enable_if<Cond, T>::type)" << std::endl;

    UsingAlias(42);
}