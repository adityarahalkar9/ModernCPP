import std;
/*
 * ============================================================================
 * LAMBDA CAPTURES - GIVING LAMBDAS ACCESS TO OUTSIDE VARIABLES
 * ============================================================================
 *
 * WHAT IS A CAPTURE?
 * ------------------
 * By default, a lambda CANNOT see any variables from the surrounding scope.
 * The capture clause [] is the gateway - it lists which outside variables
 * the lambda is allowed to use.
 *
 * THINK OF IT LIKE A PERMISSION SYSTEM:
 * -------------------------------------
 *   []        = "You may access NOTHING from outside"
 *   [x]       = "You may access x (a COPY of it)"
 *   [&x]      = "You may access x (the ACTUAL variable, not a copy)"
 *   [=]       = "You may access everything, but only copies"
 *   [&]       = "You may access everything, the actual variables"
 *   [&, x]    = "Everything by reference, except x which is a copy"
 *   [=, &x]   = "Everything by copy, except x which is by reference"
 *
 * WHY DOES THIS EXIST?
 * --------------------
 * Without captures, lambdas could only work with their parameters.
 * Captures turn a lambda into a CLOSURE - a function bundled with
 * the state it needs. This is a concept from functional programming
 * that enables powerful patterns like:
 *   - Counters that remember how many times they were called
 *   - Callbacks that "remember" configuration
 *   - Predicates configured at runtime
 *
 * ============================================================================
 */


#include <iostream>
#include <string>
#include <vector>
#include "functional_header.h"

void NoCaptureDefault() {
    std::cout << "=== Default: No Access to Outside ===" << std::endl;

    int outside = 42;

    // ERROR: This lambda cannot see 'outside'
    // The capture list [] explicitly says "capture nothing"
    //
    // auto bad = [] { return outside; };  // COMPILE ERROR!

    /*
     * WHY THIS DESIGN?
     * ================
     * The empty capture [] is like a sealed room - nothing gets in.
     * This is intentional: it makes lambdas predictable. If you see [],
     * you KNOW the lambda only works with its parameters. You don't
     * have to hunt through the surrounding code to understand it.
     */

    auto works = [](int x) { return x * 2; };  // Only uses its parameter
    std::cout << "  works(21) = " << works(21) << std::endl;
    std::cout << "  outside still = " << outside << " (untouched, unseen)" << std::endl;
}

void ExplicitCapture() {
    std::cout << "\n=== Explicit Captures ===" << std::endl;

    int value = 10;
    int another = 20;

    // Capture 'value' BY VALUE (makes a COPY inside the lambda)
    // The lambda now has its own private copy of 'value'
    auto by_value = [value]() {
        // Here, 'value' is a member of the lambda's generated class
        // It was COPIED from the outside when the lambda was CREATED
        return value;
        };

    // Capture 'another' BY REFERENCE (shares the actual variable)
    // The lambda holds a POINTER/REFERENCE to the outside variable
    auto by_reference = [&another]() {
        // Here, 'another' IS the outside variable - same memory
        return another;
        };

    // Demonstrate the difference:
    value = 100;       // Change the outside variables AFTER lambda creation
    another = 200;

    std::cout << "  by_value() = " << by_value()
        << "  (still 10 - it captured a COPY at creation time)" << std::endl;
    std::cout << "  by_reference() = " << by_reference()
        << "  (now 200 - it sees the CURRENT value)" << std::endl;

    // The by-reference lambda can even MODIFY the outside variable:
    auto modifier = [&another]() {
        another += 1000;  // Changes the actual outside variable!
        };
    modifier();
    std::cout << "  after modifier(): another = " << another << std::endl;
}

void CaptureAllSyntax() {
    std::cout << "\n=== Capture-All Shortcuts ===" << std::endl;

    int a = 1, b = 2, c = 3;
    std::string s = "hello";

    // [=] captures EVERYTHING used inside, by VALUE (copies)
    auto all_by_value = [=]() {
        // Can use a, b, s... but only the COPIES made at lambda creation
        return a + b + c + static_cast<int>(s.size());
        };

    // [&] captures EVERYTHING used inside, by REFERENCE (the real variables)
    auto all_by_ref = [&]() {
        return a + b + c;  // Uses the ACTUAL current values
        };

    // Mixed: default by-value, but 'a' by reference
    auto mixed = [=, &a]() {
        a = 100;           // OK: 'a' is by reference, can modify
        return b + c;      // b, c are copies (can't modify, but b+c works)
        // Note: we can't do b = 50 here - b is a const copy
        };

    std::cout << "  all_by_value() = " << all_by_value() << std::endl;
    std::cout << "  all_by_ref() = " << all_by_ref() << std::endl;

    // WARNING: [=] and [&] are convenient but DANGEROUS:
    // - [=] can accidentally capture LARGE objects (expensive copies)
    // - [&] can accidentally create DANGLING references
    //
    // BEST PRACTICE: Prefer explicit captures [x, &y] so readers
    // can see exactly what's captured and how.
}

void CaptureRules() {
    std::cout << "\n=== Capture Rules and Gotchas ===" << std::endl;

    /*
     * RULE 1: By-value captures are CONST by default
     * =====================================================
     * When you capture by value [x], the copy inside the lambda
     * is CONST. You cannot modify it without 'mutable' (next file).
     * This prevents accidental modification of what might look
     * like the original variable.
     */

    int counter = 0;
    // auto bad = [counter]() { ++counter; };  // ERROR: counter is const copy
    // Use 'mutable' to allow modification (see next file)

    /*
     * RULE 2: Captures happen at LAMBDA CREATION, not at CALL time
     * =====================================================
     * This is crucial and often misunderstood:
     */
    int x = 5;
    auto snapshot = [x]() { return x; };  // x is copied NOW (x=5)
    x = 100;                               // This change is NOT seen
    std::cout << "  snapshot() = " << snapshot() << " (5, not 100)" << std::endl;

    /*
     * RULE 3: Reference captures see CURRENT state
     * =====================================================
     * Unlike value captures, references track the variable:
     */
    auto tracker = [&x]() { return x; };   // References the actual x
    x = 999;
    std::cout << "  tracker() = " << tracker() << " (999 - tracks current)" << std::endl;

    /*
     * RULE 4: Only variables actually USED are captured by [=] or [&]
     * =====================================================
     * If you write [=] but only use 'x' inside, only 'x' is captured.
     * The compiler is smart - it captures what you USE, not everything
     * in scope. This saves memory.
     */
}

void WhenCapturesMatter() {
    std::cout << "\n=== Real-World Use Cases ===" << std::endl;

    // USE CASE 1: Configured predicate
    // A lambda that "remembers" a threshold for filtering
    int threshold = 50;
    std::vector<int> data = {10, 20, 60, 70, 30, 80};

    // The lambda captures 'threshold' - it's now a specialized filter
    auto is_above = [threshold](int value) {
        return value > threshold;
        };

    // Count how many are above threshold
    auto count = std::count_if(data.begin(), data.end(), is_above);
    std::cout << "  Values above " << threshold << ": " << count << std::endl;

    // USE CASE 2: Accumulator pattern
    // A lambda that builds up state across multiple calls
    int total = 0;
    auto add_to_total = [&total](int value) {
        total += value;  // Modifies the outside 'total'
        };

    add_to_total(10);
    add_to_total(20);
    add_to_total(30);
    std::cout << "  Accumulated total: " << total << std::endl;

    // USE CASE 3: Callback with context
    // Simulating an event handler that remembers configuration
    std::string user_name = "Alice";
    int retry_limit = 3;

    // This lambda "closes over" the configuration
    auto on_error = [&user_name, retry_limit]() {
        std::cout << "  [Handler] " << user_name
            << " hit error, max retries: " << retry_limit << std::endl;
        };

    on_error();  // The lambda has access to its "environment"
}