import std;
/*
 * ============================================================================
 * std::function - THE UNIVERSAL CALLABLE WRAPPER
 * ============================================================================
 *
 * WHAT IS std::function?
 * ======================
 * std::function is a container for ANY callable thing:
 * - Regular functions
 * - Lambdas (with or without captures)
 * - Function objects (functors)
 * - Member function pointers
 * - Bind expressions
 *
 * It provides a UNIFORM interface: you can store any callable
 * that matches a specific signature, regardless of what it actually is.
 *
 * THE TRADE-OFF (CRITICAL TO UNDERSTAND):
 * =======================================
 * std::function uses TYPE ERASURE. This means:
 *   + Flexibility: store ANY callable
 *   - Performance: virtual call indirection
 *   - Possible heap allocation
 *   - Cannot be inlined
 *
 * WHEN TO USE std::function:
 * ==========================
 * + When you need to STORE a callback for later
 * + When the callable type varies at runtime
 * + When you need a uniform interface (containers of callbacks)
 *
 * WHEN TO AVOID:
 * =============
 * - Performance-critical code (use templates/auto instead)
 * - When you know the exact type (use auto)
 * - Hot loops
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include "functional_header.h"

void BasicStdFunction() {
    std::cout << "=== std::function Basics ===" << std::endl;

    // DECLARING: std::function<RETURN_TYPE(PARAM_TYPES)>
    std::function<int(int, int)> operation;

    // Can store a REGULAR FUNCTION:
    operation = [](int a, int b) { return a + b; };   // Lambda
    std::cout << "  As lambda: " << operation(10, 20) << std::endl;

    // Can store a LAMBDA WITH CAPTURES:
    int multiplier = 3;
    operation = [multiplier](int a, int b) {
        return (a + b) * multiplier;  // Lambda with state!
        };
    std::cout << "  With captures: " << operation(10, 20) << std::endl;

    // Can store a FUNCTION POINTER:
    int (*raw_func)(int, int) = [](int a, int b) { return a * b; };
    operation = raw_func;
    std::cout << "  From function pointer: " << operation(10, 20) << std::endl;

    /*
     * KEY INSIGHT:
     * ============
     * The SAME std::function variable held three DIFFERENT types
     * of callables. This is impossible with 'auto' because auto
     * fixes the type at initialization.
     */
}

void StoringCallbacks() {
    std::cout << "\n=== The Primary Use: Storing Callbacks ===" << std::endl;

    /*
     * THE #1 USE CASE: Callback storage
     * =================================
     * When you need to "save" a callable for later,
     * std::function is your container.
     */

     // Event system: store handlers for later invocation
    class EventEmitter {
        std::map<std::string, std::vector<std::function<void()>>> handlers_;

    public:
        void on(const std::string& event, std::function<void()> handler) {
            handlers_[event].push_back(std::move(handler));
        }

        void emit(const std::string& event) {
            std::cout << "  [Emitting: " << event << "]" << std::endl;
            if(auto it = handlers_.find(event); it != handlers_.end()) {
                for(auto& handler : it->second) {
                    handler();  // Call each stored callback
                }
            }
        }
    };

    EventEmitter emitter;

    // Store various callbacks - they can capture state!
    int call_count = 0;

    emitter.on("start", [&call_count]() {
        ++call_count;
        std::cout << "    Start handler (call #" << call_count << ")" << std::endl;
               });

    emitter.on("start", []() {
        std::cout << "    Another start handler" << std::endl;
               });

    emitter.on("stop", [message = "Shutting down"]() {
        std::cout << "    " << message << std::endl;
               });

    // Trigger the events - stored lambdas are invoked
    emitter.emit("start");
    emitter.emit("start");
    emitter.emit("stop");

    std::cout << "  Total start calls: " << call_count << std::endl;
}

void FunctionVsAuto() {
    std::cout << "\n=== std::function vs auto ===" << std::endl;

    /*
     * THE CRITICAL DISTINCTION:
     * =========================
     * auto: Compile-time polymorphism
     *   - The type is FIXED at initialization
     *   - Zero overhead - can be inlined
     *   - Cannot change what it holds later
     *
     * std::function: Runtime polymorphism
     *   - Can hold different things over time
     *   - Has overhead (indirection, possible allocation)
     *   - Cannot be inlined (usually)
     */

     // AUTO: Type fixed, zero overhead
    auto lambda1 = [](int x) { return x * 2; };
    auto lambda2 = [](int x) { return x * 3; };

    // These have DIFFERENT types! (each lambda gets its own class)
    static_assert(!std::is_same_v<decltype(lambda1), decltype(lambda2)>);

    // std::function: Can hold either, has overhead
    std::function<int(int)> f;
    f = lambda1;  // Now holds lambda1
    std::cout << "  f (holds lambda1): " << f(21) << std::endl;

    f = lambda2;  // NOW holds lambda2 - type erasure in action!
    std::cout << "  f (now holds lambda2): " << f(21) << std::endl;

    /*
     * PERFORMANCE GUIDANCE:
     * ====================
     * Use auto when:
     *   - The lambda is used immediately
     *   - You don't need to store it
     *   - Performance matters
     *
     * Use std::function when:
     *   - You need to STORE the callable
     *   - The callable type varies
     *   - You need containers of callbacks
     */
}

void PerformanceConsiderations() {
    std::cout << "\n=== Performance Details ===" << std::endl;

    // SIZE: std::function is larger than a raw lambda
    auto small_lambda = [](int x) { return x; };
    std::function<int(int)> small_func = small_lambda;

    std::cout << "  sizeof(lambda) = " << sizeof(small_lambda) << std::endl;
    std::cout << "  sizeof(std::function) = " << sizeof(small_func) << std::endl;

    /*
     * WHAT THE EXTRA SIZE IS FOR:
     * ===========================
     * std::function typically contains:
     * - A pointer to a "manager" function (for type erasure)
     * - Storage for the callable (either inline small buffer
     *   or pointer to heap allocation)
     * - Invoke function pointer
     *
     * THE COST PER CALL:
     * =================
     * 1. Indirect call through function pointer (no inlining)
     * 2. Possible cache miss (code is elsewhere)
     * 3. If capturing large state: heap allocation at construction
     *
     * IN HOT LOOPS: This can be 10-100x slower than a direct call.
     */

    std::cout << R"(
    BENCHMARK MENTAL MODEL:
    ┌───────────────────────┬─────────┐
    │ Direct call (inlined) │    1x   │
    ├───────────────────────┼─────────┤
    │ Raw lambda via auto   │   ~1x   │
    ├───────────────────────┼─────────┤
    │ Virtual function      │  2-5x   │
    ├───────────────────────┼─────────┤
    │ std::function         │  5-20x  │
    ├───────────────────────┼─────────┤
    │ Heap + std::function  │ 10-50x  │
    └───────────────────────┴─────────┘
    (Very rough estimates - depends on context)
)" << std::endl;
}