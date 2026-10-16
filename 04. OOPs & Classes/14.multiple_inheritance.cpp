import std;
#include "class_header.h"

// Multiple inheritance basics
class Device {
protected:
    std::string name;
    bool powered_on = false;

public:
    Device(std::string n) : name(std::move(n)) {}

    void power_on() {
        powered_on = true;
        std::cout << "  " << name << " powered on" << std::endl;
    }

    void power_off() {
        powered_on = false;
        std::cout << "  " << name << " powered off" << std::endl;
    }

    const std::string& get_name() const { return name; }
    bool is_on() const { return powered_on; }

    virtual ~Device() = default;
};

class Networkable {
protected:
    std::string ip_address;
    bool connected = false;

public:
    void connect(const std::string& ip) {
        ip_address = ip;
        connected = true;
        std::cout << "  Connected to " << ip << std::endl;
    }

    void disconnect() {
        connected = false;
        std::cout << "  Disconnected" << std::endl;
    }

    bool is_connected() const { return connected; }
    const std::string& get_ip() const { return ip_address; }

    virtual ~Networkable() = default;
};

class SmartTV : public Device, public Networkable {
public:
    SmartTV() : Device("SmartTV") {}

    void stream_video(const std::string& content) {
        if(is_on() && is_connected()) {
            std::cout << "  Streaming: " << content << std::endl;
        }
        else {
            std::cout << "  Cannot stream - check power/connection" << std::endl;
        }
    }
};

void MultipleInheritanceBasics() {
    std::cout << "=== Multiple Inheritance ===" << std::endl;

    SmartTV tv;

    tv.power_on();
    tv.connect("192.168.1.100");
    tv.stream_video("Movie");

    std::cout << "\nSmartTV inherits from:" << std::endl;
    std::cout << "  Device: " << tv.get_name() << std::endl;
    std::cout << "  Networkable: " << tv.get_ip() << std::endl;

    std::cout << "\nMultiple inheritance combines behaviors" << std::endl;
}

// The diamond problem
class A {
public:
    void method_A() { std::cout << "  A::method_A" << std::endl; }
    int data = 1;

    virtual ~A() = default;
};

class B : public A {
public:
    void method_B() { std::cout << "  B::method_B" << std::endl; }
    int data_B = 2;
};

class C : public A {
public:
    void method_C() { std::cout << "  C::method_C" << std::endl; }
    int data_C = 3;
};

// D inherits from both B and C (which both inherit from A)
class D : public B, public C {
public:
    void method_D() { std::cout << "  D::method_D" << std::endl; }
};

void DiamondProblem() {
    std::cout << "\n=== Diamond Problem ===" << std::endl;

    std::cout << "Inheritance diagram:" << std::endl;
    std::cout << "      A" << std::endl;
    std::cout << "     / \\" << std::endl;
    std::cout << "    B   C" << std::endl;
    std::cout << "     \\ /" << std::endl;
    std::cout << "      D" << std::endl;

    D d;
    d.method_B();
    d.method_C();
    d.method_D();

    // AMBIGUITY: Which 'data'?
    // std::cout << d.data;  // ERROR: ambiguous

    std::cout << "\nMust disambiguate:" << std::endl;
    std::cout << "  B::data: " << d.B::data << std::endl;  // A's data via B
    std::cout << "  C::data: " << d.C::data << std::endl;  // A's data via C

    std::cout << "\nPROBLEM: TWO copies of A's data!" << std::endl;

    std::cout << "\nMemory layout:" << std::endl;
    std::cout << "  sizeof(D) includes A TWICE" << std::endl;
    std::cout << "  B's A::data at: " << &d.B::data << std::endl;
    std::cout << "  C's A::data at: " << &d.C::data << std::endl;
}

// Multiple inheritance with virtual functions
class Printable2 {
public:
    virtual void print() const = 0;
    virtual ~Printable2() = default;
};

class Saveable {
public:
    virtual void save() const = 0;
    virtual ~Saveable() = default;
};

class Report : public Printable2, public Saveable {
    std::string content;

public:
    Report(std::string c) : content(std::move(c)) {}

    void print() const override {
        std::cout << "  Report: " << content << std::endl;
    }

    void save() const override {
        std::cout << "  Saving report to disk" << std::endl;
    }
};

void VirtualFunctionMI() {
    std::cout << "\n=== Virtual Functions with Multiple Inheritance ===" << std::endl;

    Report report("Quarterly Results");

    // Polymorphic through each interface
    Printable2* p = &report;
    p->print();

    Saveable* s = &report;
    s->save();

    std::cout << "\nEach base class has its own vtable" << std::endl;
    std::cout << "Object layout contains multiple vtable pointers" << std::endl;
}

// Ambiguity resolution
class First {
public:
    void method() { std::cout << "  First::method" << std::endl; }
};

class Second {
public:
    void method() { std::cout << "  Second::method" << std::endl; }
};

class Both : public First, public Second {
public:
    // Resolve ambiguity by defining own version
    void method() {
        std::cout << "  Both::method (resolving ambiguity)" << std::endl;
    }

    // Or expose specific versions
    void first_method() { First::method(); }
    void second_method() { Second::method(); }
};

void AmbiguityResolution() {
    std::cout << "\n=== Ambiguity Resolution ===" << std::endl;

    Both b;
    b.method();  // Uses Both::method

    std::cout << "\nOr call specific versions:" << std::endl;
    b.first_method();   // First::method
    b.second_method();  // Second::method

    std::cout << "\nDirect qualification:" << std::endl;
    b.First::method();
    b.Second::method();
}

// Multiple interface inheritance (recommended)
class Runnable {
public:
    virtual void run() = 0;
    virtual ~Runnable() = default;
};

class Stoppable {
public:
    virtual void stop() = 0;
    virtual ~Stoppable() = default;
};

class Service : public Runnable, public Stoppable {
public:
    void run() override { std::cout << "  Service running" << std::endl; }
    void stop() override { std::cout << "  Service stopped" << std::endl; }
};

void MultipleInterfaces() {
    std::cout << "\n=== Multiple Interface Inheritance ===" << std::endl;

    std::cout << "BEST PRACTICE:" << std::endl;
    std::cout << "  Multiple inheritance from pure interfaces" << std::endl;
    std::cout << "  Single inheritance from concrete classes" << std::endl;

    Service service;
    service.run();
    service.stop();

    std::cout << "\nThis avoids:" << std::endl;
    std::cout << "  - Diamond problem" << std::endl;
    std::cout << "  - Ambiguity" << std::endl;
    std::cout << "  - Complex layouts" << std::endl;
}