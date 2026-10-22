import std;
/*
   LAMBDA PITFALLS - COMMON MISTAKES AND HOW TO AVOID THEM
 
   This file catalogs the most common lambda-related bugs.
   Each one has caused countless real-world crashes.
  
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "functional_header.h"

void Pitfall1_DanglingReference() {
    std::cout << "=== PITFALL 1: Dangling Reference ===" << std::endl;

    /*
     * THE BUG: Lambda outlives the variable it references
     * ================================================================
     *
     * auto create_dangling = []() {
     *     int local = 42;
     *     return [&local]() {     // Captures reference to 'local'
     *         return local * 2;    // 'local' will be dead!
     *     };
     *     // 'local' destroyed here - lambda has dangling reference
     * };
     */

    std::cout << R"(
    SYMPTOMS:
    • Works in debug, crashes in release
    • Garbage values (reads freed memory)
    • Random crashes that "can't be reproduced"
    • Security vulnerabilities (use-after-free)

    THE FIX:
    • If lambda needs to outlive the variable → capture BY VALUE [x]
    • If you need shared ownership → use shared_ptr
    • Keep the lambda within the variable's lifetime

    PREVENTION:
    • Never return a lambda that captures by reference
    • Be suspicious of [&] in any function that returns a lambda
    • Use tools: AddressSanitizer catches these
)" << std::endl;
}

void Pitfall2_CapturedThis() {
    std::cout << "\n=== PITFALL 2: Dangling 'this' ===" << std::endl;

    /*
     * THE BUG: Lambda captures 'this', object is destroyed
     * ================================================================
     *
     * class Handler {
     *     void setup() {
     *         callback = [this]() {
     *             this->doWork();  // 'this' might be dead!
     *         };
     *     }
     *     std::function<void()> callback;
     * };
     *
     * // ... later ...
     * auto h = std::make_unique<Handler>();
     * h->setup();
     * h.reset();  // Handler destroyed
     * // callback still exists, 'this' is dangling!
     */

    std::cout << R"(
    THIS IS THE #1 SOURCE OF LAMBDA CRASHES!

    SYMPTOMS:
    • Crashes in callbacks "sometimes"
    • Works fine until the object is destroyed "too early"
    • Heisenbugs (disappear when you add logging)

    THE FIX:
    1. Keep object alive as long as lambda exists
    2. Use shared_ptr + enable_shared_from_this
    3. Capture by value [*this] (C++17) if object is small
    4. Disconnect callbacks when object is destroyed

    CODE PATTERN FOR SAFETY:
    ┌─────────────────────────────────────────────────┐
    │ class SafeHandler :                             │
    │     public std::enable_shared_from_this<SafeHandler> { │
    │     void setup() {                              │
    │         auto self = shared_from_this();          │
    │         callback = [self]() {                   │
    │             self->doWork();  // Object stays    │
    │         };                                      │
    │     }                                           │
    │ };                                              │
    └─────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall3_AccidentalCopy() {
    std::cout << "\n=== PITFALL 3: Accidental Expensive Copies ===" << std::endl;

    /*
     * THE BUG: [=] captures copies of large objects
     * ================================================================
     */

     // What looks innocent:
    std::string huge_string(1000, 'x');  // 1000-char string
    std::vector<int> big_vector(10000, 42);  // 10000 ints

    // This lambda captures COPIES of everything it uses
    auto lambda = [=]() {
        // If we use huge_string or big_vector here...
        // ...we've made EXPENSIVE COPIES into the lambda object!
        return huge_string.size() + big_vector.size();
        };

    std::cout << "  sizeof(huge_string) = " << sizeof(huge_string) << std::endl;
    std::cout << "  sizeof(big_vector) = " << sizeof(big_vector) << std::endl;
    std::cout << "  sizeof(lambda) = " << sizeof(lambda)
        << " (includes copies of captured data!)" << std::endl;

    std::cout << R"(
    SYMPTOMS:
    • Unexplained memory usage
    • Slow lambda creation
    • Large binary size

    THE FIX:
    • Be explicit: [specific_var] instead of [=]
    • For large objects: capture by reference (if lifetime is safe)
    • For read-only access: capture const reference [&var]
    • Consider if you really need the whole object

    BEST PRACTICE:
    ┌─────────────────────────────────────────────────┐
    │ NEVER use [=] in a scope with large objects    │
    │ Be explicit about what you capture             │
    │ Ask: "Do I need a copy or just a look?"        │
    └─────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall4_MutableSurprise() {
    std::cout << "\n=== PITFALL 4: mutable Copies Don't Affect Original ===" << std::endl;

    /*
     * THE CONFUSION: mutable modifies the COPY, not the original
     * ================================================================
     */

    int counter = 0;

    // This lambda increments... its own copy
    auto incrementer = [counter]() mutable {
        ++counter;  // Modifies the lambda's COPY
        return counter;
        };

    incrementer();
    incrementer();
    incrementer();

    std::cout << "  After 3 calls, lambda's counter: " << incrementer() << std::endl;
    std::cout << "  But original counter: " << counter << " (unchanged!)" << std::endl;

    // What people EXPECTED: counter to be 4
    // What HAPPENED: counter is still 0

    std::cout << R"(
    THE FIX:
    • If you want to modify the original → capture by reference [&counter]
    • If you want a private counter → mutable is correct, understand it's private

    WHEN TO USE WHICH:
    ┌─────────────────────────────────────────────────┐
    │ [counter]() mutable                            │
    │ → Lambda has its OWN counter (starts at 0)     │
    │ → Changes don't affect outside                 │
    │ → Each COPY of lambda has separate state        │
    │                                                 │
    │ [&counter]()                                   │
    │ → Lambda shares the outside counter            │
    │ → Changes ARE visible outside                  │
    │ → Risk: dangling if counter dies first         │
    └─────────────────────────────────────────────────┘
)" << std::endl;
}

void Pitfall5_CaptureInLoops() {
    std::cout << "\n=== PITFALL 5: Capturing Loop Variables ===" << std::endl;

    /*
     * THE BUG: Capturing loop variables incorrectly
     * ================================================================
     */

     // THE CLASSIC BUG (fixed in C++17 for init-statements):
    std::vector<std::function<void()>> callbacks;

    // BUG VERSION (what people used to write):
    // for (int i = 0; i < 3; ++i) {
    //     callbacks.push_back([&i]() {  // DANGER: reference to loop variable!
    //         std::cout << i << " ";    // All print the same value!
    //     });
    // }
    // // All callbacks print the FINAL value of i (3)

    // CORRECT VERSION: capture by value
    for(int i = 0; i < 3; ++i) {
        callbacks.push_back([i]() {  // Capture COPY of i at this moment
            std::cout << i << " ";
                            });
    }

    std::cout << "  Correct callbacks: ";
    for(auto& cb : callbacks) cb();
    std::cout << std::endl;

    // Even better C++17: use init-statement in the loop
    std::vector<std::function<void()>> better_callbacks;
    for(int i = 0; i < 3; ++i) {
        better_callbacks.push_back([current = i]() {  // Init capture: clear intent
            std::cout << current << " ";
                                   });
    }

    std::cout << "  Better callbacks: ";
    for(auto& cb : better_callbacks) cb();
    std::cout << std::endl;

    std::cout << R"(
    THE RULE:
    ┌─────────────────────────────────────────────────┐
    │ In loops, ALWAYS capture the loop variable      │
    │ BY VALUE (or with init-capture)                │
    │                                                 │
    │ NEVER: [&i] in a loop where the lambda         │
    │ outlives the iteration                         │
    └─────────────────────────────────────────────────┘
)" << std::endl;
}