#include "basic_header.h" 

#include <iostream>
#include <utility>
#include <string>
#include <tuple>
#include <optional>
#include <variant>
#include <any>

#ifdef __cpp_lib_expected
#include <expected> // Legally included at global scope if module not used
#endif

// std::pair
void PairBasics(){
    std::pair<int, std::string> p1(1, "one");
    auto p2 = std::make_pair(2, "two");
    auto p3 = std::pair{3, "three"};

    std::cout << "p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
    std::cout << "p2: (" << p2.first << ", " << p2.second << ")" << std::endl;
    std::cout << "p1 < p2: " << std::boolalpha << (p1 < p2) << std::endl;

    p1 = p2;
    std::cout << "After assignment, p1: (" << p1.first << ", " << p1.second << ")" << std::endl;
}

// std::tuple
void TupleBasics(){
    std::tuple<int, std::string, double> t1(1, "one", 1.0);
    auto t2 = std::make_tuple(2, "two", 2.0);
    auto t3 = std::tuple{3, "three", 3.0};

    std::cout << "t1: (" << std::get<0>(t1) << ", "
        << std::get<1>(t1) << ", "
        << std::get<2>(t1) << ")" << std::endl;

    auto int_val = std::get<int>(t1);
    std::cout << "Integer value: " << int_val << std::endl;
    std::cout << "Tuple size: " << std::tuple_size<decltype(t1)>::value << std::endl;

    std::get<0>(t1) = 100;
    std::cout << "Modified t1[0]: " << std::get<0>(t1) << std::endl;
    std::cout << "t2 == t3: " << std::boolalpha << (t2 == t3) << std::endl;
}

// Structured bindings
void StructuredBindings(){
    auto p = std::make_pair(42, "answer");
    auto [num, str] = p;
    std::cout << "Pair binding: " << num << " -> " << str << std::endl;

    auto t = std::make_tuple(1, 2.5, 'c');
    auto [i, d, c] = t;
    std::cout << "Tuple binding: " << i << ", " << d << ", " << c << std::endl;

    struct Point { int x, y; };
    Point pt{10, 20};
    auto [x, y] = pt;
    std::cout << "Struct binding: (" << x << ", " << y << ")" << std::endl;

    int arr[] = {1, 2, 3, 4, 5};
    auto [a, b, cc, dd, e] = arr;
    std::cout << "Array binding: " << a << b << cc << dd << e << std::endl;

    auto& [xr, yr] = pt;
    xr = 100;
    std::cout << "After modification: pt.x = " << pt.x << std::endl;
}

/*
    // std::optional 
    void OptionalBasics(){
        std::optional<int> opt1;
        std::optional<int> opt2 = 42;
        std::optional<int> opt3 = std::nullopt;
        auto opt4 = std::optional{3.14};

        std::cout << "opt1 has value: " << std::boolalpha << opt1.has_value() << std::endl;
        std::cout << "opt2 has value: " << opt2.has_value() << std::endl;
        std::cout << "opt2 value: " << opt2.value() << std::endl;
        std::cout << "opt2 value_or: " << opt2.value_or(0) << std::endl;
        std::cout << "opt1 value_or: " << opt1.value_or(-1) << std::endl;
        std::cout << "opt2 (*): " << *opt2 << std::endl;

        opt2.reset();
        std::cout << "After reset: " << opt2.has_value() << std::endl;

        auto process = [](int x) -> std::optional<int> {
            if(x > 0) return x * 2;
            return std::nullopt;
            };

        auto result = std::optional{5}.and_then(process);
        std::cout << "and_then result: " << result.value_or(0) << std::endl;

        auto transformed = std::optional{10}.transform([](int x) { return x + 1; });
        std::cout << "transform result: " << transformed.value_or(0) << std::endl;

        auto fallback = std::optional<int>{}.or_else([]() { return std::optional{99}; });
        std::cout << "or_else result: " << fallback.value_or(0) << std::endl;
    }
*/

// std::variant
void VariantBasics(){
    std::variant<int, double, std::string> v;

    v = 42;
    std::cout << "Current type: int, value: " << std::get<int>(v) << std::endl;

    v = 3.14;
    std::cout << "Current type: double, value: " << std::get<double>(v) << std::endl;

    v = "hello";
    std::cout << "Current type: string, value: " << std::get<std::string>(v) << std::endl;
    std::cout << "Active index: " << v.index() << std::endl;
    std::cout << "Holds string: " << std::boolalpha << std::holds_alternative<std::string>(v) << std::endl;

    std::visit([](auto&& arg) {
        std::cout << "Visited value: " << arg << std::endl;
               }, v);

    if(auto* p = std::get_if<int>(&v)) {
        std::cout << "It's an int: " << *p << std::endl;
    }
    else if(auto* p = std::get_if<std::string>(&v)) {
        std::cout << "It's a string: " << *p << std::endl;
    }
}

// std::any
void AnyBasics(){
    std::any a = 42;
    std::cout << "Type: " << a.type().name() << std::endl;
    std::cout << "Value: " << std::any_cast<int>(a) << std::endl;

    a = std::string("hello");
    std::cout << "Type: " << a.type().name() << std::endl;
    std::cout << "Value: " << std::any_cast<std::string>(a) << std::endl;

    a = 3.14;
    std::cout << "Value: " << std::any_cast<double>(a) << std::endl;
    std::cout << "Has value: " << std::boolalpha << a.has_value() << std::endl;

    a.reset();
    std::cout << "After reset: " << a.has_value() << std::endl;

    a = 42;
    try{
        std::any_cast<std::string>(a);
    }
    catch(const std::bad_any_cast& e) {
        std::cout << "Bad cast: " << e.what() << std::endl;
    }
}

// std::expected
void ExpectedBasics(){
#ifdef __cpp_lib_expected
    // expected<T, E> - either a value of T or an error of E
    std::expected<int, std::string> result = 42;
    std::cout << "Has value: " << result.has_value() << std::endl;
    std::cout << "Value: " << result.value() << std::endl;

    auto error = std::unexpected<std::string>("Something went wrong");
    std::expected<int, std::string> err = error;
    std::cout << "Has value: " << err.has_value() << std::endl;
    std::cout << "Error: " << err.error() << std::endl;

    auto transform = [](int x) -> int { return x * 2; };
    auto transformed = result.transform(transform);
    std::cout << "Transformed: " << transformed.value() << std::endl;

    auto process = [](int x) -> std::expected<int, std::string> {
        if(x > 0) return x + 10;
        return std::unexpected("Negative value");
        };

    auto chained = result.and_then(process);
    std::cout << "Chained: " << chained.value() << std::endl;
#else
    std::cout << "std::expected not available (need C++23)" << std::endl;
#endif
}