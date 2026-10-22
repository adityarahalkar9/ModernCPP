import std;

/*
 * ============================================================================
 * INIT CAPTURES (C++14) - CAPTURING WITH EXPRESSIONS
 * ============================================================================
 *
 * WHAT ARE INIT CAPTURES?
 * =======================
 * Introduced in C++14, init captures (also called "generalized lambda
 * captures") let you create NEW variables inside the capture clause,
 * initialized with any expression.
 *
 * SYNTAX:
 * =======
 *   [name = expression]() { use name here }
 *
 * This is equivalent to:
 *   auto name = expression;   // Outside the lambda
 *   [name]() { ... }          // Capture that variable
 *
 * But it's more powerful because 'expression' can be anything:
 * - A move: [ptr = std::move(unique_ptr)]
 * - A computation: [doubled = x * 2]
 * - A new object: [cache = std::make_shared<Data>()]
 *
 * WHY THIS IS HUGE:
 * =================
 * Before C++14, you COULDN'T move objects into a lambda.
 * The only options were [x] (copy) or [&x] (reference).
 * This was a major limitation for move-only types like
 * unique_ptr - you couldn't capture them at all!
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "functional_header.h"

void InitCaptureBasics() {
    std::cout << "=== Init Capture Basics ===" << std::endl;

    int x = 21;

    // OLD WAY (C++11): Create a variable outside, then capture it
    int doubled = x * 2;
    auto old_way = [doubled]() { return doubled; };

    // NEW WAY (C++14): Initialize in the capture clause itself
    // 'doubled' is created INSIDE the lambda, initialized to x * 2
    auto new_way = [doubled = x * 2]() {
        return doubled;  // 'doubled' is 42
        };

    std::cout << "  old_way() = " << old_way() << std::endl;
    std::cout << "  new_way() = " << new_way() << std::endl;

    /*
     * The expression is EVALUATED ONCE, when the lambda is CREATED.
     * The result is stored in the lambda object.
     * This is like a constructor initializer for the lambda.
     */
}

void TheMoveRevolution() {
    std::cout << "\n=== The Move Revolution ===" << std::endl;

    /*
     * THE PROBLEM INIT CAPTURES SOLVED:
     * =================================
     * std::unique_ptr is MOVE-ONLY (can't be copied).
     * Before C++14:
     *   auto ptr = std::make_unique<int>(42);
     *   auto lambda = [ptr]() {...};  // ERROR! Tries to COPY
     *   auto lambda = [&ptr]() {...}; // OK but dangerous (reference)
     *
     * There was NO WAY to transfer ownership into a lambda!
     *
     * C++14 SOLUTION:
     * ===============
     * [owned = std::move(ptr)] transfers ownership INTO the lambda.
     * The lambda now OWNS the unique_ptr. When the lambda is destroyed,
     * the pointed-to object is automatically freed.
     */

    auto ptr = std::make_unique<int>(42);

    // Move the unique_ptr INTO the lambda - lambda takes ownership
    auto owner = [owned = std::move(ptr)]() {
        return *owned;  // Dereferencing the lambda's OWN unique_ptr
        };

    std::cout << "  owner() = " << owner() << std::endl;

    // The original ptr is now EMPTY (moved-from state)
    std::cout << "  Original ptr is null: " << (ptr == nullptr) << std::endl;

    /*
     * This pattern is EVERYWHERE in modern C++:
     * - Async callbacks that own resources
     * - Thread functions with exclusive ownership
     * - Callbacks stored in containers
     */
}

void ComplexCaptures() {
    std::cout << "\n=== Complex Init Captures ===" << std::endl;

    // CAPTURE 1: Computed values
    int base = 10;
    auto with_computed = [squared = base * base,
        cubed = base * base * base]() {
        return squared + cubed;
        };
    std::cout << "  Computed: " << with_computed() << std::endl;

    // CAPTURE 2: New objects created in the lambda
    auto with_new_object = [data = std::vector<int>{1, 2, 3, 4, 5}]() {
        int sum = 0;
        for(int v : data) sum += v;
        return sum;
        };
    std::cout << "  New object: " << with_new_object() << std::endl;

    // CAPTURE 3: shared_ptr for shared ownership
    auto shared_data = std::make_shared<std::string>("shared resource");
    std::cout << "  use_count before: " << shared_data.use_count() << std::endl;

    auto lambda_with_shared = [resource = shared_data]() {
        return resource->size();  // Both the lambda and outside code share this
        };

    std::cout << "  use_count after capture: " << shared_data.use_count() << std::endl;
    std::cout << "  Lambda uses shared: " << lambda_with_shared() << std::endl;

    // CAPTURE 4: Combining init captures with regular captures
    int multiplier = 3;
    auto mixed = [multiplier,              // Regular by-value capture
        value = 100]() mutable { // Init capture
        return value * multiplier;
        };
    std::cout << "  Mixed captures: " << mixed() << std::endl;
}

void PracticalPatterns() {
    std::cout << "\n=== Practical Init Capture Patterns ===" << std::endl;

    // PATTERN 1: Thread with its own resource
    // Each thread gets its own copy of expensive state
    auto create_worker = [](int worker_id) {
        // Each lambda gets its own buffer - no sharing, no races
        return [worker_id, buffer = std::vector<int>{}]() mutable {
            buffer.push_back(worker_id);
            return buffer.size();
            };
        };

    auto worker1 = create_worker(1);
    auto worker2 = create_worker(2);

    worker1(); worker1(); worker1();
    worker2();

    std::cout << "  Worker1 buffer size: " << worker1() << std::endl;
    std::cout << "  Worker2 buffer size: " << worker2() << std::endl;

    // PATTERN 2: Timer/Scoped logger
    auto make_scoped_timer = [start = std::chrono::steady_clock::now()]() {
        return [start]() {
            auto elapsed = std::chrono::steady_clock::now() - start;
            return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            };
        };

    auto timer = make_scoped_timer();
    std::cout << "  Timer created (microseconds since: " << timer() << ")" << std::endl;

    // PATTERN 3: Configuration with defaults
    auto make_processor = [config = std::string("default")]() mutable {
        return [config]() mutable -> std::string& {
            return config;  // Can modify the configuration
            };
        };

    auto processor = make_processor();
    processor() = "custom";
    std::cout << "  Config after modification: " << processor() << std::endl;
}