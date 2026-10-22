import std;
/*
 * ============================================================================
 * LAMBDAS AND 'this' - CAPTURE IN MEMBER FUNCTIONS
 * ============================================================================
 *
 * THE PROBLEM:
 * ============
 * When you write a lambda inside a CLASS MEMBER FUNCTION, you often
 * need access to the class's member variables. But 'this' is a POINTER,
 * and member variables belong to the OBJECT, not the function.
 *
 * HOW TO CAPTURE MEMBERS:
 * =======================
 * Option 1: [this] - captures the 'this' POINTER by value
 *           The lambda can access members through this->member
 *
 * Option 2: [*this] (C++17) - captures a COPY OF THE ENTIRE OBJECT
 *           The lambda has its own snapshot of the object
 *
 * Option 3: [&] - implicitly captures 'this' (same as [this])
 *
 * Option 4: Capture individual members (C++14 init captures)
 *
 * THE LIFETIME DANGER (CRITICAL!):
 * ================================
 * [this] captures the POINTER - not the object itself.
 * If the OBJECT is destroyed before the lambda runs,
 * the lambda has a DANGLING 'this' pointer.
 *
 * This is THE most common source of lambda-related crashes!
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include "functional_header.h"

 // A class that demonstrates the different capture strategies
class Widget {
public:
    explicit Widget(std::string name) : name_(std::move(name)), value_(42) {
        std::cout << "  Widget('" << name_ << "') constructed" << std::endl;
    }

    ~Widget() {
        std::cout << "  Widget('" << name_ << "') destroyed" << std::endl;
    }

    // METHOD 1: Capture 'this' (the default with [&])
    // The lambda holds a pointer to this specific object
    void demonstrate_this_capture() {
        // [this] stores the pointer - members accessed via the pointer
        auto get_name_via_this = [this]() {
            // 'this->' is implicit here, but we could write this->name_
            return name_ + " (value: " + std::to_string(value_) + ")";
            };

        std::cout << "  [this] lambda: " << get_name_via_this() << std::endl;

        // WARNING: If this Widget is destroyed, the lambda has a dangling pointer!
        // The lambda does NOT keep the object alive.
    }

    // METHOD 2: Capture a copy of the entire object (C++17)
    void demonstrate_copy_capture() {
        // [*this] copies the ENTIRE OBJECT into the lambda
        // The lambda now has its own independent Widget
        auto snapshot = [*this]() {
            // 'this' inside this lambda points to the COPY, not the original
            return name_ + " (snapshot, value: " + std::to_string(value_) + ")";
            };

        // Even if the original is destroyed, the snapshot survives
        std::cout << "  [*this] lambda: " << snapshot() << std::endl;
    }

    // METHOD 3: Capture specific members with init captures (C++14)
    void demonstrate_member_capture() {
        // Copy specific members - no dangling risk, no full object copy
        auto safe_lambda = [name = name_,          // Copy the string
            val = value_]() {       // Copy the int
            return name + " (captured members only)";
            };

        std::cout << "  [member] lambda: " << safe_lambda() << std::endl;
    }

    void modify_value(int new_val) { value_ = new_val; }

private:
    std::string name_;
    int value_;
};

void ThisCaptureBasics() {
    std::cout << "=== 'this' Capture in Classes ===" << std::endl;

    Widget w("test_widget");
    w.demonstrate_this_capture();
    w.demonstrate_copy_capture();
    w.demonstrate_member_capture();
}

void TheDanglingThisProblem() {
    std::cout << "\n=== THE DANGLING 'this' PROBLEM ===" << std::endl;

    /*
     * THE MOST COMMON LAMBDA CRASH PATTERN:
     * ====================================
     *
     * class Handler {
     *     void setup() {
     *         callback_ = [this]() {
     *             process();  // Uses 'this' - the object pointer
     *         };
     *         // Lambda stored, will be called LATER
     *     }
     *     void process() { /* uses members *\/ }
     *     std::function<void()> callback_;
     * };
     *
     * {
     *     Handler h;
     *     h.setup();
     * }  // h destroyed here - callback_ has dangling 'this'!
     *
     * // Later, somewhere:
     * some_callback();  // CRASH! 'this' points to dead object
     */

     // SAFE VERSION: The lambda keeps the object alive
     // Using shared_ptr and enable_shared_from_this
    class SafeHandler : public std::enable_shared_from_this<SafeHandler> {
    public:
        void setup() {
            // Capture a shared_ptr - keeps the object ALIVE
            auto self = shared_from_this();
            callback_ = [self]() {       // 'self' is a shared_ptr copy
                self->process();          // Safe - object guaranteed alive
                };
        }

        void run() { if(callback_) callback_(); }

    private:
        void process() {
            std::cout << "  SafeHandler::process() - object is alive!" << std::endl;
        }
        std::function<void()> callback_;
    };

    // Create via make_shared (required for enable_shared_from_this)
    auto handler = std::make_shared<SafeHandler>();
    handler->setup();

    // Even if we drop our reference, the lambda's captured 'self'
    // keeps the object alive until the callback completes
    std::weak_ptr<SafeHandler> weak = handler;
    handler.reset();  // Give up our reference

    if(auto locked = weak.lock()) {
        // Object still alive because the lambda holds a shared_ptr
        locked->run();
        std::cout << "  (Object survived because lambda holds shared_ptr)" << std::endl;
    }
}

void CaptureStrategies() {
    std::cout << "\n=== Capture Strategy Decision Guide ===" << std::endl;

    std::cout << R"(
    CAPTURING CLASS MEMBERS IN LAMBDAS:

    ┌──────────────┬─────────────────────────────────────────────────┐
    │  [this]      │  Holds a POINTER to the object                  │
    │              │  + Cheap (8 bytes)                             │
    │              │  + Sees CURRENT member values                  │
    │              │  - DANGER: Dangling if object dies              │
    │              │  Use when: lambda used within object's lifetime │
    ├──────────────┼─────────────────────────────────────────────────┤
    │  [*this]     │  Copies the ENTIRE OBJECT                       │
    │  (C++17)     │  + Safe from dangling                          │
    │              │  + Snapshot frozen at capture time              │
    │              │  - Expensive (copies whole object)              │
    │              │  - Changes to original not seen                 │
    │              │  Use when: lambda outlives object, small object │
    ├──────────────┼─────────────────────────────────────────────────┤
    │ [member=...] │  Copies SPECIFIC MEMBERS (C++14 init capture)   │
    │              │  + Safe, selective, efficient                   │
    │              │  + No dangling risk                             │
    │              │  - Must list members explicitly                 │
    │              │  Use when: you only need a few members          │
    ├──────────────┼─────────────────────────────────────────────────┤
    │ [self =      │  Holds a SHARED_PTR (keeps object alive)        │
    │  shared_from │  + SAFEST for long-lived lambdas                │
    │  _this()]    │  + Object guaranteed alive while lambda exists  │
    │              │  - Requires shared_ptr management               │
    │              │  Use when: lambda stored, called later/async    │
    └──────────────┴─────────────────────────────────────────────────┘
)" << std::endl;
}