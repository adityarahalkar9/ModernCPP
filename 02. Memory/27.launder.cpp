#include <iostream>
#include <new>
#include <string>
#include "memory_header.h"

// std::launder basics
// Tells compiler: "This memory now holds a different object"
void LaunderBasics() {
    std::cout << "=== std::launder ===" << std::endl;

    std::cout << "Purpose: Access an object placed in storage" << std::endl;
    std::cout << "where a different object existed before" << std::endl;

    std::cout << "\nWhen needed:" << std::endl;
    std::cout << "  - Placement new over existing object" << std::endl;
    std::cout << "  - Reusing storage for new object" << std::endl;
    std::cout << "  - Accessing after const/mutable changes" << std::endl;
}

// The classic use case
struct Widget {
    int value;

    Widget(int v) : value(v) {
        std::cout << "  Widget(" << v << ") constructed" << std::endl;
    }

    ~Widget() {
        std::cout << "  Widget(" << value << ") destroyed" << std::endl;
    }
};

void ClassicUseCase() {
    std::cout << "\n=== Classic Use Case ===" << std::endl;

    alignas(Widget) unsigned char buffer[sizeof(Widget)];

    // First object
    Widget* w1 = new (buffer) Widget(1);
    std::cout << "  w1->value = " << w1->value << std::endl;

    // Destroy first object
    w1->~Widget();

    // Create second object in same storage
    Widget* w2 = new (buffer) Widget(2);
    std::cout << "  w2->value = " << w2->value << std::endl;

    // PROBLEM: Compiler might still use cached info about w1
    // This is where std::launder helps:

    Widget* w2_laundered = std::launder(w2);
    std::cout << "  laundered->value = " << w2_laundered->value << std::endl;

    w2->~Widget();
}

// Launder with const members
struct ConstMember {
    const int value;  // Const member!

    ConstMember(int v) : value(v) {}
};

void LaunderWithConst() {
    std::cout << "\n=== Launder with const ===" << std::endl;

    alignas(ConstMember) unsigned char buffer[sizeof(ConstMember)];

    // First object with value 10
    new (buffer) ConstMember(10);
    auto* p1 = std::launder(reinterpret_cast<ConstMember*>(buffer));
    std::cout << "  p1->value = " << p1->value << std::endl;

    p1->~ConstMember();

    // "Change" const member by creating new object
    new (buffer) ConstMember(20);
    auto* p2 = std::launder(reinterpret_cast<ConstMember*>(buffer));
    std::cout << "  p2->value = " << p2->value << std::endl;

    // Without launder, compiler might assume value never changes
    // (since it's const). Launder tells it to re-read.

    p2->~ConstMember();
}

// When you DON'T need launder
void WhenNotNeeded() {
    std::cout << "\n=== When NOT Needed ===" << std::endl;

    std::cout << "You DON'T need launder when:" << std::endl;
    std::cout << "  1. Using placement new for FIRST time in storage" << std::endl;
    std::cout << "  2. Object has no const/reference members" << std::endl;
    std::cout << "  3. You always access through the new pointer" << std::endl;
    std::cout << "  4. Storage is fresh (never had an object)" << std::endl;

    std::cout << "\nYou DO need launder when:" << std::endl;
    std::cout << "  1. Reusing storage for new object type" << std::endl;
    std::cout << "  2. Object has const or reference members" << std::endl;
    std::cout << "  3. Accessing through old pointer to new object" << std::endl;
    std::cout << "  4. Using std::optional-like patterns manually" << std::endl;
}

// Common pattern: manual optional
template <typename T>
class ManualOptional {
public:
    ManualOptional() : engaged_(false) {}

    ~ManualOptional() {
        if(engaged_) {
            destroy();
        }
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if(engaged_) destroy();
        new (storage()) T(std::forward<Args>(args)...);
        engaged_ = true;
    }

    void destroy() {
        if(engaged_) {
            get()->~T();
            engaged_ = false;
        }
    }

    T* get() {
        return engaged_ ? std::launder(reinterpret_cast<T*>(storage())) : nullptr;
    }

    const T* get() const {
        return engaged_ ? std::launder(reinterpret_cast<const T*>(storage())) : nullptr;
    }

    T& operator*() { return *get(); }
    const T& operator*() const { return *get(); }

    bool has_value() const { return engaged_; }

private:
    void* storage() { return buffer_; }
    const void* storage() const { return buffer_; }

    alignas(T) unsigned char buffer_[sizeof(T)];
    bool engaged_;
};

void ManualOptionalDemo() {
    std::cout << "\n=== Manual Optional (using launder) ===" << std::endl;

    ManualOptional<std::string> opt;

    std::cout << "Has value: " << opt.has_value() << std::endl;

    opt.emplace("Hello");
    std::cout << "Value: " << *opt << std::endl;

    // Replace with new value (reuses storage)
    opt.emplace("World");
    std::cout << "New value: " << *opt << std::endl;

    std::cout << "std::launder makes this well-defined" << std::endl;
}