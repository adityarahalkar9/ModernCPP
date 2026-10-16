#include <iostream>
#include <string>
#include "structs_header.h"

struct Config {
    std::string name = "default";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    double scale = 1.0;
};

struct Vec3 {
    double x, y, z;
};

struct Nested {
    Vec3 position;
    Vec3 velocity;
    std::string label;
};

void InitializationForms() {
    std::cout << "=== Struct Initialization Forms ===" << std::endl;

    // 1. Default initialization (members may be uninitialized for POD)
    Vec3 v1;  // WARNING: x, y, z are uninitialized!

    // 2. Value initialization (zero for POD)
    Vec3 v2{};  // x=0, y=0, z=0

    // 3. Aggregate initialization (positional)
    Vec3 v3{1.0, 2.0, 3.0};

    // 4. Copy initialization
    Vec3 v4 = {1.0, 2.0, 3.0};

    // 5. Direct initialization
    Vec3 v5{1.0, 2.0, 3.0};

    std::cout << "v3: (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
    std::cout << "v4: (" << v4.x << ", " << v4.y << ", " << v4.z << ")" << std::endl;
    std::cout << "v5: (" << v5.x << ", " << v5.y << ", " << v5.z << ")" << std::endl;
}

void DesignatedInitializers() {
    std::cout << "\n=== Designated Initializers (C++20) ===" << std::endl;

    // C++20: Designated initializers (like C99 but more restricted)
    Config c1{
        .name = "my_app",
        .width = 1920,
        .height = 1080,
        .fullscreen = true
        // scale uses default: 1.0
    };

    std::cout << "c1:" << std::endl;
    std::cout << "  name:       " << c1.name << std::endl;
    std::cout << "  width:      " << c1.width << std::endl;
    std::cout << "  height:     " << c1.height << std::endl;
    std::cout << "  fullscreen: " << c1.fullscreen << std::endl;
    std::cout << "  scale:      " << c1.scale << std::endl;

    // Rules for designated initializers:
    // 1. Must be in declaration order
    // 2. Can skip fields (they use defaults)
    // 3. Cannot mix with positional initialization
    // 4. Designators must match member names exactly

    // Partial initialization
    Config c2{
        .width = 1024,
        .height = 768
        // name, fullscreen, scale use defaults
    };

    std::cout << "\nc2 (partial):" << std::endl;
    std::cout << "  name:       " << c2.name << std::endl;
    std::cout << "  width:      " << c2.width << std::endl;
}

void NestedInitialization() {
    std::cout << "\n=== Nested Struct Initialization ===" << std::endl;

    // Fully specified
    Nested n1{
        {1.0, 2.0, 3.0},     // position
        {0.5, 0.5, 0.5},     // velocity
        "particle"           // label
    };

    std::cout << "n1.position: (" << n1.position.x << ", "
        << n1.position.y << ", " << n1.position.z << ")" << std::endl;
    std::cout << "n1.velocity: (" << n1.velocity.x << ", "
        << n1.velocity.y << ", " << n1.velocity.z << ")" << std::endl;
    std::cout << "n1.label: " << n1.label << std::endl;

    // C++20 designated initializers with nesting
    Nested n2{
        .position = {10.0, 20.0, 30.0},
        .velocity = {1.0, 0.0, 0.0},
        .label = "nested"
    };

    std::cout << "\nn2.position.x = " << n2.position.x << std::endl;
}

void InitializerLists() {
    std::cout << "\n=== Initializer Lists ===" << std::endl;

    // std::initializer_list for aggregate-like initialization
    auto print_vec = [](std::initializer_list<double> list) {
        std::cout << "Values: ";
        for(double d : list) {
            std::cout << d << " ";
        }
        std::cout << std::endl;
        };

    print_vec({1.0, 2.0, 3.0});
    print_vec({4.5, 5.5});
    print_vec({});

    // Construction from initializer list
    std::vector<Vec3> vec = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "\nVector of Vec3:" << std::endl;
    for(const auto& v : vec) {
        std::cout << "  (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }
}

void InitializationBestPractices() {
    std::cout << "\n=== Initialization Best Practices ===" << std::endl;

    std::cout << "1. Always initialize struct members:" << std::endl;
    std::cout << "   - Use default member initializers" << std::endl;
    std::cout << "   - Or always use {} initialization" << std::endl;

    std::cout << "\n2. Prefer brace initialization {}:" << std::endl;
    std::cout << "   - Prevents narrowing conversions" << std::endl;
    std::cout << "   - Catches most initialization errors" << std::endl;

    std::cout << "\n3. Use designated initializers for readability:" << std::endl;
    std::cout << "   - Self-documenting code" << std::endl;
    std::cout << "   - Skipped fields get default values" << std::endl;

    std::cout << "\n4. Beware of most vexing parse:" << std::endl;
    std::cout << "   Vec3 v();  // FUNCTION DECLARATION, not struct!" << std::endl;
    std::cout << "   Vec3 v{};  // Correct: value initialization" << std::endl;
}