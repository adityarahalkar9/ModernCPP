import std;
#include "structs_header.h"

// Basic nested structs
struct Address {
    std::string street;
    std::string city;
    std::string zip;
};

struct Contact {
    std::string name;
    Address address;      // Nested struct
    std::string phone;
};

void BasicNestedStructs() {
    std::cout << "=== Basic Nested Structs ===" << std::endl;

    Contact contact{
        "Alice",
        {"123 Main St", "Springfield", "12345"},
        "555-1234"
    };

    std::cout << "Name: " << contact.name << std::endl;
    std::cout << "Street: " << contact.address.street << std::endl;
    std::cout << "City: " << contact.address.city << std::endl;
    std::cout << "ZIP: " << contact.address.zip << std::endl;
    std::cout << "Phone: " << contact.phone << std::endl;
}

// Deeply nested structs
struct Dimensions {
    double width, height, depth;
};

struct Material {
    std::string type;
    double density;
};

struct Product {
    std::string name;
    Dimensions dimensions;
    Material material;
    double price;
};

void DeepNesting() {
    std::cout << "\n=== Deeply Nested Structs ===" << std::endl;

    Product product{
        "Box",
        {10.0, 20.0, 30.0},           // dimensions
        {"cardboard", 0.5},            // material
        9.99                            // price
    };

    std::cout << "Product: " << product.name << std::endl;
    std::cout << "Dimensions: " << product.dimensions.width << " x "
        << product.dimensions.height << " x "
        << product.dimensions.depth << std::endl;
    std::cout << "Material: " << product.material.type
        << " (density: " << product.material.density << ")" << std::endl;
    std::cout << "Price: $" << product.price << std::endl;
}

// Nested structs with arrays
struct Student {
    std::string name;
    struct Grade {
        std::string subject;
        double score;
    };
    std::vector<Grade> grades;
};

void NestedWithArrays() {
    std::cout << "\n=== Nested Structs with Arrays ===" << std::endl;

    Student student{
        "Alice",
        {
            {"Math", 95.5},
            {"Physics", 88.0},
            {"Chemistry", 92.5}
        }
    };

    std::cout << "Student: " << student.name << std::endl;
    std::cout << "Grades:" << std::endl;
    for(const auto& grade : student.grades) {
        std::cout << "  " << grade.subject << ": " << grade.score << std::endl;
    }

    double total = 0;
    for(const auto& grade : student.grades) {
        total += grade.score;
    }
    std::cout << "Average: " << total / student.grades.size() << std::endl;
}

// Nested structs in different scopes
struct Outer {
    struct Inner1 {
        int x;
    };

    struct Inner2 {
        int y;
    };

    Inner1 a;
    Inner2 b;
};

void ScopedNestedStructs() {
    std::cout << "\n=== Scoped Nested Structs ===" << std::endl;

    Outer o;
    o.a.x = 10;
    o.b.y = 20;

    std::cout << "o.a.x = " << o.a.x << std::endl;
    std::cout << "o.b.y = " << o.b.y << std::endl;

    // Inner types are scoped to Outer
    Outer::Inner1 inner1{100};
    Outer::Inner2 inner2{200};

    std::cout << "\nStandalone Inner1: " << inner1.x << std::endl;
    std::cout << "Standalone Inner2: " << inner2.y << std::endl;
}

// Nested structs with inheritance
struct BaseNested {
    struct Inner {
        int value;
    };
};

struct DerivedNested : BaseNested {
    // Can use Base's nested types
    void use_inner() {
        BaseNested::Inner i{42};
        std::cout << "Inner value: " << i.value << std::endl;
    }
};

void NestedWithInheritance() {
    std::cout << "\n=== Nested Structs with Inheritance ===" << std::endl;

    DerivedNested d;
    d.use_inner();

    std::cout << "\nNested types are inherited:" << std::endl;
    std::cout << "  Derived can access Base::Inner" << std::endl;
    std::cout << "  But scoped to Base's name" << std::endl;
}

// Accessing nested struct members
struct Complex {
    struct Level1 {
        struct Level2 {
            struct Level3 {
                int deepest;
            };
            Level3 three;
        };
        Level2 two;
    };
    Level1 one;
};

void DeepNestedAccess() {
    std::cout << "\n=== Deep Nested Access ===" << std::endl;

    Complex c{};
    c.one.two.three.deepest = 42;

    std::cout << "Deepest value: " << c.one.two.three.deepest << std::endl;

    std::cout << "\nDeep nesting can be unwieldy:" << std::endl;
    std::cout << "  - Hard to read and maintain" << std::endl;
    std::cout << "  - Consider flattening or using composition" << std::endl;
    std::cout << "  - Use 'using' aliases for readability" << std::endl;

    // Using aliases
    using L2 = Complex::Level1::Level2;
    using L3 = Complex::Level1::Level2::Level3;

    L3 deep{};
    deep.deepest = 100;
    std::cout << "Alias access: " << deep.deepest << std::endl;
}