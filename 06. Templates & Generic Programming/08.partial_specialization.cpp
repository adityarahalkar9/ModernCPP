import std;
#include "templates_header.h"

// Primary template
template <typename T, typename U>
class Pair2 {
public:
    T first;
    U second;

    void describe() const {
        std::cout << "  Generic pair: " << first << ", " << second << std::endl;
    }
};

// Partial specialization: both same type
template <typename T>
class Pair2<T, T> {
public:
    T first, second;

    void describe() const {
        std::cout << "  Same-type pair: " << first << ", " << second << std::endl;
    }

    T sum() const { return first + second; }
};

// Partial specialization: second is pointer
template <typename T, typename U>
class Pair2<T, U*> {
public:
    T first;
    U* second;

    void describe() const {
        std::cout << "  Pointer pair: " << first << ", " << *second << std::endl;
    }
};

void PartialSpecializationBasics() {
    std::cout << "=== Partial Template Specialization ===" << std::endl;

    std::cout << "Partial specialization specializes SOME parameters:" << std::endl;
    std::cout << "  Others remain as template parameters" << std::endl;

    // Generic version
    Pair2<int, std::string> p1{42, "answer"};
    p1.describe();

    // Same-type specialization
    Pair2<int, int> p2{10, 20};
    p2.describe();
    std::cout << "  Sum: " << p2.sum() << std::endl;  // Extra functionality

    // Pointer specialization
    int value = 100;
    Pair2<std::string, int> p3{"pointer", &value};
    p3.describe();
}

// Partial specialization for containers
template <typename T>
class Wrapper2 {
public:
    void describe() const {
        std::cout << "  Generic wrapper" << std::endl;
    }
};

// Specialize for vectors
template <typename T>
class Wrapper2<std::vector<T>> {
public:
    void describe() const {
        std::cout << "  Vector wrapper (element type available)" << std::endl;
    }

    // Can access T (the vector's element type)
    using element_type = T;
};

// Specialize for smart pointers
template <typename T>
class Wrapper2<std::unique_ptr<T>> {
public:
    void describe() const {
        std::cout << "  unique_ptr wrapper" << std::endl;
    }

    using pointee_type = T;
};

void ContainerSpecialization() {
    std::cout << "\n=== Container Specialization ===" << std::endl;

    Wrapper2<int> generic;
    generic.describe();

    Wrapper2<std::vector<int>> vector_wrapper;
    vector_wrapper.describe();

    Wrapper2<std::unique_ptr<double>> ptr_wrapper;
    ptr_wrapper.describe();

    std::cout << "\nCan extract inner types:" << std::endl;
    std::cout << "  Vector element type is accessible" << std::endl;
}

// Specializing on template template parameters
template <template <typename> class Container, typename T>
class Adapter {
    Container<T> data_;

public:
    void add(const T& value) { data_.insert(data_.end(), value); }
    size_t size() const { return std::distance(data_.begin(), data_.end()); }
};

void TemplateTemplateSpecialization() {
    std::cout << "\n=== Template Template Specialization ===" << std::endl;

    std::cout << "Can partially specialize on template parameters:" << std::endl;
    std::cout << "  template <template<typename> class Container, typename T>" << std::endl;

    // This is advanced technique used in:
    std::cout << "\nUsed in:" << std::endl;
    std::cout << "  - Container adapters" << std::endl;
    std::cout << "  - Policy-based design" << std::endl;
}

// SFINAE-based partial specialization
template <typename T, typename = void>
class HasBegin : public std::false_type {};

template <typename T>
class HasBegin<T, std::void_t<decltype(std::declval<T>().begin())>>
    : public std::true_type {
};

void DetectionIdiom() {
    std::cout << "\n=== Detection Idiom (void_t) ===" << std::endl;

    static_assert(HasBegin<std::vector<int>>::value);
    static_assert(!HasBegin<int>::value);

    std::cout << "vector<int> has begin(): " << HasBegin<std::vector<int>>::value << std::endl;
    std::cout << "int has begin(): " << HasBegin<int>::value << std::endl;

    std::cout << "\nUses partial specialization + void_t:" << std::endl;
    std::cout << "  template <typename T, typename = void>" << std::endl;
    std::cout << "  class HasBegin : std::false_type {};" << std::endl;
    std::cout << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  class HasBegin<T, std::void_t<decltype(...)>>" << std::endl;
    std::cout << "      : std::true_type {};" << std::endl;
}