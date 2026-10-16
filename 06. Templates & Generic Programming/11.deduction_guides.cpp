import std;
#include "templates_header.h"

// Class without deduction guide
template <typename A, typename B>
struct Pair3 {
    A first;
    B second;

    Pair3(A f, B s) : first(std::move(f)), second(std::move(s)) {}
};

// Deduction guide (C++17)
template <typename A, typename B>
Pair3(A, B) -> Pair3<A, B>;

void CTADBasics() {
    std::cout << "=== Class Template Argument Deduction (C++17) ===" << std::endl;

    std::cout << "Before C++17:" << std::endl;
    std::cout << "  std::pair<int, std::string> p(42, \"hello\");" << std::endl;

    std::cout << "\nC++17 with deduction guides:" << std::endl;
    Pair3 p(42, "hello");  // Deduced as Pair3<int, const char*>

    std::cout << "Deduced type: " << typeid(p).name() << std::endl;
    std::cout << "  first = " << p.first << ", second = " << p.second << std::endl;
}

// Why deduction guides are needed
template <typename T>
struct Wrapper3 {
    T value;

    // Constructor takes something different from T
    explicit Wrapper3(int size) : value(size) {}
};

// Guide: tell compiler how to deduce T from constructor args
template <typename T>
Wrapper3(T) -> Wrapper3<T>;

// Alternative guide for specific constructor
// Wrapper3(int) -> Wrapper3<std::vector<int>>;

void WhyGuidesNeeded() {
    std::cout << "\n=== Why Deduction Guides ===" << std::endl;

    std::cout << "Sometimes deduction is ambiguous or impossible:" << std::endl;
    std::cout << "  Constructor args don't directly map to T" << std::endl;
    std::cout << "  Multiple constructors with different types" << std::endl;

    std::cout << "\nDeduction guide syntax:" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  ClassName(Args...) -> ClassName<Deduced>;" << std::endl;
}

// Practical example: container wrapper
template <typename T, typename Allocator = std::allocator<T>>
class Container4 {
    std::vector<T, Allocator> data_;

public:
    // Regular constructor
    Container4(std::initializer_list<T> init) : data_(init) {}

    // Constructor from iterators
    template <typename Iter>
    Container4(Iter first, Iter last) : data_(first, last) {}

    size_t size() const { return data_.size(); }
};

// Deduction guides
template <typename T>
Container4(std::initializer_list<T>) -> Container4<T>;

template <typename Iter>
Container4(Iter, Iter) -> Container4<typename std::iterator_traits<Iter>::value_type>;

void PracticalGuides() {
    std::cout << "\n=== Practical Deduction Guides ===" << std::endl;

    std::cout << "Initializer list:" << std::endl;
    Container4 c1 = {1, 2, 3, 4, 5};  // Deduced as Container4<int>
    std::cout << "  Size: " << c1.size() << std::endl;

    std::cout << "\nIterator range:" << std::endl;
    std::vector<double> vec = {1.5, 2.5, 3.5};
    Container4 c2(vec.begin(), vec.end());  // Deduced as Container4<double>
    std::cout << "  Size: " << c2.size() << std::endl;
}

// std::vector already has deduction guides
void StandardLibraryCTAD() {
    std::cout << "\n=== Standard Library CTAD ===" << std::endl;

    // These all work without explicit template arguments
    std::vector v1 = {1, 2, 3};           // vector<int>
    std::vector v2(v1);                   // vector<int> (copy)
    std::vector v3(v1.begin(), v1.end()); // vector<int> (iterators)

    std::pair p(1, 2.5);                  // pair<int, double>
    std::tuple t(1, "two", 3.0);          // tuple<int, const char*, double>

    std::cout << "Standard containers have built-in guides" << std::endl;
    std::cout << "  vector, pair, tuple, etc." << std::endl;
}

// Implicit guides from constructors
struct Implicit {
    // Constructor generates implicit guide
    template <typename T>
    Implicit(T t) {
        std::cout << "  Constructed from: " << t << std::endl;
    }
};

void ImplicitGuides() {
    std::cout << "\n=== Implicit Guides ===" << std::endl;

    std::cout << "Constructors automatically generate guides:" << std::endl;
    std::cout << "  If constructor takes T directly" << std::endl;

    Implicit i1(42);
    Implicit i2("hello");
}