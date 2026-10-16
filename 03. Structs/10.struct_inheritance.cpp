#include <iostream>
#include <string>
#include "structs_header.h"

// Basic struct inheritance
struct Base {
    int base_member = 10;

    void base_method() {
        std::cout << "Base method called" << std::endl;
    }
};

struct Derived : Base {
    int derived_member = 20;

    void derived_method() {
        std::cout << "Derived method called" << std::endl;
    }
};

void BasicInheritance() {
    std::cout << "=== Basic Struct Inheritance ===" << std::endl;

    Derived d;
    std::cout << "d.base_member = " << d.base_member << std::endl;
    std::cout << "d.derived_member = " << d.derived_member << std::endl;

    d.base_method();
    d.derived_method();

    std::cout << "\nsizeof(Base) = " << sizeof(Base) << std::endl;
    std::cout << "sizeof(Derived) = " << sizeof(Derived) << std::endl;

    std::cout << "\nDerived inherits from Base:" << std::endl;
    std::cout << "  - Gets all public/protected members" << std::endl;
    std::cout << "  - Memory layout: Base members first, then Derived" << std::endl;
}

// Multi-level inheritance
struct A { int a; };
struct B : A { int b; };
struct C : B { int c; };

void MultiLevelInheritance() {
    std::cout << "\n=== Multi-level Inheritance ===" << std::endl;

    C obj;
    obj.a = 1;
    obj.b = 2;
    obj.c = 3;

    std::cout << "a=" << obj.a << ", b=" << obj.b << ", c=" << obj.c << std::endl;

    std::cout << "\nMemory layout of C:" << std::endl;
    std::cout << "  offset(a) = " << offsetof(C, a) << std::endl;
    std::cout << "  offset(b) = " << offsetof(C, b) << std::endl;
    std::cout << "  offset(c) = " << offsetof(C, c) << std::endl;

    std::cout << "\nsizeof(C) = " << sizeof(C) << std::endl;
}

// Inheritance with same-named members
struct Parent {
    int value = 100;

    void print() {
        std::cout << "Parent value: " << value << std::endl;
    }
};

struct Child : Parent {
    int value = 200;  // Hides Parent::value

    void print() {  // Hides Parent::print
        std::cout << "Child value: " << value << std::endl;
    }

    void print_parent() {
        Parent::print();  // Explicitly call parent version
        std::cout << "Parent value via scope: " << Parent::value << std::endl;
    }
};

void MemberHiding() {
    std::cout << "\n=== Member Hiding ===" << std::endl;

    Child c;
    c.print();         // Calls Child::print
    c.print_parent();  // Accesses Parent members

    // Access parent's hidden members
    std::cout << "\nParent's value: " << c.Parent::value << std::endl;
    c.Parent::print();
}

// Inheritance and aggregates (C++17)
struct AggregateBase {
    int base_val;
};

struct AggregateDerived : AggregateBase {
    int derived_val;
};

void AggregateInheritance() {
    std::cout << "\n=== Aggregates with Inheritance (C++17) ===" << std::endl;

    static_assert(std::is_aggregate_v<AggregateDerived>);

    // Initialize with aggregate initialization
    AggregateDerived ad{{42}, 100};  // {base_val}, derived_val

    std::cout << "ad.base_val = " << ad.base_val << std::endl;
    std::cout << "ad.derived_val = " << ad.derived_val << std::endl;

    std::cout << "\nC++17 allows aggregate initialization" << std::endl;
    std::cout << "for structs with base classes" << std::endl;
}

// Composition vs inheritance
struct Engine {
    int horsepower = 300;

    void start() {
        std::cout << "Engine started (" << horsepower << " hp)" << std::endl;
    }
};

struct Car1 : Engine {  // Inheritance: Car IS-A Engine (wrong!)
    std::string model = "Unknown";
};

struct Car2 {  // Composition: Car HAS-A Engine (correct!)
    Engine engine;
    std::string model = "Unknown";

    void start() {
        engine.start();
        std::cout << "Car " << model << " started" << std::endl;
    }
};

void CompositionVsInheritance() {
    std::cout << "\n=== Composition vs Inheritance ===" << std::endl;

    std::cout << "Inheritance (IS-A relationship):" << std::endl;
    std::cout << "  Use when derived type IS a kind of base type" << std::endl;
    std::cout << "  Example: Circle IS-A Shape" << std::endl;

    std::cout << "\nComposition (HAS-A relationship):" << std::endl;
    std::cout << "  Use when type CONTAINS another type" << std::endl;
    std::cout << "  Example: Car HAS-A Engine" << std::endl;

    std::cout << "\nPrefer composition over inheritance:" << std::endl;
    std::cout << "  - More flexible" << std::endl;
    std::cout << "  - Less coupling" << std::endl;
    std::cout << "  - Easier to test" << std::endl;
    std::cout << "  - Can swap components at runtime" << std::endl;

    Car2 car;
    car.model = "Tesla";
    car.start();
}

// Protected inheritance (rarely used)
struct ProtectedBase {
protected:
    int protected_member = 42;
};

struct PublicDerived : ProtectedBase {
    // Can access protected members of Base
public:
    int get_protected() {
        return protected_member;
    }
};

void AccessLevels() {
    std::cout << "\n=== Access Levels in Inheritance ===" << std::endl;

    PublicDerived pd;
    std::cout << "protected_member via method: " << pd.get_protected() << std::endl;

    std::cout << "\nAccess levels:" << std::endl;
    std::cout << "  public:    accessible everywhere" << std::endl;
    std::cout << "  protected: accessible in derived classes" << std::endl;
    std::cout << "  private:   only in own class" << std::endl;
}