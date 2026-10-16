import std;
#include "templates_header.h"

// Dependent names with typename
template <typename T>
void ProcessContainer(const T& container) {
    // T::value_type is a dependent name
    // Must use 'typename' to tell compiler it's a type
    typename T::value_type first = *container.begin();

    std::cout << "  First element: " << first << std::endl;
}

void DependentTypeNames() {
    std::cout << "=== Dependent Type Names ===" << std::endl;

    std::cout << "Must use 'typename' for dependent types:" << std::endl;
    std::cout << "  typename T::value_type" << std::endl;
    std::cout << "  (compiler doesn't know it's a type)" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    ProcessContainer(vec);
}

// Dependent names with template
template <typename T>
struct MetaInfo {
    using type = T;

    template <typename U>
    struct Rebind {
        using type = U;
    };
};

template <typename T>
void UseNestedTemplate() {
    // '::template' needed for dependent template names
    typename MetaInfo<T>::template Rebind<double>::type dbl_value = 3.14;

    std::cout << "  Rebound to double: " << dbl_value << std::endl;
}

void DependentTemplateNames() {
    std::cout << "\n=== Dependent Template Names ===" << std::endl;

    std::cout << "Must use 'template' for dependent templates:" << std::endl;
    std::cout << "  T::template Rebind<U>" << std::endl;
    std::cout << "  (otherwise '<' is parsed as less-than)" << std::endl;

    UseNestedTemplate<int>();
}

// this-> for dependent base classes
template <typename T>
class Base6 {
protected:
    void method() {
        std::cout << "  Base6::method" << std::endl;
    }
};

template <typename T>
class Derived6 : public Base6<T> {
public:
    void call_method() {
        // Without this->, name lookup won't find Base's method
        // in dependent base class
        this->method();  // 'this->' required
    }
};

void DependentBaseClass() {
    std::cout << "\n=== Dependent Base Classes ===" << std::endl;

    std::cout << "In dependent base class, need 'this->':" << std::endl;
    std::cout << "  this->method();" << std::endl;
    std::cout << "  (unqualified name won't find it)" << std::endl;

    Derived6<int> d;
    d.call_method();
}

// Common pitfalls
struct HasFoo {
    static constexpr int foo = 42;
};

struct NoFoo {
    static constexpr int bar = 24;
};

template <typename T>
void AccessFoo() {
    if constexpr(requires { T::foo; }) {
        std::cout << "  foo = " << T::foo << std::endl;
    }
    else {
        std::cout << "  no foo available" << std::endl;
    }
}

void DependentNamePitfalls() {
    std::cout << "\n=== Dependent Name Pitfalls ===" << std::endl;

    std::cout << "Common issues:" << std::endl;
    std::cout << "  1. Forgetting 'typename'" << std::endl;
    std::cout << "  2. Forgetting 'template'" << std::endl;
    std::cout << "  3. Forgetting 'this->' in derived" << std::endl;

    AccessFoo<HasFoo>();
    AccessFoo<NoFoo>();
}