import std;
#include "structs_header.h"

// PIMPL (Pointer to Implementation) idiom
// Public interface struct
class Widget {
public:
    Widget();  // Constructor must be defined where Impl is complete
    ~Widget(); // Destructor must be defined where Impl is complete

    // Non-copyable (or implement copy with deep copy)
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    // Movable
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    // Public interface
    void do_something();
    int get_value() const;
    void set_value(int v);

private:
    struct Impl;  // Forward declaration
    std::unique_ptr<Impl> pimpl_;
};

// Implementation struct (private)
struct Widget::Impl {
    int value = 42;
    std::vector<int> data;
    std::string name = "widget";

    void internal_method() {
        std::cout << "  Internal method called, value = " << value << std::endl;
    }
};

// Constructor/Destructor must be defined here (not in header)
Widget::Widget() : pimpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::do_something() {
    pimpl_->internal_method();
    pimpl_->data.push_back(pimpl_->value);
}

int Widget::get_value() const { return pimpl_->value; }
void Widget::set_value(int v) { pimpl_->value = v; }

void PimplBasics() {
    std::cout << "=== PIMPL Idiom ===" << std::endl;

    Widget w;
    std::cout << "Initial value: " << w.get_value() << std::endl;

    w.set_value(100);
    std::cout << "New value: " << w.get_value() << std::endl;

    w.do_something();

    std::cout << "\nPIMPL benefits:" << std::endl;
    std::cout << "  - Hides implementation details" << std::endl;
    std::cout << "  - ABI stability (can change impl without recompiling users)" << std::endl;
    std::cout << "  - Reduces compile-time dependencies" << std::endl;
    std::cout << "  - Faster compilation (fewer includes needed)" << std::endl;

    std::cout << "\nPIMPL costs:" << std::endl;
    std::cout << "  - Heap allocation for impl" << std::endl;
    std::cout << "  - Extra indirection on every access" << std::endl;
    std::cout << "  - More boilerplate code" << std::endl;
}

// Fast PIMPL variant
class FastWidget {
public:
    FastWidget() : impl_(std::make_unique<Impl>()) {}

    // Inline functions that forward to impl
    int get_value() const { return impl_->value; }
    void set_value(int v) { impl_->value = v; }

private:
    struct Impl {
        int value = 42;
        double cached_result = 0.0;
        std::vector<double> history;
    };

    std::unique_ptr<Impl> impl_;
};

void FastPimpl() {
    std::cout << "\n=== Fast PIMPL ===" << std::endl;

    FastWidget fw;
    fw.set_value(99);
    std::cout << "Value: " << fw.get_value() << std::endl;

    std::cout << "\nFor hot-path code, consider:" << std::endl;
    std::cout << "  - Inline forwarding functions" << std::endl;
    std::cout << "  - Template-based polymorphism instead" << std::endl;
    std::cout << "  - Direct implementation (no pimpl)" << std::endl;
}

// PIMPL with shared state
class SharedWidget {
public:
    SharedWidget() : impl_(std::make_shared<Impl>()) {}

    // Note: Default copy shares state!
    int get_counter() const { return impl_->counter; }
    void increment() { ++impl_->counter; }

private:
    struct Impl {
        int counter = 0;
    };
    std::shared_ptr<Impl> impl_;  // shared_ptr allows copying
};

void SharedPimpl() {
    std::cout << "\n=== PIMPL with Shared State ===" << std::endl;

    SharedWidget w1;
    w1.increment();
    w1.increment();

    SharedWidget w2 = w1;  // Now they share state!
    w2.increment();

    std::cout << "w1 counter: " << w1.get_counter() << std::endl;
    std::cout << "w2 counter: " << w2.get_counter() << std::endl;
    std::cout << "(They share the same state)" << std::endl;

    std::cout << "\nBe careful: shared_ptr pimpl changes copy semantics" << std::endl;
    std::cout << "Copies will share state (reference semantics)" << std::endl;
}

// When to use PIMPL
void WhenToUsePimpl() {
    std::cout << "\n=== When to Use PIMPL ===" << std::endl;

    std::cout << "Good use cases:" << std::endl;
    std::cout << "  - Library interfaces (ABI stability)" << std::endl;
    std::cout << "  - Reducing header dependencies" << std::endl;
    std::cout << "  - Hiding complex implementation" << std::endl;
    std::cout << "  - Plugin architectures" << std::endl;

    std::cout << "\nPoor use cases:" << std::endl;
    std::cout << "  - Performance-critical code" << std::endl;
    std::cout << "  - Simple data structures" << std::endl;
    std::cout << "  - Value semantics required" << std::endl;
    std::cout << "  - Small, stable interfaces" << std::endl;
}