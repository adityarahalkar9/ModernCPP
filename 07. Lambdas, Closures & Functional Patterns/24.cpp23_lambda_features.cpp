import std;
#include <iostream>
#include <string>
#include "functional_header.h"

void AttributeOnLambdas() {
    std::cout << "=== C++23: Attributes on Lambdas ===" << std::endl;

    /*
       WHAT'S NEW:
       You can now put attributes directly on lambdas:
         auto f = [] [[nodiscard]] () { return 42; };
      
       The [[nodiscard]] attribute warns if you ignore the return value.
     */

     // Before C++23, this wasn't possible on the lambda itself.
     // C++23 syntax:
     // auto important = [] [[nodiscard]] () { return compute_value(); };

    std::cout << R"(
    AVAILABLE ATTRIBUTES:
    ┌─────────────────────────────────────────────────┐
    │ [[nodiscard]] - Warn if return value ignored    │
    │ [[maybe_unused]] - Suppress unused warnings     │
    └─────────────────────────────────────────────────┘

    USE CASE:
    • Lambdas that compute important values
    • Preventing accidental discard of results
)" << std::endl;
}

void StaticOperator() {
    std::cout << "\n=== C++23: static operator() ===" << std::endl;

    /*
       WHAT'S NEW:
       Lambdas can now have a STATIC operator():
         auto f = [] static () { ... };
      
       This means the call doesn't need a 'this' pointer.
       Useful for stateless lambdas.
     */

    std::cout << R"(
    BENEFITS:
    ┌─────────────────────────────────────────────────┐
    │ • No 'this' pointer needed                      │
    │ • Slightly smaller generated code               │
    │ • Clearer intent: "this lambda has no state"    │
    │ • Can be called without an instance             │
    └─────────────────────────────────────────────────┘

    SYNTAX (C++23):
    auto no_state = [] static () { return 42; };

    WHEN TO USE:
    • Stateless lambdas (no captures)
    • Performance-sensitive call sites
    • When you want to explicitly show "no state"
)" << std::endl;
}

void DeducingThisInLambdas() {
    std::cout << "\n=== C++23: Deducing this in Lambdas ===" << std::endl;

    /*
       THE BIG ONE: Recursive lambdas made easy!
      
       C++23 allows 'this' as an explicit parameter:
         auto f = [](this auto self, int n) {
             return n <= 1 ? 1 : n * self(n - 1);
         };
      
       'self' receives the lambda itself as a parameter.
       This enables CLEAN RECURSION without std::function!
     */

    std::cout << R"(
    THE RECURSION SOLUTION WE'VE BEEN WAITING FOR:

    BEFORE (painful):
    ┌─────────────────────────────────────────────────┐
    │ std::function<int(int)> fact;                   │
    │ fact = [&fact](int n) {                         │
    │     return n <= 1 ? 1 : n * fact(n - 1);       │
    │ };                                              │
    │ // Has std::function overhead                   │
    │ // Easy to get dangling references wrong        │
    └─────────────────────────────────────────────────┘

    C++23 (elegant):
    ┌─────────────────────────────────────────────────┐
    │ auto fact = [](this auto self, int n) {         │
    │     return n <= 1 ? 1 : n * self(n - 1);       │
    │ };                                              │
    │ // Zero overhead                                │
    │ // Type-safe                                     │
    │ // Can be inlined                               │
    │ // No std::function needed                       │
    └─────────────────────────────────────────────────┘

    HOW IT WORKS:
    • 'this auto self' declares a parameter named 'self'
    • Its type is deduced (the lambda's own type)
    • Calling self(...) invokes the same lambda
    • This is like a member function receiving 'this'
)" << std::endl;
}

void OtherImprovements() {
    std::cout << "\n=== Other C++23 Lambda Improvements ===" << std::endl;

    std::cout << R"(
    MORE C++23 GOODNESS:
    ┌─────────────────────────────────────────────────┐
    │                                                 │
    │ 1. Optional parentheses more often             │
    │    [] { ... }  (was already allowed)           │
    │    Now allowed in more contexts                │
    │                                                 │
    │ 2. Lambda packs in more places                 │
    │    Better support for variadic lambdas         │
    │                                                 │
    │ 3. If a lambda has no captures,                │
    │    it's implicitly constexpr                   │
    │    (if the body allows)                        │
    │                                                 │
    │ 4. Better constexpr support                    │
    │    Lambdas usable in constexpr contexts         │
    │                                                 │
    └─────────────────────────────────────────────────┘

    COMPILER SUPPORT NOTE:
    • C++23 features are still being implemented
    • Check your compiler's support status
    • MSVC often leads, GCC/Clang catching up
    • Use feature-test macros:
      #ifdef __cpp_static_call_operator
      // ... use static operator()
      #endif
)" << std::endl;
}