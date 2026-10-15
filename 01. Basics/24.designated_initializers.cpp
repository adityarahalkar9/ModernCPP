import std;
#include "basic_header.h"
/*
    #include <iostream>
    #include <string>
    #include <vector>
    #include <array>
*/


// C++20 Designated Initializers
struct Point{
    int x{0};
    int y{0};
};
struct Rectangle{
    Point top_left{};
    Point bottom_right{};
    std::string label{"unnamed"};
    bool filled{false};
};
struct Config{
    std::string name = "default";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    std::vector<int> levels = {1, 2, 3};
    double scale = 1.0;
    int priority = 5;
};
void BasicDesignatedInit(){
    // All fields in order
    Point p1{.x = 10, .y = 20};
    std::cout << "p1: (" << p1.x << ", " << p1.y << ")" << std::endl;

    // Skip some fields (use default values)
    Point p2{.y = 30};      // x uses default (0)
    std::cout << "p2: (" << p2.x << ", " << p2.y << ")" << std::endl;

    // Nested designated initializers
    Rectangle r1{
        .top_left = {.x = 0, .y = 0},
        .bottom_right = {.x = 100, .y = 100},
        .label = "screen",
        .filled = true
    };

    std::cout << "r1: " << r1.label << " from ("
        << r1.top_left.x << "," << r1.top_left.y << ") to ("
        << r1.bottom_right.x << "," << r1.bottom_right.y << ")" << std::endl;

    // Partial initialization with nesting
    Rectangle r2{
        .top_left = {.x = 5},
        .bottom_right = {.y = 50},
        .label = "partial"
    };

    std::cout << "r2: " << r2.label << ", TL=("
        << r2.top_left.x << "," << r2.top_left.y << ") BR=("
        << r2.bottom_right.x << "," << r2.bottom_right.y << ")" << std::endl;
}



void DesignatedInitRules(){
    // MUST be in declaration order
    // Config bad{
    //     .height = 100,     // Error: must come after .width
    //     .width = 50
    // };

    // Cannot mix designated and positional
    // Config bad2{
    //     .name = "test",
    //     800,              // Error: cannot mix
    //     .fullscreen = true
    // };

    // Cannot repeat designators
    // Config bad3{
    //     .name = "test",
    //     .name = "again"   // Error: duplicate
    // };

    // Valid: skipping is allowed
    Config c1{
        .name = "app",
        .width = 1920,
        .height = 1080
        // fullscreen, levels, scale, priority use defaults
    };

    std::cout << "c1: " << c1.name << " " << c1.width << "x" << c1.height << std::endl;
    std::cout << "Default fullscreen: " << c1.fullscreen << std::endl;
    std::cout << "Default scale: " << c1.scale << std::endl;
}



// Designated initializers with arrays 
void ArrayDesignatedInit(){
    // Note: C++ doesn't support array designated initializers like C
    // int arr[5] = {[2] = 10, [4] = 20};  // C only, not C++

    // But you can use them with structs containing arrays
    struct Buffer{
        int size{};
        std::array<int, 5> data{};
    };
    Buffer buf{
        .size = 5,
        .data = {1, 2, 3, 4, 5}  // Regular initialization for array member
    };

    std::cout << "Buffer size: " << buf.size << ", data: ";
    for(int i : buf.data) std::cout << i << " ";
    std::cout << std::endl;
}



// Comparison: before and after C++20
void BeforeAndAfterComparison(){
    // Before C++20: positional only
    Config old_way = {"myapp", 1024, 768, true, {5, 10, 15}, 2.5, 10};
    // Hard to tell what each value means

    // Designated initializers
    Config new_way{
        .name = "myapp",
        .width = 1024,
        .height = 768,
        .fullscreen = true,
        .levels = {5, 10, 15},
        .scale = 2.5,
        .priority = 10
    };
    // Much more readable and self-documenting

    std::cout << "Old way: " << old_way.name << " " << old_way.width << "x" << old_way.height << std::endl;
    std::cout << "New way: " << new_way.name << " " << new_way.width << "x" << new_way.height << std::endl;
}



// Practical examples
struct NetworkConfig{
    std::string host{"localhost"};
    int port{8080};
    bool use_ssl{false};
    int timeout_ms{5000};
    int max_retries{3};
    std::string user_agent{"MyApp/1.0"};
};
void PracticalExamples(){
    // Different configurations clearly expressed
    NetworkConfig dev{
        .host = "localhost",
        .port = 3000,
        .use_ssl = false
    };
    NetworkConfig prod{
        .host = "api.example.com",
        .port = 443,
        .use_ssl = true,
        .timeout_ms = 10000,
        .max_retries = 5
    };

    std::cout << "Dev: " << dev.host << ":" << dev.port << " (SSL: " << dev.use_ssl << ")" << std::endl;
    std::cout << "Prod: " << prod.host << ":" << prod.port << " (SSL: " << prod.use_ssl << ")" << std::endl;
}



// Designated initializers with inheritance
struct Base{
    int base_val{0};
};
struct Derived : Base{
    int derived_val{0};
};
void DesignatedInitWithInheritance() {
    // Nested braces initialize the Base subobject first, followed by Derived's members
    Derived d{10, 20};
    std::cout << "Base: " << d.base_val << ", Derived: " << d.derived_val << std::endl;
}