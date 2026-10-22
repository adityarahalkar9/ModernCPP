import std;
/*
    IF CONSTEXPR - COMPILE-TIME BRANCHING
    
    WHAT IS if constexpr?
    A compile-time if statement. The condition must be known at compile time, and 
    only the TRUE branch is compiled.

    THE REVOLUTION:
    Before if constexpr, compile-time branching required:
     - Template specialization (verbose)
     - SFINAE (unreadable)
     - Tag dispatch (clever but complex)
 
    With if constexpr, it's just... an if statement!
 
    THE KEY DIFFERENCE FROM REGULAR if:
    - Regular if: BOTH branches are compiled, one is chosen at runtime.
    - if constexpr: Only the SELECTED branch is compiled. The other branch doesn't 
                    even exist in your binary!
 
    THIS ENABLES:
     - Code that would otherwise not compile in the discarded branch!
     - Template code that acts differently based on type properties
     - Zero-cost branching for known conditions
 */

import std;
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include "constexpr_header.h"

void IfConstexprBasics() {
    std::cout << "=== if constexpr Basics ===" << std::endl;

    /*
     * THE SYNTAX:
     * ================================================================
     * if constexpr (COMPILE_TIME_CONDITION) {
     *     // Only compiled if condition is TRUE
     * } else {
     *     // Only compiled if condition is FALSE
     * }
     *
     * The other branch is NOT COMPILED AT ALL!
     */

     // Example: Type-based branching
    auto describe_type = [](auto value) {
        if constexpr(std::is_integral_v<decltype(value)>) {
            return "integral type";
        }
        else if constexpr(std::is_floating_point_v<decltype(value)>) {
            return "floating point type";
        }
        else {
            return "other type";
        }
        };

    std::cout << "  " << describe_type(42) << std::endl;        // integral
    std::cout << "  " << describe_type(3.14) << std::endl;      // floating point
    std::cout << "  " << describe_type("hello") << std::endl;   // other

    /*
     * WHAT HAPPENS AT COMPILE TIME:
     * =============================
     * When you call describe_type(42):
     *   1. decltype(value) is int
     *   2. std::is_integral_v<int> is TRUE
     *   3. Only the FIRST branch is compiled
     *   4. The other branches DON'T EXIST in the binary
     */
}

void TheDiscardedBranch() {
    std::cout << "\n=== The Power of Discarded Branches ===" << std::endl;

    /*
        THE GAME-CHANGING FEATURE:
        - The branch that ISN'T selected is NOT COMPILED.
        - This means it can contain code that WOULDN'T COMPILE otherwise!
    */

    // WITHOUT if constexpr, this would be a compile error:
    auto safe_get_value = [](auto& container, size_t index) {
        if constexpr(std::is_same_v<decltype(container), std::vector<int>&>) {
            // This branch only compiles for std::vector<int>
            return container[index];  // Vector: no bounds checking by default
        }
        else {
            // This branch only compiles for other types
            // For example, for std::map, we could do a find:
            return container[index];  // Different semantics!
        }
        };

    // The magic: each instantiation only sees ITS branch
    std::vector<int> vec = {1, 2, 3};
    std::map<int, std::string> mapping = {{0, "zero"},{1, "one"}};

    std::cout << "  Vector[1]: " << safe_get_value(vec, 1) << std::endl;

    // WITHOUT if constexpr, the following WOULDN'T COMPILE:
    auto demonstrate = [](auto value) {
        if constexpr(std::is_pointer_v<decltype(value)>) {
            // Only compiles if value is a pointer
            // We can safely dereference it!
            std::cout << "  Pointer, points to: " << *value << std::endl;
        }
        else {
            // Only compiles if value is NOT a pointer
            // We CANNOT dereference it, so we just print it
            std::cout << "  Not pointer, value: " << value << std::endl;
        }
        };

    int x = 42;
    demonstrate(&x);  // Calls pointer branch
    demonstrate(x);   // Calls non-pointer branch
    demonstrate(3.14);// Calls non-pointer branch

    /*
        WITHOUT if constexpr, BOTH branches would be compiled, and *value would 
        fail for non-pointers → COMPILE ERROR!
    */
}

void TemplateCodeSimplification() {
    std::cout << "\n=== Simplifying Template Code ===" << std::endl;

    // BEFORE: Required specialization overload
    template <typename T>
    auto old_process(T value) {
        // Would need separate overloads for different types
        return value;
    }

    // AFTER: One function with if constexpr
    auto new_process = [](auto value) {
        using T = decltype(value);

        if constexpr(std::is_integral_v<T>) {
            // Integer-specific optimization
            return value * 2;
        }
        else if constexpr(std::is_floating_point_v<T>) {
            // Float-specific: apply rounding
            return std::round(value * 2);
        }
        else if constexpr(std::is_same_v<T, std::string>) {
            // String-specific: duplicate it
            return value + value;
        }
        else {
            // Default: just return as-is
            return value;
        }
        };

    // All work with the SAME function:
    std::cout << "  int:    " << new_process(21) << std::endl;         // 42
    std::cout << "  double: " << new_process(2.5) << std::endl;       // 5 (rounded)
    std::cout << "  string: " << new_process(std::string("ab")) << std::endl;  // "abab"
    std::cout << "  char:   " << new_process('x') << std::endl;       // just 'x'

    std::cout << R"(
    THE IMPACT:
    ┌─────────────────────────────────────────────────────┐
    │ BEFORE if constexpr:                                │
    │   • Separate template specializations               │
    │   • Enable-if tricks                                │
    │   • Tag dispatch patterns                           │
    │   • Hard to read, harder to maintain                │
    │                                                     │
    │ WITH if constexpr:                                  │
    │   • One function handles all cases                  │
    │   • Readable if/else chains                         │
    │   • Only relevant branch compiled                   │
    │   • Can do type-specific things in each branch      │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void RecursiveIfConstexpr() {
    std::cout << "\n=== if constexpr with Recursion ===" << std::endl;

    // COMPILE-TIME RECURSION with if constexpr:
    // Compile-time factorial using if constexpr
    constexpr int factorial(int n) {
        if constexpr(n >= 0) {
            // Runtime version (this is just for demonstration)
            return (n <= 1) ? 1 : n * factorial(n - 1);
        }
        return -1;  // Error case
    }

    static_assert(factorial(5) == 120);
    std::cout << "  5! = " << factorial(5) << std::endl;

    // A more interesting example: compile-time type traversal
    auto print_tuple = [](auto&& tuple) {
        // This would print all elements of a tuple
        // (simplified demonstration)
        std::cout << "  (tuple printing would go here)" << std::endl;
        };

    std::tuple<int, std::string, double> data = {42, "hello", 3.14};
    print_tuple(data);
}