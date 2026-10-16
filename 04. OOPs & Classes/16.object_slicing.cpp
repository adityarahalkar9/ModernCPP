import std;
#include "class_header.h"

// Object slicing demonstration
class Base {
public:
    int base_data;

    Base(int b = 0) : base_data(b) {}

    virtual void print() const {
        std::cout << "  Base: " << base_data << std::endl;
    }

    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int derived_data;

    Derived(int b, int d) : Base(b), derived_data(d) {}

    void print() const override {
        std::cout << "  Derived: base=" << base_data
            << ", derived=" << derived_data << std::endl;
    }
};

void SlicingDemo() {
    std::cout << "=== Object Slicing ===" << std::endl;

    Derived d(42, 100);
    d.print();  // Full derived object

    std::cout << "\nAssigning to Base by VALUE:" << std::endl;
    Base b = d;  // SLICING HAPPENS!
    b.print();   // Only Base part remains

    std::cout << "\nWhat happened:" << std::endl;
    std::cout << "  - Only Base members were copied" << std::endl;
    std::cout << "  - derived_data was LOST" << std::endl;
    std::cout << "  - virtual dispatch now calls Base::print" << std::endl;
}

// Slicing in containers
void SlicingInContainers() {
    std::cout << "\n=== Slicing in Containers ===" << std::endl;

    std::cout << "DANGEROUS: vector<Base>" << std::endl;
    std::vector<Base> vec;

    vec.push_back(Derived(1, 100));  // SLICED!
    vec.push_back(Derived(2, 200));  // SLICED!

    std::cout << "Contents (all sliced to Base):" << std::endl;
    for(const auto& b : vec) {
        b.print();  // Always calls Base::print
    }

    std::cout << "\nSOLUTION: vector<unique_ptr<Base>>" << std::endl;
    std::vector<std::unique_ptr<Base>> smart_vec;

    smart_vec.push_back(std::make_unique<Derived>(1, 100));
    smart_vec.push_back(std::make_unique<Derived>(2, 200));

    std::cout << "Contents (polymorphic):" << std::endl;
    for(const auto& b : smart_vec) {
        b->print();  // Calls correct Derived::print
    }
}

// Slicing in function parameters
void process_by_value(Base b) {  // Takes by value: SLICING!
    b.print();
}

void process_by_ref(const Base& b) {  // Takes by reference: NO slicing
    b.print();
}

void SlicingInFunctions() {
    std::cout << "\n=== Slicing in Function Parameters ===" << std::endl;

    Derived d(42, 100);

    std::cout << "By value (sliced):" << std::endl;
    process_by_value(d);

    std::cout << "\nBy reference (polymorphic):" << std::endl;
    process_by_ref(d);
}

// Accidental slicing with assignment
class Polymorphic {
public:
    virtual void describe() const {
        std::cout << "  Polymorphic base" << std::endl;
    }
    virtual ~Polymorphic() = default;
};

class Specialized : public Polymorphic {
public:
    void describe() const override {
        std::cout << "  Specialized derived" << std::endl;
    }
};

void AssignmentSlicing() {
    std::cout << "\n=== Assignment Slicing ===" << std::endl;

    Specialized s;
    s.describe();

    std::cout << "\nAssigning to base by value:" << std::endl;
    Polymorphic p = s;  // Slicing
    p.describe();  // Now behaves as base

    std::cout << "\nOriginal still correct:" << std::endl;
    s.describe();
}

// When slicing might be intentional
class Coordinates {
public:
    double x, y;
    Coordinates(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
};

class ColoredPoint : public Coordinates {
public:
    std::string color;
    ColoredPoint(double x, double y, std::string c)
        : Coordinates(x, y), color(std::move(c)) {
    }
};

void IntentionalSlicing() {
    std::cout << "\n=== Intentional Slicing (Rare) ===" << std::endl;

    ColoredPoint cp(10.0, 20.0, "red");
    std::cout << "Colored: (" << cp.x << ", " << cp.y << ") color=" << cp.color << std::endl;

    // Intentionally take just the coordinates
    Coordinates coords = cp;  // Slice to just x, y
    std::cout << "Just coords: (" << coords.x << ", " << coords.y << ")" << std::endl;

    std::cout << "\nSometimes you want just the base part" << std::endl;
    std::cout << "But this is rare and should be explicit" << std::endl;
}

// Preventing slicing
class NonSlicable {
public:
    NonSlicable() = default;
    virtual ~NonSlicable() = default;

    // Make copy constructor protected to prevent slicing
protected:
    NonSlicable(const NonSlicable&) = default;
    NonSlicable& operator=(const NonSlicable&) = default;
};

void PreventingSlicing() {
    std::cout << "\n=== Preventing Slicing ===" << std::endl;

    std::cout << "Techniques to prevent slicing:" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Use references/pointers:" << std::endl;
    std::cout << "   void func(Base& b)   // No slicing" << std::endl;
    std::cout << "   void func(Base* b)   // No slicing" << std::endl;
    std::cout << std::endl;
    std::cout << "2. Use smart pointers:" << std::endl;
    std::cout << "   vector<unique_ptr<Base>>  // No slicing" << std::endl;
    std::cout << std::endl;
    std::cout << "3. Protected copy constructor:" << std::endl;
    std::cout << "   Prevents accidental copying" << std::endl;
    std::cout << std::endl;
    std::cout << "4. 'final' on inheritance:" << std::endl;
    std::cout << "   class Base final;  // Can't derive" << std::endl;
}