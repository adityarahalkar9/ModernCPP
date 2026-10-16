import std;
#include "templates_header.h"

// Basic template template parameter
template <typename T>
struct DefaultAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) noexcept {
        ::operator delete(p);
    }
};

// Container that takes allocator as template template parameter
template <typename T, template <typename> class Allocator = DefaultAllocator>
class Vector {
    Allocator<T> alloc_;
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void grow() {
        size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        T* new_data = alloc_.allocate(new_cap);
        for(size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        if(data_) alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    ~Vector() {
        for(size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        if(data_) alloc_.deallocate(data_, capacity_);
    }

    void push_back(const T& value) {
        if(size_ >= capacity_) grow();
        new (&data_[size_]) T(value);
        ++size_;
    }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    size_t size() const { return size_; }
};

void TemplateTemplateBasics() {
    std::cout << "=== Template Template Parameters ===" << std::endl;

    std::cout << "Pass templates as parameters to other templates:" << std::endl;
    std::cout << "  template <typename T, template<typename> class Alloc>" << std::endl;

    Vector<int> vec;  // Uses DefaultAllocator
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "Vector with default allocator:" << std::endl;
    for(size_t i = 0; i < vec.size(); ++i) {
        std::cout << "  " << vec[i] << std::endl;
    }
}

// Custom allocator
template <typename T>
struct LoggingAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        std::cout << "  [ALLOC] " << n * sizeof(T) << " bytes" << std::endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        std::cout << "  [DEALLOC] " << n * sizeof(T) << " bytes" << std::endl;
        ::operator delete(p);
    }
};

void CustomAllocatorTemplate() {
    std::cout << "\n=== Custom Allocator ===" << std::endl;

    std::cout << "Using custom allocator:" << std::endl;
    Vector<int, LoggingAllocator> vec;

    std::cout << "Pushing elements:" << std::endl;
    vec.push_back(1);
    vec.push_back(2);

    std::cout << "Vector size: " << vec.size() << std::endl;
}

// Policy-based design with template template parameters
template <typename T>
struct NoChecking {
    static void check(T&) {}
    static void check_index(size_t, size_t) {}
};

template <typename T>
struct EnforceBounds {
    static void check(T& value) {
        // Check invariants
    }
    static void check_index(size_t index, size_t size) {
        if(index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
    }
};

template <typename T,
    template <typename> class CheckingPolicy = NoChecking>
class SmartContainer {
    std::vector<T> data_;
    CheckingPolicy<T> policy_;

public:
    void push(const T& value) {
        policy_.check(const_cast<T&>(value));
        data_.push_back(value);
    }

    T& at(size_t index) {
        policy_.check_index(index, data_.size());
        return data_[index];
    }

    size_t size() const { return data_.size(); }
};

void PolicyBasedDesign() {
    std::cout << "\n=== Policy-Based Design ===" << std::endl;

    std::cout << "Select behavior via template template parameter:" << std::endl;

    std::cout << "\nWith NoChecking:" << std::endl;
    SmartContainer<int> unchecked;
    unchecked.push(1);
    unchecked.push(2);

    std::cout << "With EnforceBounds:" << std::endl;
    SmartContainer<int, EnforceBounds> checked;
    checked.push(1);
    checked.push(2);

    try {
        checked.at(100);  // Will throw
    }
    catch(const std::out_of_range& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
    }
}

// C++17 improvements to template template parameters
// Can now match templates with default arguments

template <typename T, typename Allocator = std::allocator<T>>
class OldStyle {
    // Pre-C++17: couldn't match std::vector (has 2 params)
};

void Cxx17Improvements() {
    std::cout << "\n=== C++17 Template Template Improvements ===" << std::endl;

    std::cout << "C++17 allows matching templates with default args:" << std::endl;
    std::cout << "  template <typename... Args>" << std::endl;
    std::cout << "  class Template;  // Matches std::vector, std::list, etc." << std::endl;

    std::cout << "\nBefore C++17:" << std::endl;
    std::cout << "  Had to match exact parameter list" << std::endl;
    std::cout << "  Couldn't match std::vector (allocator param)" << std::endl;
}