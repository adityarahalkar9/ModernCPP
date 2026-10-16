import std;
#include "templates_header.h"

// Basic template alias
template <typename T>
using Vector = std::vector<T>;

template <typename K, typename V>
using Map = std::map<K, V>;

void BasicTemplateAliases() {
    std::cout << "=== Template Aliases ===" << std::endl;

    std::cout << "C++11 'using' for templates:" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  using Vector = std::vector<T>;" << std::endl;

    Vector<int> vec = {1, 2, 3};
    std::cout << "  Vector<int> size: " << vec.size() << std::endl;

    Map<std::string, int> ages;
    ages["Alice"] = 30;
    std::cout << "  Map entry: Alice -> " << ages["Alice"] << std::endl;
}

// Alias with default parameters
template <typename T, typename Allocator = std::allocator<T>>
using Vec = std::vector<T, Allocator>;

void AliasesWithDefaults() {
    std::cout << "\n=== Aliases with Defaults ===" << std::endl;

    Vec<int> default_vec = {1, 2, 3};
    std::cout << "  Default allocator works" << std::endl;
}

// Alias for complex types
template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename Func>
using Callback = std::function<Func>;

void ComplexTypeAliases() {
    std::cout << "\n=== Complex Type Aliases ===" << std::endl;

    Matrix<int> matrix = {
        {1, 2, 3},
        {4, 5, 6}
    };

    std::cout << "  Matrix[1][2] = " << matrix[1][2] << std::endl;

    Callback<int(int)> cb = [](int x) { return x * 2; };
    std::cout << "  Callback(5) = " << cb(5) << std::endl;
}

// Alias templates vs typedef
void AliasVsTypedef() {
    std::cout << "\n=== using vs typedef ===" << std::endl;

    std::cout << "typedef (old):" << std::endl;
    std::cout << "  typedef std::vector<int> IntVec;" << std::endl;
    std::cout << std::endl;
    std::cout << "using (new):" << std::endl;
    std::cout << "  using IntVec = std::vector<int>;" << std::endl;
    std::cout << std::endl;
    std::cout << "using can be templated:" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  using Vec = std::vector<T>;" << std::endl;
    std::cout << "  (typedef cannot do this!)" << std::endl;
}

// Practical alias patterns
template <typename T>
using Ptr = T*;

template <typename T>
using Ref = T&;

template <typename T>
using ConstRef = const T&;

void PracticalPatterns() {
    std::cout << "\n=== Practical Patterns ===" << std::endl;

    int value = 42;
    Ptr<int> p = &value;
    Ref<int> r = value;
    ConstRef<int> cr = value;

    std::cout << "  *p = " << *p << std::endl;
    std::cout << "  r = " << r << std::endl;
    std::cout << "  cr = " << cr << std::endl;
}