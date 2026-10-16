import std;
#include "class_header.h"

// Covariant return types with raw pointers
class Animal {
public:
    virtual ~Animal() = default;

    // Returns Animal* - can be overridden to return derived*
    virtual Animal* clone() const {
        return new Animal(*this);
    }

    virtual void speak() const {
        std::cout << "  Animal speaks" << std::endl;
    }
};

class Dog : public Animal {
public:
    // Covariant return: Dog* instead of Animal*
    // This is allowed and useful!
    Dog* clone() const override {
        return new Dog(*this);
    }

    void speak() const override {
        std::cout << "  Dog barks" << std::endl;
    }

    void fetch() const {
        std::cout << "  Dog fetches" << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat* clone() const override {
        return new Cat(*this);
    }

    void speak() const override {
        std::cout << "  Cat meows" << std::endl;
    }
};

void CovariantReturnBasics() {
    std::cout << "=== Covariant Return Types ===" << std::endl;

    std::cout << "Virtual function can return:" << std::endl;
    std::cout << "  Base* in base class" << std::endl;
    std::cout << "  Derived* in derived class" << std::endl;
    std::cout << "  (covariant return type)" << std::endl;

    Dog original_dog;

    // Covariant return allows:
    Dog* cloned_dog = original_dog.clone();  // No cast needed!

    cloned_dog->speak();
    cloned_dog->fetch();  // Can call Dog-specific method directly

    delete cloned_dog;
}

// Covariant with smart pointers (workaround needed)
class SmartAnimal {
public:
    virtual ~SmartAnimal() = default;

    // Problem: unique_ptr<Derived> is different type
    // Can't directly use covariant returns

    // Workaround: virtual implementation method
    virtual SmartAnimal* do_clone() const = 0;

    // Public interface returns smart pointer
    std::unique_ptr<SmartAnimal> clone() const {
        return std::unique_ptr<SmartAnimal>(do_clone());
    }

    virtual void describe() const = 0;
};

class SmartDog : public SmartAnimal {
public:
    SmartDog* do_clone() const override {  // Covariant here
        return new SmartDog(*this);
    }

    void describe() const override {
        std::cout << "  SmartDog described" << std::endl;
    }
};

void CovariantWithSmartPointers() {
    std::cout << "\n=== Covariant with Smart Pointers ===" << std::endl;

    std::cout << "Problem:" << std::endl;
    std::cout << "  unique_ptr<Base> != unique_ptr<Derived>" << std::endl;
    std::cout << "  Can't use covariant directly" << std::endl;

    std::cout << "\nWorkaround: NVI pattern" << std::endl;
    std::cout << "  Public non-virtual returns smart pointer" << std::endl;
    std::cout << "  Private virtual returns raw pointer (covariant OK)" << std::endl;

    SmartDog dog;
    auto cloned = dog.clone();  // Returns unique_ptr<SmartAnimal>

    cloned->describe();
}

// Covariant with references
class Handler {
public:
    virtual ~Handler() = default;
    virtual Handler& configure() { return *this; }
};

class SpecialHandler : public Handler {
public:
    // Covariant with references too
    SpecialHandler& configure() override {
        std::cout << "  SpecialHandler configured" << std::endl;
        return *this;
    }

    SpecialHandler& extra_setup() {
        std::cout << "  Extra setup done" << std::endl;
        return *this;
    }
};

void CovariantWithReferences() {
    std::cout << "\n=== Covariant with References ===" << std::endl;

    SpecialHandler handler;
    handler.configure().extra_setup();  // Can chain derived methods

    std::cout << "\nEnables method chaining on derived types" << std::endl;
}

// Real-world example: clone pattern
class Shape2 {
public:
    virtual ~Shape2() = default;
    virtual Shape2* clone() const = 0;
    virtual void draw() const = 0;
    virtual double area() const = 0;
};

class Circle2 : public Shape2 {
    double radius;

public:
    Circle2(double r) : radius(r) {}

    Circle2* clone() const override {
        return new Circle2(*this);
    }

    void draw() const override {
        std::cout << "  Circle with radius " << radius << std::endl;
    }

    double area() const override {
        return 3.14159265358979 * radius * radius;
    }
};

class Square2 : public Shape2 {
    double side;

public:
    Square2(double s) : side(s) {}

    Square2* clone() const override {
        return new Square2(*this);
    }

    void draw() const override {
        std::cout << "  Square with side " << side << std::endl;
    }

    double area() const override {
        return side * side;
    }
};

void ClonePattern() {
    std::cout << "\n=== Clone Pattern ===" << std::endl;

    std::cout << "Covariant returns make clone() more useful:" << std::endl;

    std::vector<std::unique_ptr<Shape2>> shapes;
    shapes.push_back(std::make_unique<Circle2>(5.0));
    shapes.push_back(std::make_unique<Square2>(4.0));

    // Clone each shape
    std::vector<std::unique_ptr<Shape2>> copies;
    for(const auto& shape : shapes) {
        copies.emplace_back(shape->clone());
    }

    std::cout << "\nOriginals:" << std::endl;
    for(const auto& shape : shapes) {
        shape->draw();
    }

    std::cout << "\nCopies:" << std::endl;
    for(const auto& shape : copies) {
        shape->draw();
    }
}

// Rules for covariant returns
void CovariantRules() {
    std::cout << "\n=== Covariant Return Rules ===" << std::endl;

    std::cout << "Covariant return allowed when:" << std::endl;
    std::cout << "  Returning pointer or reference" << std::endl;
    std::cout << "  Derived type derives from base return type" << std::endl;
    std::cout << "  Same cv-qualification or less const" << std::endl;

    std::cout << "\nNOT allowed for:" << std::endl;
    std::cout << "  Value returns (must be exact same type)" << std::endl;
    std::cout << "  Smart pointers (different template instantiation)" << std::endl;

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  More precise types" << std::endl;
    std::cout << "  Fewer casts needed" << std::endl;
    std::cout << "  Better type safety" << std::endl;
}