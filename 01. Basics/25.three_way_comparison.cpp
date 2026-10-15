// import std;
#include "basic_header.h"
#include <iostream>
#include <compare>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


// Basic spaceship operator
struct Version{
    int major{};
    int minor{};
    int patch{};

    // C++20: defaulted three-way comparison
    auto operator<=>(const Version&) const = default;
};
void BasicSpaceship(){
    Version v1{1, 0, 0};
    Version v2{1, 1, 0};
    Version v3{1, 0, 0};

    // All comparison operators are generated
    std::cout << "v1 == v3: " << std::boolalpha << (v1 == v3) << std::endl;
    std::cout << "v1 != v2: " << (v1 != v2) << std::endl;
    std::cout << "v1 < v2: " << (v1 < v2) << std::endl;
    std::cout << "v1 <= v3: " << (v1 <= v3) << std::endl;
    std::cout << "v2 > v1: " << (v2 > v1) << std::endl;
    std::cout << "v2 >= v3: " << (v2 >= v3) << std::endl;

    // Using the spaceship operator directly
    auto result = v1 <=> v2;
    std::cout << "v1 <=> v2: " << (result < 0 ? "less" :
                                   result > 0 ? "greater" : "equal") << std::endl;
}



// Different comparison categories
void ComparisonCategories(){
    // std::strong_ordering: ==, !=, <, <=, >, >=
    int a = 5, b = 10;
    auto strong = a <=> b;
    static_assert(std::is_same_v<decltype(strong), std::strong_ordering>);

    std::cout << "int comparison: " << (strong == std::strong_ordering::less ? "less" :
                                        strong == std::strong_ordering::equal ? "equal" : "greater") 
                                    << std::endl;

    // std::partial_ordering: for floating point (can be NaN)
    double x{3.14}, y{2.71};
    auto partial = x <=> y;
    static_assert(std::is_same_v<decltype(partial), std::partial_ordering>);

    std::cout << "double comparison: "
        << (partial == std::partial_ordering::greater ? "greater" :
            partial == std::partial_ordering::equivalent ? "equivalent" :
            partial == std::partial_ordering::less ? "less" : "unordered") << std::endl;

    // NaN comparison
    double nan = std::numeric_limits<double>::quiet_NaN();
    auto nan_result = nan <=> 1.0;
    std::cout << "NaN <=> 1.0: " << (nan_result == std::partial_ordering::unordered ? "unordered" : "ordered") 
              << std::endl;
}



/*
    // Custom three-way comparison
    struct CaseInsensitiveString{
        std::string value{};
        std::strong_ordering operator<=>(const CaseInsensitiveString& other) const{
            std::string a = value, b = other.value;
            std::transform(a.begin(), a.end(), a.begin(), ::tolower);
            std::transform(b.begin(), b.end(), b.begin(), ::tolower);
            if(a < b){
                return std::strong_ordering::less;
            }
            if(a > b){
                return std::strong_ordering::greater;
            }
            return std::strong_ordering::equal;
        }

        bool operator==(const CaseInsensitiveString& other) const {
            return (*this <=> other) == std::strong_ordering::equal;
        }
    };
    void CustomComparison(){
        CaseInsensitiveString s1{"Hello"};
        CaseInsensitiveString s2{"HELLO"};
        CaseInsensitiveString s3{"World"};

        std::cout << "s1 == s2: " << std::boolalpha << (s1 == s2) << std::endl;
        std::cout << "s1 < s3: " << (s1 < s3) << std::endl;
    }
*/



// Using std::compare_three_way
void CompareThreeWay(){
    auto compare = std::compare_three_way{};
    auto result = compare(10, 20);
    std::cout << "10 <=> 20: " << (result < 0 ? "less" : result > 0 ? "greater" : "equal") << std::endl;
}



// Comparison with containers
struct Container{
    std::vector<int> data{};
    auto operator<=>(const Container&) const = default;
};
void ContainerComparison(){
    Container c1{{1, 2, 3}};
    Container c2{{1, 2, 3}};
    Container c3{{1, 2, 4}};

    std::cout << "c1 == c2: " << std::boolalpha << (c1 == c2) << std::endl;
    std::cout << "c1 < c3: " << (c1 < c3) << std::endl;

    // Works with std::algorithms
    std::vector<Container> containers = {c3, c1, c2};
    std::sort(containers.begin(), containers.end());

    std::cout << "Sorted: ";
    for(const auto& c : containers){
        std::cout << "[";
        for(int i : c.data) std::cout << i << " ";
        std::cout << "] ";
    }
    std::cout << std::endl;
}



// operator<=> with operator== (both needed for complete comparison)
struct Point{
    int x{}, y{};

    // <=> provides <, <=, >, >=
    auto operator<=>(const Point&) const = default;

    // == must be explicitly defaulted or defined
    bool operator==(const Point&) const = default;
};
void EqualityAndOrdering(){
    Point p1{1, 2};
    Point p2{1, 2};
    Point p3{1, 3};

    std::cout << "p1 == p2: " << std::boolalpha << (p1 == p2) << std::endl;
    std::cout << "p1 < p3: " << (p1 < p3) << std::endl;

    // Can use in containers
    std::set<Point> points = {p3, p1, p2, {0, 0}};
    std::cout << "Set size: " << points.size() << std::endl;
}



// C++20: operator==() = default without <=>
struct OnlyEquality{
    int value;
    bool operator==(const OnlyEquality&) const = default;
    // Note: This only provides ==, not <, >, etc.
};
void EqualityOnly(){
    OnlyEquality a{42};
    OnlyEquality b{42};
    OnlyEquality c{24};

    std::cout << "a == b: " << std::boolalpha << (a == b) << std::endl;
    std::cout << "a == c: " << (a == c) << std::endl;
    // std::cout << (a < b);  // Error: < not defined
}