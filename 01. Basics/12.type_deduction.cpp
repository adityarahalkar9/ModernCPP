#include <iostream>
#include <typeinfo>
#include "basic_header.h"

// C++11 auto: deduces type from initializer
void AutoBasic(){
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = "hello";      // const char*
    auto b = true;         // bool
    auto c = 'A';          // char

    std::cout << "auto i: " << typeid(i).name() << " = " << i << std::endl;
    std::cout << "auto d: " << typeid(d).name() << " = " << d << std::endl;
    std::cout << "auto s: " << typeid(s).name() << " = " << s << std::endl;
    std::cout << "auto b: " << typeid(b).name() << " = " << b << std::endl;
    std::cout << "auto c: " << typeid(c).name() << " = " << c << std::endl;
}

// auto with references and const
void AutoWithModifiers() {
    int x = 10;
    const int cx = 20;
    int& rx = x;

    auto a = x;         // int (copy)
    auto b = cx;        // int (const is dropped)
    auto c = rx;        // int (reference is dropped)
    auto& d = x;        // int& (reference preserved)
    const auto& e = x;  // const int&

    auto f = &x;        // int*
    auto g = &cx;       // const int*

    std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
    d = 100;
    std::cout << "After d=100, x = " << x << " (reference works)" << std::endl;
}

// auto in function return types
auto AddAuto(int a, int b) -> int {
    return a + b;
}
// C++14: auto return type deduction
auto MultiplyAuto(int a, int b) {
    return a * b;  // return type deduced as int
}
void AutoReturnTypes() {
    std::cout << "Add(3, 4) = " << AddAuto(3, 4) << std::endl;
    std::cout << "Multiply(5, 6) = " << MultiplyAuto(5, 6) << std::endl;
}

// decltype: inspects the declared type of an entity
void DecltypeBasic() {
    int i = 42;
    double d = 3.14;
    int& ri = i;
    const int ci = 100;

    decltype(i) a = 10;       // int
    decltype(d) b = 2.71;     // double
    decltype(ri) c = i;       // int&
    decltype(ci) e = 50;      // const int

    c = 200;
    std::cout << "After c=200, i = " << i << std::endl;
    std::cout << "a: " << a << ", b: " << b << ", e: " << e << std::endl;
}

// decltype with expressions
void DecltypeExpressions() {
    int x = 5;
    int y = 10;
    decltype(x + y) sum = x + y;        // int
    decltype(x = 10) assign = x;        // int& (assignment returns reference)
    decltype(x * 1.0) prod = x * 1.0;   // double

    std::cout << "sum: " << sum << std::endl;
    std::cout << "prod: " << prod << std::endl;
}

// decltype(auto): C++14, preserves references and cv-qualifiers
decltype(auto) GetRef(int& x) {
    return x;   // returns int&
}
decltype(auto) GetValue(int x) {
    return x;  // returns int (copy)
}
void DecltypeAuto() {
    int a = 42;
    int& ref = GetRef(a);
    ref = 100;
    std::cout << "After modification via reference: a = " << a << std::endl;

    int b = GetValue(a);
    b = 200;  // doesn't affect a
    std::cout << "a = " << a << ", b = " << b << std::endl;
}

// C++20: abbreviated function templates using auto
void AbbreviatedFunctionTemplates() {
    auto sum = [](auto a, auto b) {
            return a + b;
        };
    std::cout << "sum(3, 4) = " << sum(3, 4) << std::endl;
    std::cout << "sum(2.5, 3.7) = " << sum(2.5, 3.7) << std::endl;
    std::cout << "sum(std::string(\"Hello, \"), std::string(\"World!\")) = "
              << sum(std::string("Hello, "), std::string("World!")) << std::endl;
}

// auto with structured bindings (C++17)
void AutoWithStructuredBindings(){
    auto getPair = []() -> std::pair<int, std::string> {
        return {42, "answer"};
        };
    auto [num, str] = getPair();
    std::cout << "num = " << num << ", str = " << str << std::endl;

    // With arrays
    int arr[] = {1, 2, 3};
    auto [a, b, c] = arr;
    std::cout << "array: " << a << ", " << b << ", " << c << std::endl;
}

// C++23: auto(x) and auto{x} - decay copy in the language
/*
    void DecayCopy() {
    int arr[] = {1, 2, 3, 4, 5};

    // C++23: explicit decay copy
    auto arr_copy = auto(arr);      // int* (array decays to pointer)
    std::cout << "First element via decay copy: " << *arr_copy << std::endl;

    int x = 42;
    const int& cref = x;
    auto decayed = auto(cref);      // int (decay: const ref -> value)
    decayed = 100;                  // doesn't affect x
    std::cout << "x = " << x << " (unchanged), decayed = " << decayed << std::endl;

    // Function that takes by value, called with decay copy
    auto int_arr = auto{arr};          // int* using brace syntax
    std::cout << "Second element: " << int_arr[1] << std::endl;
}
*/

