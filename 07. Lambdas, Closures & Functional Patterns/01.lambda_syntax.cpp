/*
 * ============================================================================
 * LAMBDA SYNTAX - THE FOUNDATION
 * ============================================================================
 *
 * WHAT IS A LAMBDA?
 * -----------------
 * A lambda expression (or "lambda") is an anonymous function object
 * that you can define inline, right where you need it. Before C++11,
 * if you wanted to pass a small piece of behavior to an algorithm like
 * std::sort or std::find_if, you had to define a whole named function
 * or functor class somewhere else in your code. Lambdas let you write
 * that behavior directly at the call site.
 *
 * THE ANATOMY OF A LAMBDA:
 * -----------------------
 *
 *   [ capture_clause ] ( parameters ) mutable -> return_type { body }
 *     |                   |            |          |         |
 *     |                   |            |          |         The actual code
 *     |                   |            |          |         that runs
 *     |                   |            |          |
 *     |                   |            |          Allows modifying captured
 *     |                   |            |          values (captured by value
 *     |                   |            |          are const by default)
 *     |                   |            |
 *     |                   |            Optional! Compiler usually deduces it
 *     |                   |            from the return statements
 *     |                   |
 *     |                   Just like a normal function's parameters.
 *     |                   Can be empty: ()
 *     |
 *     Which variables from the surrounding scope the lambda can access.
 *     Can be [] (capture nothing), [=] (everything by value),
 *     [&] (everything by reference), or explicit [x, &y]
 *
 * SIMPLEST POSSIBLE LAMBDA:
 * ------------------------
 *   [] {}
 *
 * This is a lambda that captures nothing, takes no parameters,
 * and does nothing. Completely useless, but valid!
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include "functional_header.h"

void LambdaAnatomy() {
    std::cout << "=== Lambda Syntax Explained ===" << std::endl;

    // PART 1: The absolute simplest lambda
    // [] = capture nothing, () = no parameters, {} = do nothing
    [] {}();
    std::cout << "  Called the simplest lambda: [] {}" << std::endl;

    // PART 2: A lambda that takes parameters
    // Just like a regular function, you pass arguments
    // The 'auto' here is C++14's generic lambda feature
    // (explained in a later file - for now think of it as "any type")
    auto add = [](int a, int b) {
        return a + b;   // Return type is DEDUCED by the compiler (int)
        };

    std::cout << "  add(3, 4) = " << add(3, 4) << std::endl;

    // PART 3: Explicit return type with -> syntax
    // Sometimes the compiler can't deduce, or you want a different type
    // Here we force the division to return double even for int inputs
    auto divide = [](int a, int b) -> double {
        return static_cast<double>(a) / b;
        };

    std::cout << "  divide(7, 2) = " << divide(7, 2) << " (not 3!)" << std::endl;

    // PART 4: Multiple return statements
    // The compiler deduces from ALL return statements - they must agree
    auto classify = [](int n) -> const char* {
        if(n < 0) return "negative";
        if(n == 0) return "zero";
        return "positive";
        };

    std::cout << "  classify(-5) = " << classify(-5) << std::endl;
    std::cout << "  classify(0) = " << classify(0) << std::endl;
}

void TheClosureObject() {
    std::cout << "\n=== What a Lambda Actually IS ===" << std::endl;

    /*
     * IMPORTANT CONCEPT:
     * =================
     * When you write a lambda, the compiler generates an UNNAMED CLASS
     * behind the scenes. This class:
     *   - Stores the captured variables as member fields
     *   - Has an operator() that contains your lambda body
     *
     * So this lambda:
     *   auto f = [](int x) { return x * 2; };
     *
     * Is roughly equivalent to the compiler generating:
     *
     *   class __SomeGeneratedName {
     *   public:
     *       int operator()(int x) const {
     *           return x * 2;
     *       }
     *   };
     *   auto f = __SomeGeneratedName{};
     *
     * This is why lambdas are sometimes called "closure objects" or
     * "functors" - they are objects that behave like functions.
     */

    auto doubler = [](int x) { return x * 2; };

    // You can call it like a function:
    std::cout << "  doubler(21) = " << doubler(21) << std::endl;

    // You can also call it explicitly through operator() (proof it's an object):
    std::cout << "  doubler.operator()(21) = " << doubler.operator()(21) << std::endl;

    // Its size tells us about the generated class
    // (a lambda that captures nothing is typically EMPTY - the compiler
    //  optimizes it to take no space via the Empty Base Optimization)
    std::cout << "  sizeof(doubler) = " << sizeof(doubler)
        << " (empty - captures nothing)" << std::endl;
}

void LambdaVariations() {
    std::cout << "\n=== Lambda Syntax Variations ===" << std::endl;

    // Variation 1: No parameters - the () is optional!
    // C++23 made () even more optional in more cases
    auto greet = [] { return "Hello!"; };  // Note: no ()
    std::cout << "  greet() = " << greet() << std::endl;

    // Variation 2: Multiple parameters
    auto sum3 = [](int a, int b, int c) { return a + b + c; };
    std::cout << "  sum3(1, 2, 3) = " << sum3(1, 2, 3) << std::endl;

    // Variation 3: Const parameters (just like regular functions)
    auto process = [](const std::string& s) { return s.size(); };
    std::cout << "  process(\"hello\") = " << process("hello") << std::endl;

    // Variation 4: Default arguments (C++14+, with limitations)
    // Note: default arguments in lambdas have some restrictions
    // and are generally discouraged - prefer overloading or optional

    // Variation 5: Variadic parameters (C++14 generic lambdas)
    // The '...' here means "any number of arguments of any type"
    auto count_args = [](auto... args) {
        // sizeof...(args) is a compile-time count of arguments
        return sizeof...(args);
        };
    std::cout << "  count_args(1, 2.5, 'x') = " << count_args(1, 2.5, 'x') << std::endl;
}

void ImmediatelyExplainedConcepts() {
    std::cout << "\n=== Key Concepts Summary ===" << std::endl;

    std::cout << R"(
  LAMBDA = anonymous function defined inline

  WHY USE LAMBDAS?
  1. Less code - behavior lives right where it's used
  2. No pollution of namespace with helper functions
  3. Can capture local state (closures)
  4. The compiler can INLINE them for zero overhead

  WHAT THE COMPILER GENERATES:
  - An unnamed class with operator()
  - Captured variables become member fields
  - The lambda itself is an INSTANCE of that class

  COMMON USES:
  - Predicates for algorithms (std::sort, std::find_if)
  - Callbacks for event handlers
  - Custom deleters for smart pointers
  - Thread functions
  - Comparators in maps/sets
)" << std::endl;
}