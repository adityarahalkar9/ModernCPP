import std;
#include "smart_pointers_header.h"

class Widget {
public:
    explicit Widget(int id) : id_(id) {
        std::cout << "  Widget(" << id_ << ") constructed" << std::endl;
    }

    ~Widget() {
        std::cout << "  Widget(" << id_ << ") destroyed" << std::endl;
    }

    int id() const { return id_; }

    // Move-only type
    Widget(Widget&&) = default;
    Widget& operator=(Widget&&) = default;
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

private:
    int id_;
};

// Returning unique_ptr from factory
std::unique_ptr<Widget> create_widget(int id) {
    return std::make_unique<Widget>(id);
}

void UniquePtrFromFactory() {
    std::cout << "=== unique_ptr Factory Functions ===" << std::endl;

    std::cout << "Factory creates and returns ownership:" << std::endl;
    auto w = create_widget(42);
    std::cout << "  Got widget with id: " << w->id() << std::endl;
}

// Passing unique_ptr to functions (transfer ownership)
void consume_widget(std::unique_ptr<Widget> w) {
    std::cout << "  Consuming widget " << w->id() << std::endl;
    // Widget destroyed when function exits
}

void observe_widget(const Widget& w) {
    std::cout << "  Observing widget " << w.id() << std::endl;
    // No ownership involved
}

void UniquePtrToFunctions() {
    std::cout << "\n=== Passing unique_ptr to Functions ===" << std::endl;

    std::cout << "Pass by value (transfers ownership):" << std::endl;
    auto w = create_widget(10);
    consume_widget(std::move(w));  // Must explicitly move
    std::cout << "  After consume, w is null: " << (w == nullptr) << std::endl;

    std::cout << "\nPass by reference (no ownership):" << std::endl;
    auto w2 = create_widget(20);
    observe_widget(*w2);  // Borrow, don't transfer
    std::cout << "  w2 still owns: " << w2->id() << std::endl;
}

// unique_ptr in containers
void UniquePtrInContainers() {
    std::cout << "\n=== unique_ptr in Containers ===" << std::endl;

    std::vector<std::unique_ptr<Widget>> widgets;

    // Add elements (moves happen automatically)
    std::cout << "Adding widgets:" << std::endl;
    widgets.push_back(create_widget(1));
    widgets.push_back(create_widget(2));
    widgets.emplace_back(3);

    std::cout << "\nContainer contents:" << std::endl;
    for(const auto& w : widgets) {
        std::cout << "  Widget id: " << w->id() << std::endl;
    }

    // Vector growth moves all elements
    std::cout << "\nAdding more (may cause reallocation):" << std::endl;
    widgets.reserve(10);  // Prevent reallocation for demo
    widgets.push_back(create_widget(4));

    // Remove element (destroys it)
    std::cout << "\nRemoving first widget:" << std::endl;
    widgets.erase(widgets.begin());
}

// unique_ptr with inheritance
class Base {
public:
    virtual ~Base() { std::cout << "  Base destroyed" << std::endl; }
    virtual void describe() const { std::cout << "  Base" << std::endl; }
};

class Derived : public Base {
public:
    ~Derived() override { std::cout << "  Derived destroyed" << std::endl; }
    void describe() const override { std::cout << "  Derived" << std::endl; }
};

void UniquePtrPolymorphism() {
    std::cout << "\n=== unique_ptr Polymorphism ===" << std::endl;

    std::cout << "Storing derived in base unique_ptr:" << std::endl;
    std::unique_ptr<Base> poly = std::make_unique<Derived>();
    poly->describe();

    std::cout << "\nVector of polymorphic pointers:" << std::endl;
    std::vector<std::unique_ptr<Base>> objects;
    objects.push_back(std::make_unique<Derived>());
    objects.push_back(std::make_unique<Base>());

    for(const auto& obj : objects) {
        obj->describe();
    }
}

// Conditional ownership
void ConditionalOwnership() {
    std::cout << "\n=== Conditional Ownership ===" << std::endl;

    bool take_ownership = true;

    std::unique_ptr<Widget> owner;

    if(take_ownership) {
        owner = create_widget(100);
        std::cout << "  Owner has widget: " << owner->id() << std::endl;
    }

    // Owner automatically cleaned up
    std::cout << "  Scope ending..." << std::endl;
}

// unique_ptr with scope guards
void ScopeGuardPattern() {
    std::cout << "\n=== Scope Guard Pattern ===" << std::endl;

    std::cout << "RAII ensures cleanup even with early returns:" << std::endl;

    auto process = [](int value) -> bool {
        auto resource = std::make_unique<Widget>(value);

        if(value < 0) {
            std::cout << "  Negative value, returning early" << std::endl;
            return false;  // Resource still cleaned up!
        }

        if(value > 100) {
            std::cout << "  Too large, returning early" << std::endl;
            return false;  // Resource still cleaned up!
        }

        std::cout << "  Processing " << value << std::endl;
        return true;
        };

    process(50);
    process(-10);
    process(200);
}