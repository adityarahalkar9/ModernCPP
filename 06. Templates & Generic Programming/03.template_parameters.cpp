import std;
#include "templates_header.h"

// Type template parameter
template <typename T>
void TypeParam() {
    std::cout << "  Type parameter: " << typeid(T).name() << std::endl;
}

void TypeParameters() {
    std::cout << "=== Type Template Parameters ===" << std::endl;

    TypeParam<int>();
    TypeParam<double>();
    TypeParam<std::string>();
}

// Multiple type parameters
template <typename T, typename U, typename V = int>
void MultipleTypes(T t, U u, V v = V{}) {
    std::cout << "  " << typeid(T).name() << ", "
        << typeid(U).name() << ", "
        << typeid(V).name() << std::endl;
}

void MultipleTypeParams() {
    std::cout << "\n=== Multiple Type Parameters ===" << std::endl;

    MultipleTypes(1, 2.5);           // V defaults to int
    MultipleTypes(1, 2.5, "three"); // All explicit
}

// Template template parameters
template <typename T>
class Allocator1 {
public:
    using value_type = T;

    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) noexcept {
        ::operator delete(p);
    }
};

template <typename T, template <typename> class Alloc = Allocator1>
class Container2 {
    Alloc<T> alloc_;
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    ~Container2() {
        if(data_) alloc_.deallocate(data_, capacity_);
    }

    void push_back(const T& value) {
        if(size_ >= capacity_) {
            reserve(capacity_ * 2 + 1);
        }
        data_[size_++] = value;
    }

    void reserve(size_t new_cap) {
        if(new_cap > capacity_) {
            T* new_data = alloc_.allocate(new_cap);
            for(size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }
            if(data_) alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_cap;
        }
    }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    size_t size() const { return size_; }
};

void TemplateTemplateParams() {
    std::cout << "\n=== Template Template Parameters ===" << std::endl;

    std::cout << "Template takes another template as parameter:" << std::endl;
    std::cout << "  template <typename T, template<typename> class Alloc>" << std::endl;
    std::cout << "  class Container { ... };" << std::endl;

    Container2<int> default_container;
    default_container.push_back(1);
    default_container.push_back(2);
    default_container.push_back(3);

    std::cout << "Container with default allocator:" << std::endl;
    for(size_t i = 0; i < default_container.size(); ++i) {
        std::cout << "  " << default_container[i] << std::endl;
    }
}

// Parameter packs in templates
template <typename... Args>
void ParameterPack(Args... args) {
    std::cout << "  Pack size: " << sizeof...(args) << std::endl;
}

void ParameterPacks() {
    std::cout << "\n=== Parameter Packs ===" << std::endl;

    std::cout << "Variadic template parameters:" << std::endl;
    std::cout << "  template <typename... Args>" << std::endl;

    ParameterPack();
    ParameterPack(1);
    ParameterPack(1, 2.5, "three");
}

// Mixing parameter types
template <typename T, int N, template <typename> class Temp, typename... Rest>
void MixedParameters(T value) {
    std::cout << "  Mixed: type + non-type + template + pack" << std::endl;
    std::cout << "  T = " << typeid(T).name() << std::endl;
    std::cout << "  N = " << N << std::endl;
    std::cout << "  Rest pack size = " << sizeof...(Rest) << std::endl;
}

void AllParameterTypes() {
    std::cout << "\n=== All Parameter Types ===" << std::endl;

    std::cout << "Template parameters can be:" << std::endl;
    std::cout << "  1. Type parameters (typename/class T)" << std::endl;
    std::cout << "  2. Non-type parameters (int N, auto V)" << std::endl;
    std::cout << "  3. Template parameters (template<typename> class C)" << std::endl;
    std::cout << "  4. Parameter packs (typename... Args)" << std::endl;

    MixedParameters<int, 42, Allocator1, double, std::string>(10);
}