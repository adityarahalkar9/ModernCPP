import std;
/*
 * ============================================================================
 * CAPTURE BY VALUE - DEEP DIVE
 * ============================================================================
 *
 * WHAT HAPPENS WHEN YOU CAPTURE BY VALUE?
 * =======================================
 * When you write [x] or [=], the compiler:
 *   1. Creates a member variable in the lambda's generated class
 *   2. COPIES the value of x into that member at LAMBDA CREATION time
 *   3. The lambda now has its own independent snapshot
 *
 * THE "SNAPSHOT" METAPHOR:
 * =======================
 * Think of by-value capture like taking a PHOTOGRAPH of the variable.
 * The photo shows what the variable looked like at that moment.
 * Later changes to the original don't affect the photo.
 *
 * WHEN TO USE BY-VALUE:
 * ====================
 * + When the lambda might OUTLIVE the captured variable
 *   (e.g., lambda stored for later, variable goes out of scope)
 * + When you want a SNAPSHOT that won't change
 * + For small types (int, pointers, etc.) - cheap to copy
 * + When you need thread-safety (each thread's lambda has its own copy)
 *
 * WHEN TO AVOID:
 * =============
 * - Large objects (expensive copy)
 * - When you need CURRENT state (not a snapshot)
 * - Objects that can't be copied (use reference or pointer)
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include "functional_header.h"

void ValueCaptureSemantics() {
    std::cout << "=== By-Value Capture Semantics ===" << std::endl;

    std::string message = "Hello";

    // CAPTURE TIME: The copy happens HERE, when the lambda is CREATED
    auto snapshot_lambda = [message]() {
        return message;  // This is the COPY, not the original
        };

    // Change the original AFTER the lambda was created
    message = "World";

    // The lambda still has the old value - it took a "photograph"
    std::cout << "  Original message: " << message << " (World)" << std::endl;
    std::cout << "  Lambda's message: " << snapshot_lambda()
        << " (Hello - the snapshot)" << std::endl;

    /*
     * KEY INSIGHT:
     * ============
     * The copy is made ONCE, at lambda creation. Not at every call.
     * The lambda stores the value as a member of its generated class:
     *
     *   class LambdaType {
     *       std::string message;  // <- the captured copy
     *   public:
     *       std::string operator()() const { return message; }
     *   };
     */
}

void ValueCaptureCost() {
    std::cout << "\n=== The Cost of By-Value Capture ===" << std::endl;

    // SMALL TYPE: by-value is essentially free
    int small = 42;
    auto small_lambda = [small]() { return small; };
    std::cout << "  sizeof(small_lambda) = " << sizeof(small_lambda)
        << " (stores one int)" << std::endl;

    // LARGE TYPE: by-value copies the ENTIRE object into the lambda
    std::string large(1000, 'x');  // 1000-character string
    auto large_lambda = [large]() { return large.size(); };

    // The lambda is now HUGE - it contains a copy of the 1000-char string
    std::cout << "  sizeof(large_lambda) = " << sizeof(large_lambda)
        << " bytes (contains copy of 1000-char string!)" << std::endl;

    // POINTER capture is cheap (8 bytes) but has different semantics
    // (the pointer is copied, but it still POINTS to the original object)
    std::string* ptr = &large;
    auto ptr_lambda = [ptr]() { return ptr->size(); };
    std::cout << "  sizeof(ptr_lambda) = " << sizeof(ptr_lambda)
        << " (just a pointer, but see: " << ptr_lambda() << ")" << std::endl;

    /*
     * WARNING: [=] with large objects in scope
     * =======================================
     * If you write auto f = [=]() {...} and the lambda uses a
     * 100KB object, you've just made a 100KB copy.
     *
     * BEST PRACTICE: Be explicit about what you capture.
     * [specific_var] is better than [=] for large scopes.
     */
}

void ValueCaptureLifetime() {
    std::cout << "\n=== Lifetime Safety of By-Value ===" << std::endl;

    /*
     * THE BIG ADVANTAGE: By-value captures are SAFE for later use
     * ================================================================
     * If you capture by value, the lambda OWNS its copy.
     * Even if the original variable is destroyed, the lambda's copy
     * is still valid. This is critical for:
     *   - Callbacks stored and called later
     *   - Lambdas passed to other threads
     *   - Deferred execution
     */

     // This function returns a lambda that outlives its capture environment
    auto make_greeter = [](std::string name) {
        // 'name' parameter is a local variable...
        // ...but capturing it BY VALUE copies it into the returned lambda
        return [name]() {   // 'name' copied into lambda's storage
            std::cout << "  Hello, " << name << "!" << std::endl;
            };
        // 'name' (the parameter) is destroyed here...
        // ...but the lambda's COPY lives on!
        };

    // Create the lambda - the parameter's lifetime ends,
    // but the lambda still works because it has its own copy
    auto greeter = make_greeter("Future");
    std::cout << "  (Parameter destroyed, but lambda still works:)" << std::endl;
    greeter();  // SAFE! The lambda has its own copy of "Future"

    /*
     * THIS WOULD BE DANGEROUS WITH [&]:
     * auto make_bad = []() {
     *     std::string local = "temp";
     *     return [&local]() { return local; };  // DANGER!
     *     // 'local' destroyed here, lambda has dangling reference!
     * };
     */
}

void ValueCaptureConst() {
    std::cout << "\n=== By-Value Captures are CONST ===" << std::endl;

    int counter = 0;

    // By-value captures are CONST inside the lambda
    // This is a deliberate design decision for safety:
    // - Prevents confusion: modifying the copy doesn't affect the original
    // - Makes it clear the lambda is "read-only" with respect to its captures
    auto reader = [counter]() {
        // counter = 5;  // ERROR! counter is const (without 'mutable')
        return counter + 1;  // OK: reading is fine
        };

    // To MODIFY a by-value capture, you need 'mutable' (next topic)
    auto mutable_example = [counter]() mutable {
        ++counter;  // OK with 'mutable' - but modifies the COPY only
        return counter;
        };

    std::cout << "  reader() = " << reader() << " (original untouched)" << std::endl;
    std::cout << "  mutable_example() = " << mutable_example()
        << " (modified the copy)" << std::endl;
    std::cout << "  Original counter still = " << counter << std::endl;
    // ^ The outside 'counter' was NEVER affected by either lambda
}