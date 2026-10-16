import std;
#include "smart_pointers_header.h"

// Polymorphic hierarchy
class ShapeBase {
public:
    ShapeBase() = default;
    virtual ~ShapeBase() {
        std::cout << "  ShapeBase destroyed" << std::endl;
    }

    virtual double area() const = 0;
    virtual std::string name() const = 0;
    virtual void draw() const {
        std::cout << "  Drawing " << name() << " (area: " << area() << ")" << std::endl;
    }
};

class CircleShape : public ShapeBase {
    double radius_;

public:
    explicit CircleShape(double r) : radius_(r) {}

    ~CircleShape() override {
        std::cout << "  CircleShape destroyed" << std::endl;
    }

    double area() const override {
        return 3.14159265358979 * radius_ * radius_;
    }

    std::string name() const override { return "Circle"; }
};

class SquareShape : public ShapeBase {
    double side_;

public:
    explicit SquareShape(double s) : side_(s) {}

    ~SquareShape() override {
        std::cout << "  SquareShape destroyed" << std::endl;
    }

    double area() const override { return side_ * side_; }
    std::string name() const override { return "Square"; }
};

void PolymorphicUniquePtr() {
    std::cout << "=== Polymorphic unique_ptr ===" << std::endl;

    std::cout << "Base unique_ptr holding Derived:" << std::endl;
    std::unique_ptr<ShapeBase> shape = std::make_unique<CircleShape>(5.0);

    shape->draw();

    std::cout << "\nVirtual destructor ensures proper cleanup:" << std::endl;
    std::cout << "  Derived destructor called, then Base" << std::endl;
}

void PolymorphicSharedPtr() {
    std::cout << "\n=== Polymorphic shared_ptr ===" << std::endl;

    std::shared_ptr<ShapeBase> shape = std::make_shared<CircleShape>(3.0);
    shape->draw();

    std::cout << "\nCopy as base type:" << std::endl;
    std::shared_ptr<ShapeBase> same_shape = shape;  // Same object
    same_shape->draw();

    std::cout << "Same object: " << (shape.get() == same_shape.get()) << std::endl;
}

void PolymorphicContainers() {
    std::cout << "\n=== Polymorphic Containers ===" << std::endl;

    std::vector<std::unique_ptr<ShapeBase>> shapes;

    std::cout << "Adding different shapes:" << std::endl;
    shapes.push_back(std::make_unique<CircleShape>(2.0));
    shapes.push_back(std::make_unique<SquareShape>(4.0));
    shapes.push_back(std::make_unique<CircleShape>(1.5));

    std::cout << "\nDrawing all shapes:" << std::endl;
    for(const auto& shape : shapes) {
        shape->draw();
    }

    std::cout << "\nTotal area calculation:" << std::endl;
    double total = 0;
    for(const auto& shape : shapes) {
        total += shape->area();
    }
    std::cout << "  Total area: " << total << std::endl;

    std::cout << "\nSorting by area:" << std::endl;
    std::sort(shapes.begin(), shapes.end(),
              [](const auto& a, const auto& b) {
                  return a->area() < b->area();
              });

    for(const auto& shape : shapes) {
        std::cout << "  " << shape->name() << ": " << shape->area() << std::endl;
    }
}

void PolymorphicFactory() {
    std::cout << "\n=== Polymorphic Factory ===" << std::endl;

    auto create_shape = [](const std::string& type, double param)
        -> std::unique_ptr<ShapeBase> {
        if(type == "circle") {
            return std::make_unique<CircleShape>(param);
        }
        else if(type == "square") {
            return std::make_unique<SquareShape>(param);
        }
        return nullptr;
        };

    auto circle = create_shape("circle", 5.0);
    auto square = create_shape("square", 3.0);

    if(circle) circle->draw();
    if(square) square->draw();
}

void DowncastingSmartPointers() {
    std::cout << "\n=== Downcasting Smart Pointers ===" << std::endl;

    std::cout << "dynamic_pointer_cast for shared_ptr:" << std::endl;

    std::shared_ptr<ShapeBase> base = std::make_shared<CircleShape>(10.0);

    // Try to cast to derived
    auto circle = std::dynamic_pointer_cast<CircleShape>(base);
    if(circle) {
        std::cout << "  Successfully cast to CircleShape" << std::endl;
        std::cout << "  Area: " << circle->area() << std::endl;
    }

    // Try to cast to wrong type
    auto square = std::dynamic_pointer_cast<SquareShape>(base);
    if(!square) {
        std::cout << "  Cast to SquareShape failed (null)" << std::endl;
    }

    std::cout << "\nstatic_pointer_cast:" << std::endl;
    auto static_circle = std::static_pointer_cast<CircleShape>(base);
    std::cout << "  Static cast always succeeds (no runtime check)" << std::endl;
    std::cout << "  Use only when you're CERTAIN of the type!" << std::endl;
}