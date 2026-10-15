#include <iostream>
#include <type_traits>
#include <string>
#include <new>
#include "memory_header.h"

// std::aligned_storage basics
void AlignedStorageBasics() {
    std::cout << "=== std::aligned_storage ===" << std::endl;

    // std::aligned_storage<Size, Align>::type
    // Gives you a POD type of specified size and alignment

    using Storage = std::aligned_storage<sizeof(double), alignof(double)>::type;

    std::cout << "sizeof(Storage): " << sizeof(Storage) << std::endl;
    std::cout << "alignof(Storage): " << alignof(Storage) << std::endl;

    Storage storage;  // Raw storage, no object yet

    // Place a double in it
    double* d = new (&storage) double(3.14);
    std::cout << "Value: " << *d << std::endl;

    // Cleanup
    d->~double();
}

// Building std::optional from scratch
template <typename T>
class MyOptional {
public:
    MyOptional() : engaged_(false) {}

    ~MyOptional() {
        reset();
    }

    // Copy constructor
    MyOptional(const MyOptional& other)
        : engaged_(other.engaged_) {
        if(engaged_) {
            new (storage()) T(*other);
        }
    }

    // Move constructor
    MyOptional(MyOptional&& other) noexcept
        : engaged_(other.engaged_) {
        if(engaged_) {
            new (storage()) T(std::move(*other));
            other.reset();
        }
    }

    // Construct value in place
    template <typename... Args>
    void emplace(Args&&... args) {
        reset();
        new (storage()) T(std::forward<Args>(args)...);
        engaged_ = true;
    }

    void reset() {
        if(engaged_) {
            get()->~T();
            engaged_ = false;
        }
    }

    bool has_value() const { return engaged_; }

    T& operator*() {
        return *get();
    }

    const T& operator*() const {
        return *get();
    }

    T* operator->() { return get(); }
    const T* operator->() const { return get(); }

    T* get() {
        return engaged_ ? std::launder(reinterpret_cast<T*>(&storage_)) : nullptr;
    }

    const T* get() const {
        return engaged_ ? std::launder(reinterpret_cast<const T*>(&storage_)) : nullptr;
    }

private:
    // aligned_storage for T
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
    bool engaged_;
};

void MyOptionalDemo() {
    std::cout << "\n=== Building MyOptional ===" << std::endl;

    MyOptional<std::string> opt;
    std::cout << "Has value: " << opt.has_value() << std::endl;

    opt.emplace("Hello, World!");
    std::cout << "Value: " << *opt << std::endl;
    std::cout << "Size: " << opt->size() << std::endl;

    // Reset and re-engage
    opt.reset();
    std::cout << "After reset: " << opt.has_value() << std::endl;

    opt.emplace("New value");
    std::cout << "New value: " << *opt << std::endl;
}

// Deprecation notice
void DeprecationNotice() {
    std::cout << "\n=== Deprecation (C++23) ===" std::endl;

    std::cout << "std::aligned_storage is DEPRECATED in C++23!" << std::endl;
    std::cout << std::endl;
    std::cout << "Why?" << std::endl;
    std::cout << "  1. Requires ::type (verbose)" << std::endl;
    std::cout << "  2. No support for over-aligned types" << std::endl;
    std::cout << "  3. Trivially-copyable requirement unclear" << std::endl;

    std::cout << "\nReplacement:" << std::endl;
    std::cout << "  alignas(T) unsigned char buffer[sizeof(T)];" << std::endl;
    std::cout << std::endl;
    std::cout << "  // Instead of:" << std::endl;
    std::cout << "  std::aligned_storage<sizeof(T), alignof(T)>::type buffer;" << std::endl;

    // Modern way
    struct Example {
        double a, b, c;
    };

    // OLD (deprecated):
    std::aligned_storage<sizeof(Example), alignof(Example)>::type old_storage;

    // NEW (preferred):
    alignas(Example) unsigned char new_storage[sizeof(Example)];

    std::cout << "\nModern approach works the same:" << std::endl;
    auto* obj = new (&new_storage) Example{1.0, 2.0, 3.0};
    std::cout << "a=" << obj->a << " b=" << obj->b << " c=" << obj->c << std::endl;
    obj->~Example();
}