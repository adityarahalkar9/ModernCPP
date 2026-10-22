import std;
/*
 * ============================================================================
 * RECURSIVE LAMBDAS - LAMBDAS THAT CALL THEMSELVES
 * ============================================================================
 *
 * THE PROBLEM:
 * ============
 * A lambda doesn't have a NAME. How can it call itself if it
 * doesn't know what it's called?
 *
 *   auto fact = [](int n) {
 *       return n <= 1 ? 1 : n * fact(n - 1);  // ERROR: 'fact' unknown!
 *   };
 *
 * SOLUTIONS (from worst to best):
 * ===============================
 * 1. std::function (works but has overhead)
 * 2. The Y-combinator (functional programming trick)
 * 3. C++23: Deducing this (the clean solution!)
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <functional>
#include "functional_header.h"

void StdFunctionApproach() {
    std::cout << "=== Recursive Lambda: std::function ===" << std::endl;

    /*
     * APPROACH 1: std::function
     * =========================
     * The trick: declare the std::function FIRST,
     * then assign a lambda to it that captures the std::function.
     *
     * The lambda captures 'fact' by reference.
     * When the lambda calls 'fact(...)', it calls itself through
     * the std::function indirection.
     */

     // Step 1: Declare a std::function that takes int, returns int
    std::function<int(int)> fact;

    // Step 2: Assign a lambda that captures 'fact' by reference
    // The lambda can now call itself THROUGH the captured reference
    fact = [&fact](int n) -> int {
        if(n <= 1) return 1;
        return n * fact(n - 1);  // Recursive call through std::function
        };

    // Step 3: Use it
    std::cout << "  5! = " << fact(5) << std::endl;
    std::cout << "  10! = " << fact(10) << std::endl;

    /*
     * HOW IT WORKS:
     * =============
     * When you call fact(5):
     *   1. The std::function invokes the lambda
     *   2. The lambda needs fact(4)
     *   3. It calls through the captured 'fact' reference
     *   4. That's the same std::function, invoking the lambda again
     *
     * THE COST:
     * =========
     * std::function has OVERHEAD:
     * - Heap allocation (possibly)
     * - Indirection (virtual call or function pointer)
     * - Prevents inlining
     *
     * For performance-critical recursion, use a regular function.
     */
}

void YCombinator() {
    std::cout << "\n=== Recursive Lambda: Y-Combinator ===" << std::endl;

    /*
     * APPROACH 2: The Y-Combinator (Advanced!)
     * ========================================
     * This is a classic result from lambda calculus.
     * It lets you create recursion WITHOUT naming the function.
     *
     * The idea: instead of the function calling ITSELF by name,
     * you pass the function TO ITSELF as a parameter.
     *
     * This is more of a theoretical curiosity in C++, but it's
     * good to know it exists.
     */

     // The Y-combinator pattern in C++:
     // A function that takes a function, and returns a function
     // that applies the original function recursively

    auto make_recursive = [](auto&& f) {
        // This returns a lambda that calls f with itself
        return [f](auto&&... args) {
            return f(f, std::forward<decltype(args)>(args)...);
            };
        };

    // Use it: factorial
    // The inner lambda receives ITSELF as the first parameter
    auto factorial_impl = [](auto self, int n) -> int {
        if(n <= 1) return 1;
        // 'self' is this same lambda - we can recurse through it
        return n * self(self, n - 1);
        };

    // 'make_recursive' wraps it so we don't need to pass 'self' manually
    auto factorial = make_recursive(factorial_impl);

    std::cout << "  6! = " << factorial(6) << std::endl;
    std::cout << "  0! = " << factorial(0) << std::endl;

    // Another example: fibonacci
    auto fib_impl = [](auto self, int n) -> int {
        if(n <= 1) return n;
        return self(self, n - 1) + self(self, n - 2);
        };
    auto fibonacci = make_recursive(fib_impl);

    std::cout << "  fib(10) = " << fibonacci(10) << std::endl;

    /*
     * HOW TO READ THE PATTERN:
     * ========================
     * [f](auto&&... args) {
     *     return f(f, args...);
     * }
     *
     * - Captures the implementation function 'f'
     * - When called, passes 'f' as the first argument to 'f'
     * - So 'f' receives ITSELF as its first parameter
     * - Inside 'f', the first parameter is called 'self'
     * - 'self' can be called recursively
     */
}

void Cxx23DeducingThis() {
    std::cout << "\n=== C++23: Deducing this (The Future!) ===" << std::endl;

    /*
     * APPROACH 3: C++23 'deducing this'
     * =================================
     * The CLEANEST solution, but requires C++23.
     *
     * C++23 allows 'this' as an explicit parameter,
     * making the self-reference type-safe and zero-cost.
     */

    std::cout << R"(
    // C++23 syntax (not yet widely supported):
    auto factorial = [](this auto self, int n) -> int {
        if (n <= 1) return 1;
        return n * self(n - 1);  // 'self' is properly typed!
    };

    // 'this auto self' means:
    //   - 'self' is a parameter
    //   - It receives the lambda itself
    //   - Its type is automatically deduced
    //
    // This is:
    //   + Zero overhead (no std::function indirection)
    //   + Type-safe (no Y-combinator gymnastics)
    //   + Can be inlined by the compiler
    //   + Works with method chaining and perfect forwarding
)" << std::endl;
}

void PracticalRecursion() {
    std::cout << "\n=== Practical Recursive Patterns ===" << std::endl;

    // PRACTICAL USE: Tree traversal with a recursive lambda
    struct Node {
        int value;
        std::vector<std::unique_ptr<Node>> children;

        explicit Node(int v) : value(v) {}
    };

    // Build a small tree
    auto root = std::make_unique<Node>(1);
    root->children.push_back(std::make_unique<Node>(2));
    root->children.push_back(std::make_unique<Node>(3));
    root->children[0]->children.push_back(std::make_unique<Node>(4));
    root->children[0]->children.push_back(std::make_unique<Node>(5));

    // Recursive lambda to sum all values in the tree
    // Uses std::function approach
    std::function<int(const Node*)> sum_tree;
    sum_tree = [&sum_tree](const Node* node) -> int {
        if(!node) return 0;
        int total = node->value;
        for(const auto& child : node->children) {
            total += sum_tree(child.get());
        }
        return total;
        };

    std::cout << "  Tree sum: " << sum_tree(root.get()) << std::endl;
    std::cout << "  (1+2+3+4+5 = 15)" << std::endl;

    // Performance note:
    std::cout << R"(
    WHEN TO USE RECURSIVE LAMBDAS vs REGULAR FUNCTIONS:
    ┌─────────────────────────────────────────────────────┐
    │ Use recursive lambda when:                         │
    │   • The recursion is LOCAL to one function          │
    │   • You don't want to pollute the namespace         │
    │   • Performance isn't critical                     │
    │                                                     │
    │ Use a regular function when:                       │
    │   • Performance matters (avoid std::function)      │
    │   • The recursion is used in multiple places       │
    │   • Deep recursion (stack overflow risk)          │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}