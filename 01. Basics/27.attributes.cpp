#include <iostream>
#include <string>
#include "basic_header.h"

// [[nodiscard]] - warn if return value is discarded
[[nodiscard]] 
int ImportantFunction(){
    return 42;
}
[[nodiscard]] 
std::string GetString(){
    return "Important value";
}
void NodiscardAttribute(){
    // This will generate a compiler warning:
    // ImportantFunction();  // Warning: ignoring return value

    int result = ImportantFunction();  // OK
    std::cout << "Result: " << result << std::endl;

    // [[nodiscard]] with reason (C++20)
    // [[nodiscard("Check the error code")]] int OpenFile() { ... }
}



// [[maybe_unused]] - suppress unused warnings
void MaybeUnusedAttribute() {
    [[maybe_unused]] int unused_variable = 42;
    [[maybe_unused]] std::string unused_string = "not used";

    // No compiler warnings for these

    // Useful for debug-only variables
    [[maybe_unused]] bool debug_flag =
#ifdef DEBUG
        true
#else
        false
#endif
        ;

    std::cout << "No unused warnings" << std::endl;
}



// [[deprecated]] - mark as deprecated
[[deprecated("Use NewFunction() instead")]]
void OldFunction(){
    std::cout << "Old function (deprecated)" << std::endl;
}
void NewFunction(){
    std::cout << "New function" << std::endl;
}
void DeprecatedAttribute(){
    // OldFunction();  // Compiler warning: deprecated
    NewFunction();  // No warning
}



// [[fallthrough]] - indicate intentional fallthrough in switch
void FallthroughAttribute(){
    int value = 2;
    switch(value){
        case 1:
            std::cout << "Case 1" << std::endl;
            break;
        case 2:
            std::cout << "Case 2" << std::endl;
            [[fallthrough]];  // Intentional fallthrough - no warning
        case 3:
            std::cout << "Case 3 (also executed for case 2)" << std::endl;
            break;
        default:
            std::cout << "Default case" << std::endl;
            break;
    }
}



// [[likely]] and [[unlikely]] (C++20) - optimization hints
int ProcessValue(int value){
    if(value > 0) [[likely]]{
        // This branch is expected to be taken more often
        return value * 2;
    }
    else [[unlikely]]{
        // This branch is expected to be taken less often
        return -value;
    }
}
void LikelyUnlikelyAttributes(){
    std::cout << "ProcessValue(10): " << ProcessValue(10) << std::endl;
    std::cout << "ProcessValue(-5): " << ProcessValue(-5) << std::endl;

    // These attributes help compiler optimization:
    // - [[likely]] on a branch tells the compiler to optimize for that path
    // - [[unlikely]] tells the compiler the path is rarely taken
}



// [[no_unique_address]] (C++20) - allow empty objects to be optimized away
struct Empty {};
struct WithNoUniqueAddress{
    int value{};
    [[no_unique_address]] Empty empty;  // May not take any space
};
struct WithoutAttribute{
    int value{};
    Empty empty{};  // May take space
};
void NoUniqueAddressAttribute(){
    std::cout << "sizeof(Empty): " << sizeof(Empty) << std::endl;
    std::cout << "sizeof(WithNoUniqueAddress): " << sizeof(WithNoUniqueAddress) << std::endl;
    std::cout << "sizeof(WithoutAttribute): " << sizeof(WithoutAttribute) << std::endl;

    // With [[no_unique_address]], the empty member can share
    // storage with other members, potentially reducing struct size
}



// [[noreturn]] - function never returns
[[noreturn]] void ThrowError(){
    throw std::runtime_error("Fatal error");
}
void NoReturnAttribute(){
    try{
        ThrowError();
    }
    catch(const std::exception& e){
        std::cout << "Caught: " << e.what() << std::endl;
    }

    // [[noreturn]] tells the compiler that this function never returns normally
    // Examples: functions that always throw, call exit(), or infinite loop
}



// [[carries_dependency]] - for dependency ordering in optimization
// Rarely used, mainly for expert optimization scenarios



// Standard attributes vs vendor-specific attributes
void AttributeSyntaxes() {
    // Standard C++ attributes: [[...]]
    // C++11: [[noreturn]], [[carries_dependency]]
    // C++14: [[deprecated]]
    // C++17: [[fallthrough]], [[maybe_unused]], [[nodiscard]]
    // C++20: [[likely]], [[unlikely]], [[no_unique_address]]

    // Vendor-specific attributes use different syntaxes:
    // GCC/Clang: __attribute__((...))
    // MSVC: __declspec(...)
    // These are non-portable

    // C++17 allows attributes in more places:
    // - After [[ (no spaces between [[)
    // - In more syntactic positions
    std::cout << "Standard attributes are portable across compilers" << std::endl;
}



// [[assume]] (C++23) - compiler optimization assumptions
void AssumeAttribute(){
    int x = 42;

    // C++23: [[assume]] tells the compiler a condition is true
    // This can enable optimizations
    // [[assume(x > 0)]];

    // Example function that could benefit:
    auto divide = [](int a, int b) -> int {
            // [[assume(b != 0)]];  // C++23: compiler can skip division-by-zero check
            return a / b;
        };

    std::cout << "42 / 6 = " << divide(42, 6) << std::endl;
    // Note: [[assume]] is C++23 and may not be available in all compilers yet
}



// Attributes on lambdas (C++23)
void AttributesOnLambdas(){
    // C++23: Attributes can be applied to lambda expressions
    auto lambda = [] [[nodiscard]] () {
        return 42;
    };

    // lambda();  // Would generate warning: discarding return value
    int result = lambda();  // OK
    std::cout << "Lambda result: " << result << std::endl;
}



/*
    // Combining multiple attributes on a variable
    void MultipleAttributes(){
        // [[maybe_unused]] suppresses warnings if the variable is unreferenced.
        // [[deprecated]] accepts a custom string message (a C++20 feature) 
        // which the compiler prints if this variable is accessed or compiled.
        [[maybe_unused]] [[deprecated("cached_value: For tutorial demonstration purposes only")]]
                static int cached_value = []() {
                return 42;
            }();
        std::cout << "Cached value: " << cached_value << std::endl;
    }
*/