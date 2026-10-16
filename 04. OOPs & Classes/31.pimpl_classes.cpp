import std;
#include "class_header.h"

// Basic PIMPL with classes
class Widget2 {
public:
    Widget2();
    ~Widget2();

    // Copy/Move (must define where Impl is complete)
    Widget2(const Widget2&);
    Widget2& operator=(const Widget2&);
    Widget2(Widget2&&) noexcept;
    Widget2& operator=(Widget2&&) noexcept;

    // Public interface
    void do_work();
    int get_value() const;
    void set_value(int v);

private:
    struct Impl;  // Forward declaration
    std::unique_ptr<Impl> pimpl_;
};

// Implementation (could be in .cpp file)
struct Widget2::Impl {
    int value = 42;
    std::vector<double> history;
    std::string name = "widget";

    void internal_method() {
        std::cout << "  Internal method, value = " << value << std::endl;
        history.push_back(value);
    }
};

// Constructor/Destructor must be in .cpp (not header)
Widget2::Widget2() : pimpl_(std::make_unique<Impl>()) {}
Widget2::~Widget2() = default;
Widget2::Widget2(const Widget2& other)
    : pimpl_(std::make_unique<Impl>(*other.pimpl_)) {
}
Widget2& Widget2::operator=(const Widget2& other) {
    *pimpl_ = *other.pimpl_;
    return *this;
}
Widget2::Widget2(Widget2&&) noexcept = default;
Widget2& Widget2::operator=(Widget2&&) noexcept = default;

void Widget2::do_work() { pimpl_->internal_method(); }
int Widget2::get_value() const { return pimpl_->value; }
void Widget2::set_value(int v) { pimpl_->value = v; }

void PimplBasics() {
    std::cout << "=== PIMPL with Classes ===" << std::endl;

    Widget2 w;
    std::cout << "Initial value: " << w.get_value() << std::endl;

    w.set_value(100);
    std::cout << "New value: " << w.get_value() << std::endl;

    w.do_work();

    std::cout << "\nPIMPL structure:" << std::endl;
    std::cout << "  Header: class definition + forward declaration" << std::endl;
    std::cout << "  Source: struct Impl + all method definitions" << std::endl;
}

// PIMPL benefits
void PimplBenefits() {
    std::cout << "\n=== PIMPL Benefits ===" << std::endl;

    std::cout << "1. Compile firewall:" << std::endl;
    std::cout << "   Implementation changes don't recompile users" << std::endl;
    std::cout << std::endl;

    std::cout << "2. ABI stability:" << std::endl;
    std::cout << "   Can change internals without breaking ABI" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Header hygiene:" << std::endl;
    std::cout << "   Don't leak implementation includes" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Hidden dependencies:" << std::endl;
    std::cout << "   Users don't see implementation details" << std::endl;
}

// Fast PIMPL (with inline forwarding)
class FastWidget {
public:
    explicit FastWidget(int initial = 0)
        : impl_(std::make_unique<Impl>(initial)) {
    }

    // Inline forwarding for hot path
    int get() const { return impl_->value; }
    void set(int v) { impl_->value = v; }
    void increment() { ++impl_->value; }

    // Larger operations go through impl
    void complex_operation() { impl_->complex(); }

private:
    struct Impl {
        int value;
        std::vector<int> cache;

        explicit Impl(int v) : value(v) {}

        void complex() {
            cache.push_back(value);
            std::cout << "  Complex operation, cache size: "
                << cache.size() << std::endl;
        }
    };

    std::unique_ptr<Impl> impl_;
};

void FastPimpl() {
    std::cout << "\n=== Fast PIMPL ===" << std::endl;

    std::cout << "For performance-critical parts:" << std::endl;
    std::cout << "  Inline the forwarding functions" << std::endl;
    std::cout << "  Only use impl-> for complex operations" << std::endl;

    FastWidget fw(42);
    std::cout << "Initial: " << fw.get() << std::endl;

    fw.increment();
    fw.increment();
    std::cout << "After increments: " << fw.get() << std::endl;

    fw.complex_operation();
}

// PIMPL with shared state
class SharedStateWidget {
    struct Impl {
        int counter = 0;
        std::string state = "initial";
    };

    std::shared_ptr<Impl> impl_;

public:
    SharedStateWidget() : impl_(std::make_shared<Impl>()) {}

    // Note: default copy shares state
    void increment() { ++impl_->counter; }
    int get_counter() const { return impl_->counter; }
    void set_state(std::string s) { impl_->state = std::move(s); }
    const std::string& get_state() const { return impl_->state; }
};

void PimplSharedState() {
    std::cout << "\n=== PIMPL with Shared State ===" << std::endl;

    std::cout << "shared_ptr pimpl:" << std::endl;
    std::cout << "  Copies share state (reference semantics)" << std::endl;

    SharedStateWidget w1;
    w1.increment();
    w1.increment();

    SharedStateWidget w2 = w1;  // Shares impl
    w2.increment();

    std::cout << "w1 counter: " << w1.get_counter() << std::endl;
    std::cout << "w2 counter: " << w2.get_counter() << std::endl;
    std::cout << "(Both see the same state)" << std::endl;
}

// When to use PIMPL
void WhenToUsePimpl() {
    std::cout << "\n=== When to Use PIMPL ===" << std::endl;

    std::cout << "Good use cases:" << std::endl;
    std::cout << "  - Public library interfaces" << std::endl;
    std::cout << "  - ABI stability required" << std::endl;
    std::cout << "  - Heavy implementation includes" << std::endl;
    std::cout << "  - Reducing compile times" << std::endl;

    std::cout << "\nPoor use cases:" << std::endl;
    std::cout << "  - Performance-critical classes" << std::endl;
    std::cout << "  - Simple data holders" << std::endl;
    std::cout << "  - Internal implementation details" << std::endl;
    std::cout << "  - Value semantics needed" << std::endl;
}