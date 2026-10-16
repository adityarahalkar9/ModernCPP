#include <iostream>
#include <string>
#include "structs_header.h"

// Basic struct definition
struct Point {
    int x;
    int y;
};

struct Person {
    std::string name;
    int age;
    double height;
};

void StructDefinition() {
    std::cout << "=== Basic Struct Definition ===" << std::endl;

    // Create struct instances
    Point p1;              // Uninitialized members (garbage values!)
    Point p2{};            // Zero-initialized members
    Point p3{10, 20};      // Initialized with values

    std::cout << "p3.x = " << p3.x << ", p3.y = " << p3.y << std::endl;

    // Access members
    std::cout << "Accessing members: " << p3.x << " " << p3.y << std::endl;

    // Modify members
    p3.x = 100;
    p3.y = 200;
    std::cout << "After modification: " << p3.x << " " << p3.y << std::endl;
}

void StructWithDifferentTypes() {
    std::cout << "\n=== Struct with Various Types ===" << std::endl;

    Person alice{"Alice", 30, 1.75};

    std::cout << "Name:   " << alice.name << std::endl;
    std::cout << "Age:    " << alice.age << std::endl;
    std::cout << "Height: " << alice.height << "m" << std::endl;

    std::cout << "\nsizeof(Point): " << sizeof(Point) << " bytes" << std::endl;
    std::cout << "sizeof(Person): " << sizeof(Person) << " bytes" << std::endl;
}

// Struct with default member initializers (C++11)
struct Defaults {
    int a = 10;
    double b = 3.14;
    std::string c = "default";
};

void DefaultMemberInitializers() {
    std::cout << "\n=== Default Member Initializers ===" << std::endl;

    Defaults d1;   // Uses defaults: a=10, b=3.14, c="default"
    Defaults d2{}; // Also uses defaults (not zero!)
    Defaults d3{20, 6.28, "custom"};

    std::cout << "d1: a=" << d1.a << ", b=" << d1.b << ", c=" << d1.c << std::endl;
    std::cout << "d2: a=" << d2.a << ", b=" << d2.b << ", c=" << d2.c << std::endl;
    std::cout << "d3: a=" << d3.a << ", b=" << d3.b << ", c=" << d3.c << std::endl;

    std::cout << "\nNote: {} uses default initializers, NOT zero-init" << std::endl;
}

// Copying structs
struct Copyable {
    int value;
    std::string name;
};

void StructCopying() {
    std::cout << "\n=== Struct Copying ===" << std::endl;

    Copyable original{42, "original"};
    Copyable copy = original;      // Copy constructor
    Copyable copy2{original};      // Copy initialization

    std::cout << "original: " << original.value << " " << original.name << std::endl;
    std::cout << "copy:     " << copy.value << " " << copy.name << std::endl;

    // Modifying copy doesn't affect original
    copy.value = 100;
    copy.name = "modified";
    std::cout << "After modification:" << std::endl;
    std::cout << "original: " << original.value << " " << original.name << std::endl;
    std::cout << "copy:     " << copy.value << " " << copy.name << std::endl;

    // Assignment
    Copyable assigned;
    assigned = original;  // Copy assignment
    std::cout << "assigned: " << assigned.value << " " << assigned.name << std::endl;
}

// Structs as value types
void StructsAsValueTypes() {
    std::cout << "\n=== Structs are Value Types ===" << std::endl;

    Point a{1, 2};
    Point b{1, 2};

    std::cout << "a and b are separate objects:" << std::endl;
    std::cout << "&a = " << &a << std::endl;
    std::cout << "&b = " << &b << std::endl;

    // Copy semantics (unlike classes with reference semantics)
    Point c = a;
    c.x = 999;
    std::cout << "After modifying copy: a.x = " << a.x << " (unchanged)" << std::endl;

    std::cout << "\nStructs behave like built-in types (value semantics)" << std::endl;
}

// Arrays of structs
void StructArrays() {
    std::cout << "\n=== Arrays of Structs ===" << std::endl;

    Point points[] = {
        {1, 2},
        {3, 4},
        {5, 6}
    };

    std::cout << "Array of points:" << std::endl;
    for(const auto& p : points) {
        std::cout << "  (" << p.x << ", " << p.y << ")" << std::endl;
    }

    // Vector of structs
    std::vector<Point> more_points = {
        {10, 20},
        {30, 40}
    };

    std::cout << "\nVector of points:" << std::endl;
    for(const auto& p : more_points) {
        std::cout << "  (" << p.x << ", " << p.y << ")" << std::endl;
    }
}

// Struct pointer access
void StructPointerAccess() {
    std::cout << "\n=== Pointer to Struct ===" << std::endl;

    Point p{100, 200};
    Point* ptr = &p;

    // Arrow operator for pointer access
    std::cout << "ptr->x = " << ptr->x << std::endl;
    std::cout << "(*ptr).y = " << (*ptr).y << std::endl;

    // Modify through pointer
    ptr->x = 999;
    std::cout << "After modification: p.x = " << p.x << std::endl;
}