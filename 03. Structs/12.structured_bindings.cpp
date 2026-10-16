import std;
#include "structs_header.h"

// Structured bindings with custom structs
struct Point3D {
    double x, y, z;
};

struct PersonInfo {
    std::string name;
    int age;
    std::string city;
    double salary;
};

void BasicStructuredBindings() {
    std::cout << "=== Basic Structured Bindings ===" << std::endl;

    Point3D p{1.0, 2.0, 3.0};

    // C++17: Structured bindings
    auto [x, y, z] = p;
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;

    // With references
    auto& [xr, yr, zr] = p;
    xr = 100.0;  // Modifies p.x
    std::cout << "After modification: p.x = " << p.x << std::endl;

    // With const reference
    const auto& [xcr, ycr, zcr] = p;
    // xcr = 200.0;  // Error: const reference
    std::cout << "Const access: " << xcr << std::endl;
}

void BindingsWithMultipleMembers() {
    std::cout << "\n=== Bindings with Multiple Members ===" << std::endl;

    PersonInfo alice{"Alice", 30, "NYC", 75000.0};

    auto [name, age, city, salary] = alice;
    std::cout << "Name:   " << name << std::endl;
    std::cout << "Age:    " << age << std::endl;
    std::cout << "City:   " << city << std::endl;
    std::cout << "Salary: " << salary << std::endl;
}

// Structured bindings in loops
void BindingsInLoops() {
    std::cout << "\n=== Bindings in Loops ===" << std::endl;

    std::vector<PersonInfo> people = {
        {"Alice", 30, "NYC", 75000.0},
        {"Bob", 25, "LA", 65000.0},
        {"Charlie", 35, "Chicago", 85000.0}
    };

    std::cout << "Using structured bindings in range-for:" << std::endl;
    for(const auto& [name, age, city, salary] : people) {
        std::cout << "  " << name << " (" << age << ") from "
            << city << " earns $" << salary << std::endl;
    }
}

// Bindings with std::map
void BindingsWithMaps() {
    std::cout << "\n=== Bindings with std::map ===" << std::endl;

    std::map<std::string, PersonInfo> database = {
        {"emp1", {"Alice", 30, "NYC", 75000.0}},
        {"emp2", {"Bob", 25, "LA", 65000.0}}
    };

    std::cout << "Iterating map with structured bindings:" << std::endl;
    for(const auto& [key, person] : database) {
        std::cout << "  " << key << ": " << person.name
            << " (" << person.age << ")" << std::endl;
    }

    // Nested bindings (C++17 doesn't support directly)
    std::cout << "\nAccessing nested members:" << std::endl;
    for(const auto& [key, person] : database) {
        // Can't do: auto& [id, [name, age, city, salary]] = ...
        // Must access person's members separately
        std::cout << "  " << key << " -> " << person.name << std::endl;
    }
}

// Bindings with arrays
struct RGB {
    unsigned char r, g, b;
};

void BindingsWithArrays() {
    std::cout << "\n=== Bindings with Arrays ===" << std::endl;

    // C-style array
    int arr[] = {1, 2, 3, 4, 5};
    auto [a, b, c, d, e] = arr;
    std::cout << "Array bindings: " << a << " " << b << " " << c
        << " " << d << " " << e << std::endl;

    // Struct that looks like array
    RGB color{255, 128, 0};
    auto [r, g, b] = color;
    std::cout << "RGB: r=" << +r << ", g=" << +g << ", b=" << +b << std::endl;
}

// Swapping with structured bindings
void SwapWithBindings() {
    std::cout << "\n=== Swapping with Bindings ===" << std::endl;

    int x = 10, y = 20;
    std::cout << "Before: x=" << x << ", y=" << y << std::endl;

    // Swap using structured bindings
    auto [a, b] = std::make_pair(y, x);
    x = a; y = b;

    std::cout << "After:  x=" << x << ", y=" << y << std::endl;

    // Alternative: std::swap
    std::swap(x, y);
    std::cout << "After std::swap: x=" << x << ", y=" << y << std::endl;
}

// Limitations
void BindingLimitations() {
    std::cout << "\n=== Binding Limitations ===" << std::endl;

    std::cout << "1. Can't use structured bindings in lambda captures (pre-C++20):" << std::endl;
    std::cout << "   auto [x, y] = point;" << std::endl;
    std::cout << "   auto lambda = [x, y]() { ... };  // ERROR in C++17" << std::endl;

    std::cout << "\n2. C++20 allows capturing structured bindings:" << std::endl;
    std::cout << "   auto lambda = [&x, &y]() { ... };  // OK in C++20" << std::endl;

    std::cout << "\n3. Can't have nested structured bindings:" << std::endl;
    std::cout << "   // auto [[a, b], [c, d]] = pair_of_pairs;  // NOT VALID" << std::endl;

    std::cout << "\n4. Number of names must match members exactly:" << std::endl;
    std::cout << "   struct has 3 members -> need exactly 3 names" << std::endl;

    std::cout << "\n5. All members must be public:" << std::endl;
    std::cout << "   Private members prevent structured bindings" << std::endl;
}