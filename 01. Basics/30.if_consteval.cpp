#include <iostream>
#include <string>
#include <array>
#include "basic_header.h"

// C++23: if consteval - detect compile-time evaluation context
constexpr int CompileTimeOnly() {
    if consteval {
        // This branch is taken during compile-time evaluation
        return 42;
    }
    else {
        // This branch is taken during runtime
        return 0;
    }
}

void IfConstevalBasic() {
    // Compile-time call
    constexpr int compile_result = CompileTimeOnly();
    static_assert(compile_result == 42);

    // Runtime call
    int runtime_result = CompileTimeOnly();
    std::cout << "Compile-time result: " << compile_result << std::endl;
    std::cout << "Runtime result: " << runtime_result << std::endl;
}

// Practical use: optimization based on evaluation context
constexpr size_t StringLength(const char* str) {
    if consteval {
        // Compile-time: can use recursion
        return *str == '\0' ? 0 : 1 + StringLength(str + 1);
    }
    else {
        // Runtime: use more efficient iterative approach
        size_t len = 0;
        while(str[len] != '\0') ++len;
        return len;
    }
}

void IfConstevalOptimization() {
    constexpr const char* str = "Hello, World!";

    // Compile-time evaluation
    constexpr size_t ct_len = StringLength(str);
    static_assert(ct_len == 13);

    // Runtime evaluation
    size_t rt_len = StringLength(str);

    std::cout << "Compile-time length: " << ct_len << std::endl;
    std::cout << "Runtime length: " << rt_len << std::endl;
}

// if consteval vs if constexpr
void IfConstevalVsIfConstexpr() {
    // if constexpr: branch based on compile-time condition (type or value)
    // if consteval: branch based on whether we're in constant evaluation

    // Example: if consteval is useful when you need different behavior
    // in compile-time vs runtime contexts of the same function

    auto hybrid_function = [](int x) constexpr {
        if consteval {
            // Strict compile-time logic (can use static_assert, etc.)
            return x * 2;
        }
        else {
            // Can use runtime-only features (I/O, etc.)
            // std::cout << "Runtime call with x = " << x << std::endl;
            return x * 3;
        }
        };

    constexpr int ct_result = hybrid_function(10);  // Compile-time: 20
    int rt_result = hybrid_function(10);             // Runtime: 30

    std::cout << "Compile-time result: " << ct_result << std::endl;
    std::cout << "Runtime result: " << rt_result << std::endl;
}

// if not consteval (C++23)
constexpr int OnlyRuntime() {
    if not consteval {
        // Runtime-only code path
        return 100;
    }
    else {
        // Compile-time path
        return 200;
    }
}

void IfNotConsteval() {
    constexpr int ct = OnlyRuntime();
    static_assert(ct == 200);  // Compile-time returns 200

    int rt = OnlyRuntime();
    std::cout << "Compile-time: " << ct << " (should be 200)" << std::endl;
    std::cout << "Runtime: " << rt << " (should be 100)" << std::endl;
}

// is_constant_evaluated (C++20) vs if consteval (C++23)
// C++20 introduced std::is_constant_evaluated()
// C++23 introduced if consteval as a cleaner syntax

constexpr bool IsConstantEvaluated() {
    // C++20 way
    if(std::is_constant_evaluated()) {
        return true;
    }
    else {
        return false;
    }
}

constexpr bool IfConstevalWay() {
    // C++23 way (cleaner)
    if consteval {
        return true;
    }
    else {
        return false;
    }
}

void ConstevalComparison() {
    constexpr bool ct20 = IsConstantEvaluated();  // true at compile time
    constexpr bool ct23 = IfConstevalWay();        // true at compile time

    static_assert(ct20 == true);
    static_assert(ct23 == true);

    std::cout << "Both approaches work for compile-time detection" << std::endl;
    std::cout << "C++23 if consteval is preferred for new code" << std::endl;
}

// Compile-time data generation
template <size_t N>
constexpr std::array<int, N> GenerateSquares() {
    std::array<int, N> result{};
    for(size_t i = 0; i < N; ++i) {
        if consteval {
            // Compile-time: straightforward generation
            result[i] = static_cast<int>(i * i);
        }
        else {
            // Runtime: could use optimized approach
            result[i] = static_cast<int>(i * i);
        }
    }
    return result;
}

void CompileTimeGeneration() {
    constexpr auto squares = GenerateSquares<10>();

    std::cout << "Generated squares: ";
    for(int sq : squares) {
        std::cout << sq << " ";
    }
    std::cout << std::endl;

    // Verify at compile time
    static_assert(squares[3] == 9);
    static_assert(squares[5] == 25);
}

// Combining with consteval functions
consteval int StrictCompileTime(int x) {
    // consteval: MUST be evaluated at compile time
    if consteval {
        return x * 2;
    }
    // Note: consteval functions are always compile-time,
    // so the else branch is unreachable
    return 0;  // Never reached, but required for compilation
}

void ConstevalWithIfConsteval() {
    constexpr int result = StrictCompileTime(21);
    static_assert(result == 42);

    std::cout << "consteval result: " << result << std::endl;

    // Note: consteval function cannot be called at runtime
    // int rt = StrictCompileTime(21);  // Error: immediate function
}