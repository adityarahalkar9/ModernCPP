import std;
#include "class_header.h"

// Vtable demonstration
class Polymorphic {
public:
    virtual void method1() { std::cout << "  method1" << std::endl; }
    virtual void method2() { std::cout << "  method2" << std::endl; }
    virtual ~Polymorphic() = default;

    int data = 42;
};

class Derived2 : public Polymorphic {
public:
    void method1() override { std::cout << "  Derived2::method1" << std::endl; }
    // method2 not overridden

    int extra = 100;
};

void VtableDemo() {
    std::cout << "=== Object Model ===" << std::endl;

    std::cout << "Memory layout with virtual functions:" << std::endl;
    std::cout << "  [vptr] -> vtable -> function pointers" << std::endl;
    std::cout << "  [data members]" << std::endl;

    std::cout << "\nsizeof(Polymorphic) = " << sizeof(Polymorphic) << std::endl;
    std::cout << "(vptr + int + padding)" << std::endl;

    std::cout << "\nsizeof(Derived2) = " << sizeof(Derived2) << std::endl;
    std::cout << "(vptr + Polymorphic's data + Derived's data)" << std::endl;

    // Virtual call
    Polymorphic* p = new Derived2();
    p->method1();  // Goes through vtable
    p->method2();  // Uses Polymorphic::method2
    delete p;
}

// Object layout inspection
class Layout1 {
    int a;
    double b;
public:
    Layout1() : a(1), b(2.0) {}
};

class Layout2 : public Layout1 {
    char c;
public:
    Layout2() { c = 'x'; }
};

class VirtualLayout : virtual public Layout1 {
    int d;
public:
    VirtualLayout() : d(3) {}
};

void LayoutInspection() {
    std::cout << "\n=== Object Layout ===" << std::endl;

    std::cout << "Simple class:" << std::endl;
    std::cout << "  sizeof(Layout1) = " << sizeof(Layout1) << std::endl;

    std::cout << "\nDerived class:" << std::endl;
    std::cout << "  sizeof(Layout2) = " << sizeof(Layout2) << std::endl;

    std::cout << "\nVirtual inheritance:" << std::endl;
    std::cout << "  sizeof(VirtualLayout) = " << sizeof(VirtualLayout) << std::endl;
    std::cout << "  (extra vbptr for virtual base)" << std::endl;
}

// Construction and destruction order
class OrderTracker {
public:
    OrderTracker(std::string name) : name_(std::move(name)) {
        std::cout << "  " << name_ << " constructed" << std::endl;
    }

    ~OrderTracker() {
        std::cout << "  " << name_ << " destroyed" << std::endl;
    }

private:
    std::string name_;
};

class Complex {
    OrderTracker member1{"member1"};
    OrderTracker member2{"member2"};

public:
    Complex() : OrderTracker_base() {
        std::cout << "  Complex constructor body" << std::endl;
    }

    ~Complex() {
        std::cout << "  Complex destructor body" << std::endl;
    }

    class OrderTracker_base : public OrderTracker {
    public:
        OrderTracker_base() : OrderTracker("base") {}
    } base_;
};

void ConstructionOrder() {
    std::cout << "\n=== Construction Order ===" << std::endl;

    std::cout << "Order of construction:" << std::endl;
    std::cout << "  1. Virtual bases (most base first)" << std::endl;
    std::cout << "  2. Non-virtual bases (declaration order)" << std::endl;
    std::cout << "  3. Members (declaration order)" << std::endl;
    std::cout << "  4. Constructor body" << std::endl;

    std::cout << "\nDestruction is REVERSE order:" << std::endl;

    Complex c;
}

// Multiple vtables
class MI1 {
public:
    virtual void mi1_method() { std::cout << "  MI1 method" << std::endl; }
    virtual ~MI1() = default;
    int mi1_data = 1;
};

class MI2 {
public:
    virtual void mi2_method() { std::cout << "  MI2 method" << std::endl; }
    virtual ~MI2() = default;
    int mi2_data = 2;
};

class MultiDerived : public MI1, public MI2 {
public:
    void mi1_method() override { std::cout << "  MultiDerived MI1" << std::endl; }
    void mi2_method() override { std::cout << "  MultiDerived MI2" << std::endl; }
};

void MultipleVtables() {
    std::cout << "\n=== Multiple Inheritance Layout ===" << std::endl;

    std::cout << "Multiple virtual bases = multiple vtables:" << std::endl;
    std::cout << "  sizeof(MI1) = " << sizeof(MI1) << std::endl;
    std::cout << "  sizeof(MI2) = " << sizeof(MI2) << std::endl;
    std::cout << "  sizeof(MultiDerived) = " << sizeof(MultiDerived) << std::endl;

    std::cout << "\nObject contains:" << std::endl;
    std::cout << "  [vtable1][MI1 data][vtable2][MI2 data]" << std::endl;

    MultiDerived md;
    md.mi1_method();
    md.mi2_method();
}

// Empty base optimization
class Empty2 {};
class WithEmpty2 : public Empty2 {
    int data;
};

void EmptyBaseOpt() {
    std::cout << "\n=== Empty Base Optimization ===" << std::endl;

    std::cout << "Empty classes can take no space:" << std::endl;
    std::cout << "  sizeof(Empty2) = " << sizeof(Empty2) << " (must be ≥ 1)" << std::endl;
    std::cout << "  sizeof(WithEmpty2) = " << sizeof(WithEmpty2) << " (EBO applied)" << std::endl;
}