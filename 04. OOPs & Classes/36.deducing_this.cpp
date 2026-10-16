import std;
#include "class_header.h"

// C++23: Deducing this (explicit object parameter)
class Value2 {
public:
    // Traditional member function
    void traditional_method()& {
        std::cout << "  traditional_method on lvalue" << std::endl;
    }

    void traditional_method()&& {
        std::cout << "  traditional_method on rvalue" << std::endl;
    }

    // C++23: Explicit object parameter (deducing this)
    template <typename Self>
    auto&& modern_method(this Self&& self) {
        std::cout << "  modern_method called" << std::endl;

        if constexpr(std::is_lvalue_reference_v<Self&&>) {
            std::cout << "    (on lvalue)" << std::endl;
        }
        else {
            std::cout << "    (on rvalue)" << std::endl;
        }

        return std::forward<Self>(self);
    }
};

void DeducingThisBasics() {
    std::cout << "=== Deducing this (C++23) ===" << std::endl;

    std::cout << "'this' as explicit parameter:" << std::endl;
    std::cout << "  template <typename Self>" << std::endl;
    std::cout << "  auto method(this Self&& self)" << std::endl;

    Value2 v;

    std::cout << "\nTraditional approach:" << std::endl;
    v.traditional_method();
    Value2().traditional_method();

    std::cout << "\nC++23 approach:" << std::endl;
    v.modern_method();
    Value2().modern_method();
}

// Perfect forwarding with deducing this
class Chainable {
    int value = 0;

public:
    Chainable() = default;

    // With deducing this, can perfectly forward
    template <typename Self>
    auto& increment(this Self&& self) {
        ++self.value;
        return self;  // Perfect forwarding preserved
    }

    template <typename Self>
    auto& add(this Self&& self, int amount) {
        self.value += amount;
        return self;
    }

    int get() const { return value; }
};

void ChainingWithDeducingThis() {
    std::cout << "\n=== Chaining with Deducing this ===" << std::endl;

    std::cout << "Method chaining preserves value category:" << std::endl;

    Chainable c;
    c.increment().add(10).increment();
    std::cout << "  Final value: " << c.get() << std::endl;
}

// Deduplicating code
class WithoutDeducing {
public:
    // Without deducing this: need separate implementations
    void process()& {
        std::cout << "  Processing lvalue: " << data << std::endl;
        // Common logic
    }

    void process()&& {
        std::cout << "  Processing rvalue: " << data << std::endl;
        // Same logic duplicated!
    }

private:
    std::string data = "example";
};

class WithDeducing {
public:
    template <typename Self>
    void process(this Self&& self) {
        // One implementation handles both cases
        std::cout << "  Processing: " << self.data;
        if constexpr(std::is_rvalue_reference_v<Self&&>) {
            std::cout << " (rvalue - can steal resources)";
        }
        else {
            std::cout << " (lvalue - must copy)";
        }
        std::cout << std::endl;
    }

private:
    std::string data = "example";
};

void DeduplicatingCode() {
    std::cout << "\n=== Deduplicating Code ===" std::endl;

    std::cout << "Problem without deducing this:" << std::endl;
    std::cout << "  const/non-const overloads" << std::endl;
    std::cout << "  lvalue/rvalue overloads" << std::endl;
    std::cout << "  Code duplication" << std::endl;

    WithDeducing wd;
    wd.process();           // lvalue
    WithDeducing().process(); // rvalue
}

// SFINAE-free overloading
class Processor {
public:
    // Without deducing this, need enable_if or overloads
    template <typename T>
    void old_process(T& t, std::enable_if_t<std::is_integral_v<T>, int> = 0) {
        std::cout << "  Old integral processing" << std::endl;
    }

    // With deducing this, can use if constexpr
    template <typename Self, typename T>
    void new_process(this Self&& self, T&& t) {
        if constexpr(std::is_integral_v<std::remove_reference_t<T>>) {
            std::cout << "  New integral processing" << std::endl;
        }
        else if constexpr(std::is_floating_point_v<std::remove_reference_t<T>>) {
            std::cout << "  New floating-point processing" << std::endl;
        }
        else {
            std::cout << "  Generic processing" << std::endl;
        }
    }
};

void SfinaeFree() {
    std::cout << "\n=== SFINAE-Free Overloading ===" << std::endl;

    std::cout << "Deducing this enables cleaner code:" << std::endl;
    std::cout << "  No SFINAE needed" << std::endl;
    std::cout << "  if constexpr for branching" << std::endl;

    Processor p;
    p.new_process(42);
    p.new_process(3.14);
    p.new_process("string");
}