import std;
#include "class_header.h"

// Without virtual destructor (PROBLEM!)
class BadBase {
public:
    BadBase() { std::cout << "  BadBase constructed" << std::endl; }
    ~BadBase() { std::cout << "  BadBase destroyed" << std::endl; }  // NOT virtual!
};

class BadDerived : public BadBase {
private:
    int* expensive_resource;

public:
    BadDerived() : expensive_resource(new int[1000]) {
        std::cout << "  BadDerived constructed (allocated resource)" << std::endl;
    }

    ~BadDerived() {
        std::cout << "  BadDerived destroyed (freeing resource)" << std::endl;
        delete[] expensive_resource;
    }
};

void WithoutVirtualDestructor() {
    std::cout << "=== Without Virtual Destructor (LEAK!) ===" << std::endl;

    std::cout << "Deleting through base pointer:" << std::endl;
    BadBase* ptr = new BadDerived();
    delete ptr;  // Only ~BadBase called! ~BadDerived never runs!

    std::cout << "\nPROBLEM: Derived destructor never called!" << std::endl;
    std::cout << "Resource leaks (1000 ints not freed)" << std::endl;
}

// With virtual destructor (CORRECT)
class GoodBase {
public:
    GoodBase() { std::cout << "  GoodBase constructed" << std::endl; }
    virtual ~GoodBase() { std::cout << "  GoodBase destroyed" << std::endl; }  // virtual!
};

class GoodDerived : public GoodBase {
private:
    int* expensive_resource;

public:
    GoodDerived() : expensive_resource(new int[1000]) {
        std::cout << "  GoodDerived constructed (allocated resource)" << std::endl;
    }

    ~GoodDerived() override {
        std::cout << "  GoodDerived destroyed (freeing resource)" << std::endl;
        delete[] expensive_resource;
    }
};

void WithVirtualDestructor() {
    std::cout << "\n=== With Virtual Destructor (SAFE) ===" << std::endl;

    std::cout << "Deleting through base pointer:" << std::endl;
    GoodBase* ptr = new GoodDerived();
    delete ptr;  // Both destructors called correctly!

    std::cout << "\nDerived destructor properly called" << std::endl;
}

// Virtual destructor with smart pointers
void SmartPointerVirtualDestructor() {
    std::cout << "\n=== Smart Pointers ===" << std::endl;

    std::cout << "unique_ptr with virtual destructor:" << std::endl;
    {
        std::unique_ptr<GoodBase> ptr = std::make_unique<GoodDerived>();
        std::cout << "  (using unique_ptr)" << std::endl;
    }  // Correct destruction

    std::cout << "\nunique_ptr with non-virtual destructor:" << std::endl;
    {
        // This still works! unique_ptr remembers actual type
        std::unique_ptr<BadDerived> ptr = std::make_unique<BadDerived>();
        std::cout << "  (using unique_ptr to derived)" << std::endl;
    }

    std::cout << "\nshared_ptr can use non-virtual destructor:" << std::endl;
    {
        std::shared_ptr<BadBase> ptr = std::make_shared<BadDerived>();
        std::cout << "  (shared_ptr remembers deleter)" << std::endl;
    }  // Works correctly even without virtual destructor!

    std::cout << "\nBut raw new/delete through base NEEDS virtual!" << std::endl;
}

// Protected non-virtual destructor (alternative pattern)
class Interface {
protected:
    ~Interface() = default;  // Protected: can't delete through this pointer
public:
    virtual void method() = 0;
};

class Implementation : public Interface {
public:
    void method() override { std::cout << "  Implementation::method" << std::endl; }
};

void ProtectedDestructorPattern() {
    std::cout << "\n=== Protected Destructor Pattern ===" << std::endl;

    std::cout << "For abstract interfaces:" << std::endl;
    std::cout << "  Option 1: public virtual destructor" << std::endl;
    std::cout << "  Option 2: protected non-virtual destructor" << std::endl;

    std::cout << "\nProtected prevents:" << std::endl;
    std::cout << "  delete basePtr;  // Compile error!" << std::endl;

    std::cout << "\nAllows only:" << std::endl;
    std::cout << "  delete derivedPtr;  // OK" << std::endl;

    Implementation impl;
    impl.method();
}

// When you need virtual destructors
void WhenVirtualDestructor() {
    std::cout << "\n=== When to Use Virtual Destructors ===" << std::endl;

    std::cout << "NEED virtual destructor:" << std::endl;
    std::cout << "  - Class will be inherited" << std::endl;
    std::cout << "  - Objects deleted through base pointer" << std::endl;
    std::cout << "  - Class has virtual functions (is polymorphic)" << std::endl;

    std::cout << "\nDON'T need virtual destructor:" << std::endl;
    std::cout << "  - Class not designed for inheritance" << std::endl;
    std::cout << "  - Use 'final' to prevent inheritance" << std::endl;
    std::cout << "  - Small value types (Point, Vec3)" << std::endl;

    std::cout << "\nGuideline:" << std::endl;
    std::cout << "  If class has ANY virtual function," << std::endl;
    std::cout << "  it probably needs a virtual destructor" << std::endl;
}

// Cost of virtual destructors
void VirtualDestructorCost() {
    std::cout << "\n=== Cost of Virtual Destructors ===" << std::endl;

    std::cout << "Virtual destructor adds:" << std::endl;
    std::cout << "  + Entry in vtable" << std::endl;
    std::cout << "  + Virtual call overhead (indirect)" << std::endl;
    std::cout << "  + Prevents some optimizations" << std::endl;

    std::cout << "\nFor small classes:" << std::endl;
    std::cout << "  Base without virtual: " << sizeof(BadBase) << " bytes" << std::endl;
    std::cout << "  Base with virtual:    " << sizeof(GoodBase) << " bytes" << std::endl;
    std::cout << "  (difference is vtable pointer)" << std::endl;

    std::cout << "\nBest practice:" << std::endl;
    std::cout << "  Accept the cost when needed" << std::endl;
    std::cout << "  Don't make classes polymorphic 'just in case'" << std::endl;
}