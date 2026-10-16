import std;
#include "class_header.h"

// Basic delegating constructors
class Rectangle {
private:
    double width, height;
    std::string label;

public:
    // Main constructor does the work
    Rectangle(double w, double h, std::string l)
        : width(w), height(h), label(std::move(l)) {
        std::cout << "  Main constructor: " << this->label << std::endl;
        validate();
    }

    // Delegating constructors
    Rectangle(double w, double h)
        : Rectangle(w, h, "unnamed") {
        std::cout << "  Delegated (2 args)" << std::endl;
    }

    Rectangle(double side)
        : Rectangle(side, side, "square") {
        std::cout << "  Delegated (square)" << std::endl;
    }

    Rectangle()
        : Rectangle(1.0, 1.0, "default") {
        std::cout << "  Delegated (default)" << std::endl;
    }

    void validate() {
        if(width < 0 || height < 0) {
            throw std::invalid_argument("Negative dimensions");
        }
    }

    double area() const { return width * height; }
    const std::string& get_label() const { return label; }
};

void DelegatingBasics() {
    std::cout << "=== Delegating Constructors ===" << std::endl;

    std::cout << "Full construction:" << std::endl;
    Rectangle r1(3.0, 4.0, "my_rect");

    std::cout << "\nDelegating construction:" << std::endl;
    Rectangle r2(5.0, 6.0);  // Delegates to 3-arg version
    Rectangle r3(2.0);       // Delegates through chain
    Rectangle r4;            // Delegates through chain

    std::cout << "\nAll constructed:" << std::endl;
    std::cout << "  r1: " << r1.get_label() << " area=" << r1.area() << std::endl;
    std::cout << "  r2: " << r2.get_label() << " area=" << r2.area() << std::endl;
    std::cout << "  r3: " << r3.get_label() << " area=" << r3.area() << std::endl;
}

// Avoiding duplication
class BadVersion {
    // WITHOUT delegation: code duplication
public:
    BadVersion(int x) {
        // Common init code
        std::cout << "  Common initialization" << std::endl;
        std::cout << "  Specific for int" << std::endl;
    }

    BadVersion(double x) {
        // Same common init code duplicated
        std::cout << "  Common initialization" << std::endl;
        std::cout << "  Specific for double" << std::endl;
    }
};

class GoodVersion {
public:
    GoodVersion() {
        // Common initialization once
        std::cout << "  Common initialization" << std::endl;
    }

    GoodVersion(int x) : GoodVersion() {
        std::cout << "  Specific for int" << std::endl;
    }

    GoodVersion(double x) : GoodVersion() {
        std::cout << "  Specific for double" << std::endl;
    }
};

void AvoidingDuplication() {
    std::cout << "\n=== Avoiding Duplication ===" << std::endl;

    std::cout << "Without delegation (duplicate code):" << std::endl;
    BadVersion bad1(42);
    BadVersion bad2(3.14);

    std::cout << "\nWith delegation (no duplication):" << std::endl;
    GoodVersion good1(42);
    GoodVersion good2(3.14);
}

// Chaining constructors
class Config {
    std::string host = "localhost";
    int port = 80;
    bool use_ssl = false;
    std::string path = "/";

public:
    // Most specific
    Config(std::string h, int p, bool ssl, std::string path_)
        : host(std::move(h)), port(p), use_ssl(ssl), path(std::move(path_)) {
        std::cout << "  Full config constructed" << std::endl;
    }

    // Chain to more specific
    Config(std::string h, int p, bool ssl)
        : Config(std::move(h), p, ssl, "/") {
        std::cout << "  3-arg delegates to 4-arg" << std::endl;
    }

    Config(std::string h, int p)
        : Config(std::move(h), p, false) {
        std::cout << "  2-arg delegates to 3-arg" << std::endl;
    }

    Config(std::string h)
        : Config(std::move(h), 80) {
        std::cout << "  1-arg delegates to 2-arg" << std::endl;
    }

    Config()
        : Config("localhost") {
        std::cout << "  Default delegates to 1-arg" << std::endl;
    }

    void print() const {
        std::cout << "  " << host << ":" << port << (use_ssl ? " (SSL)" : "") << path << std::endl;
    }
};

void ChainingConstructors() {
    std::cout << "\n=== Constructor Chaining ===" << std::endl;

    Config c1;
    Config c2("example.com");
    Config c3("example.com", 443);
    Config c4("example.com", 443, true);
    Config c5("example.com", 443, true, "/api");

    std::cout << "\nAll configurations:" << std::endl;
    c1.print();
    c2.print();
    c3.print();
    c4.print();
    c5.print();
}

// Init-delegation vs member-init
class Complex {
    int a = 0;
    int b = 0;

public:
    // Member initializer (C++11)
    Complex() : a(10), b(20) {
        std::cout << "  Member init: a=" << a << " b=" << b << std::endl;
    }

    // Delegating to this
    Complex(int x) : Complex() {
        std::cout << "  Delegated with x=" << x << std::endl;
        a = x;  // After delegation, can modify
    }
};

void InitVsDelegation() {
    std::cout << "\n=== Init vs Delegation ===" std::endl;

    std::cout << "Member initialization:" << std::endl;
    std::cout << "  Complex c;  -> initializes a, b" << std::endl;

    std::cout << "\nDelegation:" << std::endl;
    std::cout << "  Complex c(5); -> runs default first, then modifies" << std::endl;

    Complex c1;
    Complex c2(5);
}