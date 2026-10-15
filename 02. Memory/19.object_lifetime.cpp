#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "memory_header.h"

// Object lifetime tracking
class LifetimeTracker {
public:
    explicit LifetimeTracker(std::string name) : name_(std::move(name)) {
        std::cout << "  [" << name_ << "] constructed at " << this << std::endl;
    }

    ~LifetimeTracker() {
        std::cout << "  [" << name_ << "] destroyed at " << this << std::endl;
    }

    LifetimeTracker(const LifetimeTracker& other)
        : name_(other.name_ + "_copy") {
        std::cout << "  [" << name_ << "] copy-constructed at " << this << std::endl;
    }

    LifetimeTracker(LifetimeTracker&& other) noexcept
        : name_(std::move(other.name_)) {
        other.name_ += "_moved_from";
        std::cout << "  [" << name_ << "] move-constructed at " << this << std::endl;
    }

private:
    std::string name_;
};

void BasicLifetime() {
    std::cout << "=== Basic Object Lifetime ===" << std::endl;

    std::cout << "Creating object in scope:" << std::endl;
    {
        LifetimeTracker obj("local");
        std::cout << "  (inside scope)" << std::endl;
    }
    std::cout << "  (scope ended)" << std::endl;
}

// Temporary object lifetimes
void TemporaryLifetimes() {
    std::cout << "\n=== Temporary Object Lifetimes ===" << std::endl;

    std::cout << "Creating temporary:" << std::endl;
    LifetimeTracker("temporary");  // Destroyed at end of full expression

    std::cout << "\nTemporary in expression:" << std::endl;
    auto get_tracker = []() -> LifetimeTracker {
        return LifetimeTracker("returned");
        };

    // Temporary destroyed at end of statement
    get_tracker();
    std::cout << "  (temporary destroyed)" << std::endl;

    // Lifetime extension with reference
    std::cout << "\nLifetime extension:" << std::endl;
    {
        const LifetimeTracker& ref = get_tracker();
        std::cout << "  (using reference to temporary)" << std::endl;
    }  // Temporary destroyed here (end of reference's scope)
    std::cout << "  (reference scope ended)" << std::endl;
}

// Lifetime in containers
void ContainerLifetimes() {
    std::cout << "\n=== Lifetimes in Containers ===" << std::endl;

    std::vector<LifetimeTracker> vec;

    std::cout << "Emplacing into vector:" << std::endl;
    vec.emplace_back("first");

    std::cout << "Adding second (may cause reallocation):" << std::endl;
    vec.emplace_back("second");  // First is MOVED to new memory

    std::cout << "Vector contents:" << std::endl;
    std::cout << "  Clearing vector:" << std::endl;
    vec.clear();  // Destroys elements, doesn't free capacity
}

// Lifetime with std::optional
void OptionalLifetime() {
    std::cout << "\n=== Lifetime with std::optional ===" << std::endl;

    std::optional<LifetimeTracker> opt;

    std::cout << "Engaging optional:" << std::endl;
    opt.emplace("optional_value");

    std::cout << "Resetting optional:" << std::endl;
    opt.reset();  // Destroys contained object

    std::cout << "Re-engaging:" << std::endl;
    opt.emplace("new_value");

    std::cout << "Optional destroyed at scope end:" << std::endl;
}

// Lifetime and dynamic allocation
void DynamicLifetime() {
    std::cout << "\n=== Dynamic Lifetime ===" << std::endl;

    std::cout << "Creating with new:" << std::endl;
    LifetimeTracker* ptr = new LifetimeTracker("heap");

    std::cout << "Deleting manually:" << std::endl;
    delete ptr;

    // With smart pointer
    std::cout << "\nWith unique_ptr:" << std::endl;
    {
        auto smart = std::make_unique<LifetimeTracker>("smart");
        std::cout << "  (using smart pointer)" << std::endl;
    }  // Automatically deleted

    std::cout << "Smart pointer cleaned up automatically" << std::endl;
}

// Lifetime issues
void LifetimeIssues() {
    std::cout << "\n=== Lifetime Issues ===" << std::endl;

    // Dangling reference to temporary
    auto get_string = []() -> std::string {
        return "temporary string";
        };

    // BAD: Dangling reference
    // const std::string& bad = get_string() + " extended";
    // std::cout << bad;  // UB: temporary destroyed

    // GOOD: Value semantics
    std::string good = get_string() + " extended";
    std::cout << "Good: " << good << std::endl;

    // Returning reference to local
    // auto bad_func = []() -> const int& {
    //     int local = 42;
    //     return local;  // Dangling!
    // };

    // std::string_view dangers
    std::string str = "Hello";
    std::string_view sv = str;
    str = "Modified";  // May invalidate sv if reallocation occurs
    // std::cout << sv;  // Potentially dangling

    std::cout << "Be careful with string_view and reallocation" << std::endl;
}

// Construction/destruction order
void ConstructionOrder() {
    std::cout << "\n=== Construction/Destruction Order ===" << std::endl;

    std::cout << "Members are constructed in declaration order:" << std::endl;
    std::cout << "Destroyed in REVERSE order" << std::endl;

    struct Ordered {
        LifetimeTracker first;
        LifetimeTracker second;
        LifetimeTracker third;

        Ordered()
            : third("third"), second("second"), first("first") {
            // Note: Initialization list order doesn't matter
            // Members are ALWAYS constructed in declaration order
        }
    };

    std::cout << "Creating Ordered struct:" << std::endl;
    Ordered o;

    std::cout << "Destroying Ordered struct:" << std::endl;
}