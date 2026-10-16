import std;
#include "templates_header.h"

// Basic default template arguments
template <typename T, typename Allocator = std::allocator<T>>
class Container3 {
    std::vector<T, Allocator> data_;

public:
    void push(const T& value) { data_.push_back(value); }
    size_t size() const { return data_.size(); }
};

void DefaultArguments() {
    std::cout << "=== Default Template Arguments ===" << std::endl;

    Container3<int> default_container;  // Uses std::allocator
    default_container.push(1);
    default_container.push(2);

    std::cout << "Default: " << default_container.size() << " elements" << std::endl;
}

// Multiple defaults
template <typename Key,
    typename Value = std::string,
    typename Hash = std::hash<Key>,
    typename Compare = std::less<Key>>
    class Map {
    std::vector<std::pair<Key, Value>> entries_;

    public:
        void insert(const Key& key, const Value& value) {
            entries_.emplace_back(key, value);
        }

        void print() const {
            for(const auto& [key, value] : entries_) {
                std::cout << "  " << key << " -> " << value << std::endl;
            }
        }
};

void MultipleDefaults() {
    std::cout << "\n=== Multiple Default Arguments ===" << std::endl;

    Map<int> int_map;  // All defaults
    int_map.insert(1, "one");
    int_map.insert(2, "two");
    int_map.print();

    // Partial specification
    Map<int, double> int_double_map;
    int_double_map.insert(3, 3.14);
}

// Defaults referring to other parameters
template <typename T, typename Pointer = T*, typename Reference = T&>
class Iterator {
    Pointer ptr_;

public:
    explicit Iterator(Pointer p) : ptr_(p) {}

    Reference operator*() const { return *ptr_; }
    Pointer operator->() const { return ptr_; }
};

void DependentDefaults() {
    std::cout << "\n=== Defaults Referring to Other Parameters ===" << std::endl;

    std::cout << "Defaults can use earlier parameters:" << std::endl;
    std::cout << "  template <typename T," << std::endl;
    std::cout << "            typename Pointer = T*," << std::endl;
    std::cout << "            typename Reference = T&>" << std::endl;

    int value = 42;
    Iterator<int> iter(&value);  // Pointer=int*, Reference=int&

    std::cout << "Iterator points to: " << *iter << std::endl;
}

// Function template defaults (C++11)
template <typename T = int>
T DefaultFunction(T value = T{}) {
    return value;
}

void FunctionTemplateDefaults() {
    std::cout << "\n=== Function Template Defaults ===" << std::endl;

    // Uses both defaults
    auto a = DefaultFunction();

    // Explicit for type, default for value
    auto b = DefaultFunction<double>();

    // Explicit for both
    auto c = DefaultFunction<std::string>("hello");

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
}

// Default with SFINAE
template <typename T,
    typename = std::enable_if_t<std::is_integral_v<T>>>
void OnlyIntegral(T value) {
    std::cout << "  Integral: " << value << std::endl;
}

void ConditionalDefaults() {
    std::cout << "\n=== Conditional Defaults (SFINAE) ===" << std::endl;

    std::cout << "Use default parameter + enable_if:" << std::endl;
    std::cout << "  template <typename T," << std::endl;
    std::cout << "            typename = std::enable_if_t<std::is_integral_v<T>>>" << std::endl;

    OnlyIntegral(42);     // OK: int is integral
    OnlyIntegral(100L);   // OK: long is integral
    // OnlyIntegral(3.14);  // Error: double not integral
}