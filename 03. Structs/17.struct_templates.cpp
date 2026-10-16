import std;
#include "structs_header.h"

// Basic struct template
template <typename T>
struct Pair {
    T first;
    T second;

    T sum() const { return first + second; }
    T max() const { return first > second ? first : second; }
};

void BasicStructTemplates() {
    std::cout << "=== Basic Struct Templates ===" << std::endl;

    Pair<int> int_pair{10, 20};
    Pair<double> dbl_pair{1.5, 2.5};
    Pair<std::string> str_pair{"Hello", "World"};

    std::cout << "int pair sum: " << int_pair.sum() << std::endl;
    std::cout << "double pair max: " << dbl_pair.max() << std::endl;
    std::cout << "string pair: " << str_pair.first << " " << str_pair.second << std::endl;
}

// Template with multiple parameters
template <typename Key, typename Value>
struct Entry {
    Key key;
    Value value;

    bool operator<(const Entry& other) const {
        return key < other.key;
    }
};

void MultipleTemplateParams() {
    std::cout << "\n=== Multiple Template Parameters ===" << std::endl;

    Entry<std::string, int> name_age{"Alice", 30};
    Entry<int, std::string> id_name{42, "Bob"};

    std::cout << "Name: " << name_age.key << ", Age: " << name_age.value << std::endl;
    std::cout << "ID: " << id_name.key << ", Name: " << id_name.value << std::endl;
}

// Template with non-type parameters
template <typename T, size_t N>
struct FixedArray {
    T data[N];

    constexpr size_t size() const { return N; }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
};

void NonTypeTemplateParams() {
    std::cout << "\n=== Non-Type Template Parameters ===" << std::endl;

    FixedArray<int, 5> arr;
    for(size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>(i * 10);
    }

    std::cout << "Array contents: ";
    for(size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "sizeof(FixedArray<int,5>) = " << sizeof(arr) << std::endl;
}

// Template specialization
template <typename T>
struct TypeTraits {
    static constexpr bool is_pointer = false;
    static constexpr const char* name = "unknown";
};

template <typename T>
struct TypeTraits<T*> {
    static constexpr bool is_pointer = true;
    static constexpr const char* name = "pointer";
};

template <>
struct TypeTraits<int> {
    static constexpr bool is_pointer = false;
    static constexpr const char* name = "int";
};

template <>
struct TypeTraits<double> {
    static constexpr bool is_pointer = false;
    static constexpr const char* name = "double";
};

void TemplateSpecialization() {
    std::cout << "\n=== Template Specialization ===" << std::endl;

    std::cout << "TypeTraits<int>::name = " << TypeTraits<int>::name << std::endl;
    std::cout << "TypeTraits<double>::name = " << TypeTraits<double>::name << std::endl;
    std::cout << "TypeTraits<int*>::name = " << TypeTraits<int*>::name << std::endl;
    std::cout << "TypeTraits<int*>::is_pointer = " << TypeTraits<int*>::is_pointer << std::endl;
}

// CRTP with structs
template <typename Derived>
struct Base {
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

struct Impl1 : Base<Impl1> {
    void implementation() {
        std::cout << "Impl1::implementation" << std::endl;
    }
};

struct Impl2 : Base<Impl2> {
    void implementation() {
        std::cout << "Impl2::implementation" << std::endl;
    }
};

void CRTPWithStructs() {
    std::cout << "\n=== CRTP with Structs ===" << std::endl;

    Impl1 i1;
    Impl2 i2;

    i1.interface();  // Calls Impl1::implementation
    i2.interface();  // Calls Impl2::implementation

    std::cout << "\nCRTP = Curiously Recurring Template Pattern" << std::endl;
    std::cout << "Enables static polymorphism without virtual functions" << std::endl;
}

// Template with concepts (C++20)
#include <concepts>

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
struct Number2 {
    T value;

    Number2 operator+(const Number2& other) const {
        return Number2{value + other.value};
    }
};

void ConceptConstrainedStructs() {
    std::cout << "\n=== Concept-Constrained Structs (C++20) ===" << std::endl;

    Number2<int> ni{42};
    Number2<double> nd{3.14};

    auto sum = ni + Number2<int>{10};
    std::cout << "Int sum: " << sum.value << std::endl;

    auto dsum = nd + Number2<double>{2.86};
    std::cout << "Double sum: " << dsum.value << std::endl;

    std::cout << "\nConcepts restrict template parameters:" << std::endl;
    std::cout << "  Number2<std::string> won't compile" << std::endl;
}