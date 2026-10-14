#include <iostream>
#include <string>
#include "basic_header.h"


// Basic macros
#define PI 3.14159
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
void BasicMacros(){
    std::cout << "PI: " << PI << std::endl;
    std::cout << "MAX(10, 20): " << MAX(10, 20) << std::endl;
    std::cout << "SQUARE(5): " << SQUARE(5) << std::endl;
    // Macro pitfalls
    int i = 5;
    // std::cout << SQUARE(i++) << std::endl;  // BAD: expands to ((i++) * (i++))
    // Undefined behavior: i incremented twice
    std::cout << "SQUARE(i+1): " << SQUARE(i + 1) << std::endl;  // ((i+1) * (i+1)) = 36
}



// Conditional compilation
void ConditionalCompilation(){
#ifdef DEBUG
    std::cout << "Debug mode is enabled" << std::endl;
#else
    std::cout << "Debug mode is disabled" << std::endl;
#endif

#if defined(_WIN32)
    std::cout << "Running on Windows" << std::endl;
#elif defined(__linux__)
    std::cout << "Running on Linux" << std::endl;
#elif defined(__APPLE__)
    std::cout << "Running on macOS" << std::endl;
#else
    std::cout << "Unknown platform" << std::endl;
#endif

    // C++23: #elifdef and #elifndef
#if defined(DEBUG)
    std::cout << "Debug enabled" << std::endl;
#elifdef VERBOSE
    std::cout << "Verbose mode enabled" << std::endl;
#endif
}



// Predefined macros
void PredefinedMacros(){
    std::cout << "__LINE__: " << __LINE__ << std::endl;
    std::cout << "__FILE__: " << __FILE__ << std::endl;
    std::cout << "__DATE__: " << __DATE__ << std::endl;
    std::cout << "__TIME__: " << __TIME__ << std::endl;

#ifdef __cplusplus
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
    // 201703L = C++17, 202002L = C++20, 202302L = C++23
#endif

// Compiler-specific macros
#ifdef _MSC_VER
    std::cout << "MSVC version: " << _MSC_VER << std::endl;
#endif
#ifdef __GNUC__
    std::cout << "GCC version: " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#endif
#ifdef __clang__
    std::cout << "Clang version: " << __clang_major__ << std::endl;
#endif
}



// Token pasting and stringification
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define PASTE_AND_STRINGIFY(a, b) STRINGIFY(CONCAT(a, b))
void TokenManipulation(){
    std::cout << "STRINGIFY(Hello): " << STRINGIFY(Hello) << std::endl;

    int CONCAT(var, 1) = 42;  // Creates variable named var1
    std::cout << "var1 = " << var1 << std::endl;

    std::cout << "PASTE_AND_STRINGIFY(foo, bar): "
        << PASTE_AND_STRINGIFY(foo, bar) << std::endl;
}
// Include guards and #pragma once
// This file itself demonstrates the concept
// #pragma once (at the top of this file)
// OR
// #ifndef BASIC_HEADER_H
// #define BASIC_HEADER_H
// ... content ...
// #endif



// Variadic macros (C++11)
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)
#define COUNT_ARGS_IMPL(a, b, c, d, e, count, ...) count
#define PRINT_ALL(...) print_all(__VA_ARGS__)
void print_all(){
    std::cout << std::endl;
}
template <typename T, typename... Args>
void print_all(T first, Args... args){
    std::cout << first;
    if(sizeof...(args) > 0) { 
        std::cout << ", "; 
    }
    print_all(args...);
}
void VariadicMacros(){
    std::cout << "COUNT_ARGS(a, b, c): " << COUNT_ARGS(a, b, c) << std::endl;
    std::cout << "COUNT_ARGS(x): " << COUNT_ARGS(x) << std::endl;

    PRINT_ALL(1, 2, 3);
    PRINT_ALL("hello", "world", 42, 3.14);
}



// Feature testing macros 
void FeatureTestingMacros(){
#ifdef __cpp_constexpr
    std::cout << "__cpp_constexpr: " << __cpp_constexpr << std::endl;
#endif

#ifdef __cpp_if_constexpr
    std::cout << "__cpp_if_constexpr: " << __cpp_if_constexpr << std::endl;
#endif

#ifdef __cpp_concepts
    std::cout << "__cpp_concepts: " << __cpp_concepts << std::endl;
#endif

#ifdef __cpp_modules
    std::cout << "__cpp_modules: " << __cpp_modules << std::endl;
#endif

#ifdef __cpp_coroutines
    std::cout << "__cpp_coroutines: " << __cpp_coroutines << std::endl;
#endif

#ifdef __cpp_lib_optional
    std::cout << "__cpp_lib_optional: " << __cpp_lib_optional << std::endl;
#endif
}



// C++23: #warning directive
void WarningDirective(){
    // #warning "This code needs review"
    // Uncomment to see a compile-time warning (not error)

#ifdef __has_warning
#if __has_warning("-Wunused-variable")
    std::cout << "Compiler supports -Wunused-variable warning" << std::endl;
#endif
#endif
}



// Compile-time assertions
void CompileTimeAssertions(){
    // static_assert: compile-time check
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    static_assert(sizeof(void*) >= 4, "Pointer must be at least 4 bytes");

    // With C++17, message is optional
    static_assert(sizeof(double) == 8);

    std::cout << "All static_asserts passed!" << std::endl;
}



// Line continuation
void LineContinuation(){
    // Multi-line macro using backslash continuation
#define LONG_MACRO(x) \
        do { \
            std::cout << "Processing: " << x << std::endl; \
        } while(0)

    LONG_MACRO("test");

    // Note: C++23 removes the need for trailing whitespace before backslash
}