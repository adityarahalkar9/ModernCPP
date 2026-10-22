/*
    CONSTEXPR CLASSES - COMPILE-TIME OBJECTS
 
    WHAT IS A constexpr CLASS?
    A class whose constructor, member functions, etc. can be marked constexpr, 
    allowing objects to be created at compile time.
 
    THE IMPLICATIONS:
    1. Objects can exist at compile time (not just built-in types)
    2. Classes can have compile-time state
    3. You can do OOP at compile time!
 
    WHAT CAN BE constexpr IN A CLASS?
    - Constructors
    - Member functions
    - Static member variables
    - Operator overloads
 
   C++20 ADDITIONS:
    - Virtual functions can be constexpr
    - try/catch blocks (but throws are still compile-time errors)
    - Dynamic allocation (new/delete) in limited contexts
 */

import std;
#include <iostream>
#include <string>
#include "constexpr_header.h"

 // A constexpr class - everything can run at compile time
class Vector2 {
public:
    // constexpr constructor
    constexpr Vector2(double x, double y) : x_(x), y_(y) {}

    // constexpr member functions
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }

    constexpr double magnitude_squared() const {
        return x_ * x_ + y_ * y_;
    }

    constexpr Vector2 operator+(const Vector2& other) const {
        return Vector2(x_ + other.x_, y_ + other.y_);
    }

    constexpr Vector2 operator*(double scalar) const {
        return Vector2(x_ * scalar, y_ * scalar);
    }

    constexpr bool operator==(const Vector2& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

private:
    double x_, y_;
};

void ConstexprClassBasics() {
    std::cout << "=== constexpr Classes ===" << std::endl;

    /*
     * CREATING OBJECTS AT COMPILE TIME:
     * ================================================================
     */

     // Create vectors at compile time:
    constexpr Vector2 v1(3.0, 4.0);
    constexpr Vector2 v2(1.0, 2.0);

    // Do math at compile time:
    constexpr Vector2 sum = v1 + v2;
    constexpr Vector2 scaled = v1 * 2.0;

    // Compute properties at compile time:
    constexpr double mag_squared = v1.magnitude_squared();

    // Verify at compile time:
    static_assert(mag_squared == 25.0);  // 3² + 4² = 25 (3-4-5 triangle!)

    std::cout << "  v1 = (" << v1.x() << ", " << v1.y() << ")" << std::endl;
    std::cout << "  |v1|² = " << mag_squared << " (computed at compile time!)" << std::endl;
    std::cout << "  v1 + v2 = (" << sum.x() << ", " << sum.y() << ")" << std::endl;

    /*
     * WHAT JUST HAPPENED:
     * ==================
     * - Objects were CONSTRUCTED at compile time
     * - Operator overloads ran at compile time
     * - Member functions executed at compile time
     * - The results are baked into your binary!
     *
     * This is FULL OOP at compile time!
     */
}

void ConstexprInheritance() {
    std::cout << "\n=== constexpr with Inheritance ===" << std::endl;

    /*
     * EVEN INHERITANCE WORKS AT COMPILE TIME!
     * ================================================================
     */

    class Shape {
    public:
        constexpr Shape(double area) : area_(area) {}
        constexpr double area() const { return area_; }
    private:
        double area_;
    };

    class Circle : public Shape {
    public:
        constexpr Circle(double radius)
            : Shape(3.14159265358979 * radius * radius), radius_(radius) {
        }
        constexpr double radius() const { return radius_; }
    private:
        double radius_;
    };

    // Create derived objects at compile time:
    constexpr Circle unit_circle(1.0);
    constexpr double area = unit_circle.area();

    static_assert(area > 3.14 && area < 3.15);
    std::cout << "  Unit circle area: " << area << " (computed at compile time)" << std::endl;

    std::cout << R"(
    WHAT'S POSSIBLE:
    ┌─────────────────────────────────────────────────────┐
    │ • Constructors at compile time                      │
    │ • Member functions at compile time                  │
    │ • Operator overloads at compile time                │
    │ • Inheritance hierarchies at compile time           │
    │ • Encapsulation at compile time                     │
    │                                                     │
    │ YOU'RE WRITING NORMAL OOP CODE                      │
    │ THAT HAPPENS TO RUN DURING COMPILATION!             │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void CompileTimeObjects() {
    std::cout << "\n=== Practical Compile-Time Objects ===" << std::endl;

    /*
     * REAL-WORLD USE CASES:
     * ================================================================
     */

     // A compile-time matrix class:
    class Matrix2x2 {
    public:
        constexpr Matrix2x2(double a, double b, double c, double d)
            : data_{a, b, c, d} {
        }

        constexpr Matrix2x2 multiply(const Matrix2x2& other) const {
            return Matrix2x2(
                data_[0] * other.data_[0] + data_[1] * other.data_[2],
                data_[0] * other.data_[1] + data_[1] * other.data_[3],
                data_[2] * other.data_[0] + data_[3] * other.data_[2],
                data_[2] * other.data_[1] + data_[3] * other.data_[3]
            );
        }

        constexpr double determinant() const {
            return data_[0] * data_[3] - data_[1] * data_[2];
        }

        constexpr double at(size_t row, size_t col) const {
            return data_[row * 2 + col];
        }

    private:
        double data_[4];
    };

    // Compute at compile time:
    constexpr Matrix2x2 rotation = Matrix2x2(
        0.7071067811865476, -0.7071067811865476,   // cos(45°), -sin(45°)
        0.7071067811865476, 0.7071067811865476     // sin(45°), cos(45°)
    );

    constexpr Matrix2x2 identity = Matrix2x2(1, 0, 0, 1);
    constexpr Matrix2x2 rotation_squared = rotation.multiply(rotation);

    // Rotation by 45° twice = rotation by 90°
    // The determinant of a rotation matrix is always 1
    static_assert(rotation.determinant() > 0.999 && rotation.determinant() < 1.001);

    std::cout << "  Rotation matrix determinant: " << rotation.determinant() << std::endl;
    std::cout << "  (Computed at compile time!)" << std::endl;

    std::cout << R"(
    APPLICATIONS:
    ┌─────────────────────────────────────────────────────┐
    │ • Game development: precomputed physics constants   │
    │ • Graphics: transformation matrices                 │
    │ • Engineering: precomputed lookup tables            │
    │ • Simulation: initial states computed once          │
    │ • Any expensive computation done once               │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}