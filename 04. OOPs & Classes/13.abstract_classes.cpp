import std;
#include "class_header.h"

// Pure virtual functions and abstract classes
class Shape {
public:
    // Pure virtual function: must be overridden
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;

    // Virtual function with default implementation
    virtual void describe() const {
        std::cout << "  " << name() << ": area=" << area()
            << ", perimeter=" << perimeter() << std::endl;
    }

    // Virtual destructor for proper cleanup
    virtual ~Shape() = default;
};

// Concrete derived classes
class Circle : public Shape {
    double radius;

public:
    explicit Circle(double r) : radius(r) {}

    double area() const override {
        return 3.14159265358979 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159265358979 * radius;
    }

    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double a, b, c;

public:
    Triangle(double s1, double s2, double s3) : a(s1), b(s2), c(s3) {}

    double area() const override {
        // Heron's formula
        double s = perimeter() / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    double perimeter() const override { return a + b + c; }
    std::string name() const override { return "Triangle"; }
};

void AbstractClassBasics() {
    std::cout << "=== Abstract Classes ===" << std::endl;

    std::cout << "Abstract class characteristics:" << std::endl;
    std::cout << "  - Has at least one pure virtual function (= 0)" << std::endl;
    std::cout << "  - CANNOT be instantiated directly" << std::endl;
    std::cout << "  - Can have implementation for some methods" << std::endl;
    std::cout << "  - Derived MUST override pure virtuals" << std::endl;

    std::cout << "\nPolymorphic usage:" << std::endl;
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));

    for(const auto& shape : shapes) {
        shape->describe();
    }
}

// Partially abstract classes
class PartialAbstract {
public:
    // Pure virtual: must override
    virtual int required_method() = 0;

    // Virtual with default: may override
    virtual int optional_method() {
        return required_method() * 2;
    }

    // Non-virtual: cannot override (only hide)
    int fixed_method() {
        return 42;
    }

    virtual ~PartialAbstract() = default;
};

class PartialImpl : public PartialAbstract {
public:
    int required_method() override { return 10; }
    // optional_method uses default implementation
};

void PartialAbstraction() {
    std::cout << "\n=== Partial Abstraction ===" << std::endl;

    PartialImpl impl;
    std::cout << "required_method: " << impl.required_method() << std::endl;
    std::cout << "optional_method: " << impl.optional_method() << std::endl;
    std::cout << "fixed_method: " << impl.fixed_method() << std::endl;

    std::cout << "\nDesign choices:" << std::endl;
    std::cout << "  Pure virtual: 'must provide'" << std::endl;
    std::cout << "  Virtual (default): 'may override'" << std::endl;
    std::cout << "  Non-virtual: 'cannot override'" << std::endl;
}

// Interface pattern (all pure virtual)
class Drawable {
public:
    virtual void draw() const = 0;
    virtual ~Drawable() = default;
};

class Printable {
public:
    virtual void print() const = 0;
    virtual ~Printable() = default;
};

class Serializable {
public:
    virtual std::string serialize() const = 0;
    virtual ~Serializable() = default;
};

// Class implementing multiple interfaces
class Document : public Drawable, public Printable, public Serializable {
    std::string content;

public:
    Document(std::string c) : content(std::move(c)) {}

    void draw() const override {
        std::cout << "  Drawing: " << content << std::endl;
    }

    void print() const override {
        std::cout << "  Printing: " << content << std::endl;
    }

    std::string serialize() const override {
        return "Document(" + content + ")";
    }
};

void InterfacePattern() {
    std::cout << "\n=== Interface Pattern ===" << std::endl;

    Document doc("Hello World");

    // Can use as any of its interfaces
    Drawable& drawable = doc;
    drawable.draw();

    Printable& printable = doc;
    printable.print();

    Serializable& serializable = doc;
    std::cout << "  Serialized: " << serializable.serialize() << std::endl;

    std::cout << "\nInterfaces in C++:" << std::endl;
    std::cout << "  Class with all pure virtual functions" << std::endl;
    std::cout << "  No data members" << std::endl;
    std::cout << "  Virtual destructor" << std::endl;
    std::cout << "  Defines a contract" << std::endl;
}

// Pure virtual with implementation
class BaseWithImpl {
public:
    virtual void method() = 0;  // Pure virtual

    // Can still provide implementation
    virtual void helper() {
        std::cout << "  Base helper" << std::endl;
        method();  // Calls derived version
    }

    virtual ~BaseWithImpl() = default;
};

// Provide implementation for pure virtual outside class
void BaseWithImpl::method() {
    std::cout << "  Base default method" << std::endl;
}

class DerivedOverride : public BaseWithImpl {
public:
    void method() override {
        std::cout << "  Derived method" << std::endl;
    }
};

class DerivedUseDefault : public BaseWithImpl {
public:
    // Can explicitly use base's implementation of pure virtual
    void method() override {
        std::cout << "  Using base's method:" << std::endl;
        BaseWithImpl::method();
    }
};

void PureVirtualWithImpl() {
    std::cout << "\n=== Pure Virtual with Implementation ===" << std::endl;

    std::cout << "Pure virtual CAN have implementation:" << std::endl;
    std::cout << "  Derived can call Base::method()" << std::endl;
    std::cout << "  But still MUST override" << std::endl;

    DerivedOverride d1;
    d1.method();

    DerivedUseDefault d2;
    d2.method();
}

// Cannot instantiate abstract class
void AbstractInstantiation() {
    std::cout << "\n=== Abstract Class Restrictions ===" << std::endl;

    std::cout << "CANNOT do:" << std::endl;
    std::cout << "  Shape s;                    // Error: abstract" << std::endl;
    std::cout << "  Shape* p = new Shape();     // Error: abstract" << std::endl;
    std::cout << "  Shape arr[10];              // Error: abstract" << std::endl;

    std::cout << "\nCAN do:" << std::endl;
    std::cout << "  Shape& ref = concrete;      // Reference to concrete" << std::endl;
    std::cout << "  Shape* ptr = &concrete;     // Pointer to concrete" << std::endl;
    std::cout << "  Shape* p = new Circle(5);   // Pointer to heap concrete" << std::endl;
}