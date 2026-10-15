import std;
#include "basic_header.h"
/*
    #include <iostream>
    #include <map>
    #include <set>
    #include <tuple>
    #include <string>
    #include <array>
*/



// Structured bindings with std::pair
void PairBindings(){
    auto getPair = []() -> std::pair<int, std::string>{
            return {42, "answer"};
        };
    auto [num, str] = getPair();
    std::cout << "num: " << num << ", str: " << str << std::endl;

    // With references
    auto p = std::make_pair(10, 20);
    auto& [a, b] = p;
    a = 100;     // Modifies p.first
    std::cout << "After modification: p.first = " << p.first << std::endl;
}



// Structured bindings with std::tuple
void TupleBindings(){
    auto getTuple = [](){
            return std::make_tuple(1, 2.5, 'c', "string");
        };
    auto [i, d, c, s] = getTuple();
    std::cout << "int: " << i << ", double: " << d
        << ", char: " << c << ", string: " << s << std::endl;

    // Nested tuples
    auto nested = std::make_tuple(
        std::make_pair(1, 2),
        std::make_pair(3, 4)
    );

    auto [pair1, pair2] = nested;
    auto [x1, y1] = pair1;
    auto [x2, y2] = pair2;

    std::cout << "Pair1: (" << x1 << ", " << y1 << ")" << std::endl;
    std::cout << "Pair2: (" << x2 << ", " << y2 << ")" << std::endl;
}



// Structured bindings with structs
struct Point{
    int x{};
    int y{};
    int z{};
};
struct Person{
    std::string name{};
    int age{};
    std::string city{};
};
void StructBindings(){
    Point p{10, 20, 30};
    auto [x, y, z] = p;
    std::cout << "Point: (" << x << ", " << y << ", " << z << ")" << std::endl;

    Person person{"Alice", 30, "New York"};
    auto [name, age, city] = person;
    std::cout << "Person: " << name << ", " << age << ", " << city << std::endl;

    // With const reference (doesn't copy)
    const Point& pRef = p;
    auto& [xr, yr, zr] = pRef;
    // xr = 100;  // Error: const reference

    std::cout << "Through reference: (" << xr << ", " << yr << ", " << zr << ")" << std::endl;
}



// Structured bindings with arrays
void ArrayBindings(){
    // Fixed-size array
    int arr[] = {1, 2, 3, 4, 5};
    auto [a, b, c, d, e] = arr;
    std::cout << "Array: " << a << ", " << b << ", " << c << ", " << d << ", " << e << std::endl;

    // std::array
    std::array<int, 3> stdArr = {10, 20, 30};
    auto [x, y, z] = stdArr;
    std::cout << "std::array: " << x << ", " << y << ", " << z << std::endl;
}



// Structured bindings with std::map
void MapBindings(){
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    std::cout << "Iterating map with structured bindings:" << std::endl;
    for(const auto& [name, age] : ages){
        std::cout << name << " is " << age << " years old" << std::endl;
    }

    // Modifying values through bindings
    for(auto& [name, age] : ages){
        age += 1;  // Increment everyone's age
    }

    std::cout << "After increment:" << std::endl;
    for(const auto& [name, age] : ages){
        std::cout << name << " is " << age << " years old" << std::endl;
    }
}



// Structured bindings in range-based for loops
void RangeForBindings(){
    std::vector<std::pair<int, std::string>> vec = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };
    for(const auto& [num, name] : vec){
        std::cout << num << " -> " << name << std::endl;
    }

    // With set
    std::set<std::pair<int, int>> points = {{1, 2}, {3, 4}, {5, 6}};
    for(const auto& [x, y] : points){
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
}



// Structured bindings with custom types
struct Vector3D{
    double x{}, y{}, z{};

    // Make it destructurable
    template <std::size_t N>
    constexpr decltype(auto) get() const noexcept{
        if constexpr(N == 0) return x;
        else if constexpr(N == 1) return y;
        else if constexpr(N == 2) return z;
    }
};



// Enable tuple protocol for structured bindings
namespace std{
    template <>
    struct tuple_size<Vector3D> : std::integral_constant<std::size_t, 3> {};

    template <>
    struct tuple_element<0, Vector3D>{ 
        using type = double; 
    };

    template <>
    struct tuple_element<1, Vector3D>{ 
        using type = double; 
    };

    template <>
    struct tuple_element<2, Vector3D>{
        using type = double; 
    };
}
void CustomTypeBindings(){
    Vector3D v{1.0, 2.0, 3.0};
    auto [x, y, z] = v;
    std::cout << "Vector3D: (" << x << ", " << y << ", " << z << ")" << std::endl;
}



// Structured bindings with if/switch
void IfWithBindings(){
    auto getData = []() -> std::optional<std::pair<int, std::string>>{
            return std::make_pair(42, "answer");
        };

    if(auto [value, name] = getData().value_or(std::make_pair(0, "")); value != 0){
        std::cout << "Got: " << name << " = " << value << std::endl;
    }
    else{
        std::cout << "No data" << std::endl;
    }
}



// Structured bindings can be captured in lambdas
void LambdaCaptureBindings(){
    auto [x, y] = std::make_pair(10, 20);
    auto lambda = [&x, &y](){
            std::cout << "Captured: x = " << x << ", y = " << y << std::endl;
        };
    lambda();

    // Structured bindings in lambda init-capture
    auto lambda2 = [x_copy = x, y_copy = y](){
            std::cout << "Copied: x = " << x_copy << ", y = " << y_copy << std::endl;
        };
    lambda2();
}



// Structured bindings with aliases in init-statements
void InitStatementBindings(){
    std::vector<std::pair<int, std::string>> data = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };

    // using declarations in init-statements
    // for (using T = std::pair<int, std::string>; const auto& [num, name] : data) {
    //     std::cout << num << " -> " << name << std::endl;
    // }

    // Note: Full C++23 support required
    for(const auto& [num, name] : data) {
        std::cout << num << " -> " << name << std::endl;
    }
}