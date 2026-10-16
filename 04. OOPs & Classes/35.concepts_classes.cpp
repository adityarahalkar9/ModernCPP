import std;
#include "class_header.h"

// Concepts with classes
template <typename T>
concept Drawable = requires(const T & t) {
    { t.draw() } -> std::same_as<void>;
    { t.area() } -> std::convertible_to<double>;
};

// Class constrained by concept
template <Drawable T>
class Canvas {
    T shape;

public:
    explicit Canvas(T s) : shape(std::move(s)) {}

    void render() {
        std::cout << "  Rendering shape:" << std::endl;
        shape.draw();
        std::cout << "  Area: " << shape.area() << std::endl;
    }
};

class Circle4 {
    double radius;

public:
    Circle4(double r) : radius(r) {}

    void draw() const {
        std::cout << "  Circle with radius " << radius << std::endl;
    }

    double area() const {
        return 3.14159265358979 * radius * radius;
    }
};

void ConceptConstrainedClass() {
    std::cout << "=== Concepts with Classes (C++20) ===" << std::endl;

    std::cout << "Concepts constrain template parameters:" << std::endl;
    std::cout << "  Better error messages" << std::endl;
    std::cout << "  Clear requirements" << std::endl;

    Circle4 circle(5.0);
    Canvas<Circle4> canvas(circle);
    canvas.render();
}

// Concepts for class hierarchies
template <typename T>
concept InheritsFrom = std::is_base_of_v<Drawable, T>;

template <InheritsFrom T>
void process_drawable(T& obj) {
    std::cout << "  Processing inherited drawable" << std::endl;
    obj.draw();
}

void InheritanceConcepts() {
    std::cout << "\n=== Concepts with Inheritance ===" << std::endl;

    std::cout << "Concepts can check inheritance:" << std::endl;
    std::cout << "  std::is_base_of_v<Base, Derived>" << std::endl;

    Circle4 c(5.0);
    // process_drawable(c);  // ERROR if concept not satisfied
}

// Requires clause with classes
template <typename T>
    requires std::is_default_constructible_v<T>&& std::is_copy_constructible_v<T>
class Container4 {
    T value;

public:
    Container4() : value{} {}
    explicit Container4(T v) : value(v) {}

    T get() const { return value; }
    void set(T v) { value = v; }
};

void RequiresClause() {
    std::cout << "\n=== Requires Clause ===" << std::endl;

    std::cout << "'requires' specifies constraints:" << std::endl;

    Container4<int> ci(42);
    std::cout << "  int container: " << ci.get() << std::endl;

    Container4<std::string> cs("hello");
    std::cout << "  string container: " << cs.get() << std::endl;
}

// Concept-based overloading
template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

void process_value(Integral auto value) {
    std::cout << "  Processing integral: " << value << std::endl;
}

void process_value(FloatingPoint auto value) {
    std::cout << "  Processing floating: " << value << std::endl;
}

void ConceptOverloading() {
    std::cout << "\n=== Concept Overloading ===" << std::endl;

    std::cout << "Different behavior based on type category:" << std::endl;

    process_value(42);
    process_value(3.14);
}

// Concepts for polymorphism
template <typename T>
concept Polymorphic = requires(T t) {
    { t.virtual_method() };
    typename T::value_type;
    { T::static_method() };
};

void PolymorphicConcepts() {
    std::cout << "\n=== Concepts for Polymorphism ===" << std::endl;

    std::cout << "Concepts replace some virtual function use:" << std::endl;
    std::cout << "  Compile-time polymorphism" << std::endl;
    std::cout << "  No vtable overhead" << std::endl;

    std::cout << "\nWhen to use concepts:" << std::endl;
    std::cout << "  Type known at compile time" << std::endl;
    std::cout << "  Performance matters" << std::endl;

    std::cout << "\nWhen to use virtual:" << std::endl;
    std::cout << "  Type determined at runtime" << std::endl;
    std::cout << "  Plugin architectures" << std::endl;
}