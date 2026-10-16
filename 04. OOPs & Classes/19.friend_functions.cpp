import std;
#include "class_header.h"

// Friend functions
class Vector2 {
private:
    double x, y;

public:
    Vector2(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    // Friend function declaration
    friend double dot(const Vector2& a, const Vector2& b);
    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);

    double get_x() const { return x; }
    double get_y() const { return y; }
};

// Friend function definitions (have access to private members)
double dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;  // Accessing private members!
}

Vector2 operator+(const Vector2& a, const Vector2& b) {
    return Vector2(a.x + b.x, a.y + b.y);  // Accessing private members!
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    return os << "(" << v.x << ", " << v.y << ")";  // Accessing private members!
}

void FriendFunctions() {
    std::cout << "=== Friend Functions ===" << std::endl;

    Vector2 v1(3.0, 4.0);
    Vector2 v2(1.0, 2.0);

    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;
    std::cout << "v1 + v2 = " << (v1 + v2) << std::endl;
    std::cout << "dot(v1, v2) = " << dot(v1, v2) << std::endl;

    std::cout << "\nFriend functions:" << std::endl;
    std::cout << "  - Access private members" << std::endl;
    std::cout << "  - Not member functions" << std::endl;
    std::cout << "  - Not in class scope" << std::endl;
}

// Friend classes
class Storage {
private:
    std::vector<int> data;
    bool is_dirty = false;

    // Grant StorageManager access to everything
    friend class StorageManager;

public:
    void add(int value) {
        data.push_back(value);
        is_dirty = true;
    }
};

class StorageManager {
public:
    void optimize(Storage& s) {
        // Can access Storage's private members
        std::sort(s.data.begin(), s.data.end());
        s.is_dirty = false;
        std::cout << "  Optimized " << s.data.size() << " elements" << std::endl;
    }

    int sum(const Storage& s) {
        // Can access private data
        int total = 0;
        for(int v : s.data) total += v;
        return total;
    }

    bool needs_optimization(const Storage& s) {
        return s.is_dirty;
    }
};

void FriendClasses() {
    std::cout << "\n=== Friend Classes ===" << std::endl;

    Storage storage;
    storage.add(5);
    storage.add(3);
    storage.add(8);

    StorageManager manager;

    std::cout << "Needs optimization: " << manager.needs_optimization(storage) << std::endl;
    manager.optimize(storage);
    std::cout << "Sum: " << manager.sum(storage) << std::endl;

    std::cout << "\nFriend class:" << std::endl;
    std::cout << "  - Access to ALL private members" << std::endl;
    std::cout << "  - Friendship is granted, not taken" << std::endl;
    std::cout << "  - Not symmetric, not inherited" << std::endl;
}

// Friend member functions
class Printer;  // Forward declaration

class Document2 {
private:
    std::string content = "Secret document content";

    // Grant specific member function access
    friend void Printer::print_document(const Document2& d);
    friend void Printer::print_summary(const Document2& d);
};

class Printer {
public:
    void print_document(const Document2& d) {
        // Can access private members (declared as friend)
        std::cout << "  Full: " << d.content << std::endl;
    }

    void print_summary(const Document2& d) {
        std::cout << "  Summary: " << d.content.substr(0, 10) << "..." << std::endl;
    }

    void unauthorized_access(const Document2& d) {
        // std::cout << d.content;  // ERROR: not a friend
        std::cout << "  Cannot access private members" << std::endl;
    }
};

void FriendMemberFunctions() {
    std::cout << "\n=== Friend Member Functions ===" << std::endl;

    std::cout << "Grant access to specific member functions:" << std::endl;
    std::cout << "  friend void Printer::print_document(const Document2&);" << std::endl;

    std::cout << "\nMore precise than friending entire class" << std::endl;
}

// Operator overloading with friends
class Money {
private:
    long long cents;

public:
    explicit Money(long long c) : cents(c) {}

    // Binary operators as friends
    friend Money operator+(const Money& a, const Money& b);
    friend Money operator-(const Money& a, const Money& b);
    friend bool operator<(const Money& a, const Money& b);
    friend bool operator==(const Money& a, const Money& b);
    friend std::ostream& operator<<(std::ostream& os, const Money& m);

    long long get_cents() const { return cents; }
};

Money operator+(const Money& a, const Money& b) {
    return Money(a.cents + b.cents);
}

Money operator-(const Money& a, const Money& b) {
    return Money(a.cents - b.cents);
}

bool operator<(const Money& a, const Money& b) {
    return a.cents < b.cents;
}

bool operator==(const Money& a, const Money& b) {
    return a.cents == b.cents;
}

std::ostream& operator<<(std::ostream& os, const Money& m) {
    return os << "$" << m.cents / 100 << "." << std::setfill('0')
        << std::setw(2) << m.cents % 100;
}

void OperatorOverloadingFriends() {
    std::cout << "\n=== Operator Overloading with Friends ===" << std::endl;

    Money price(1050);  // $10.50
    Money tax(85);       // $0.85

    Money total = price + tax;
    std::cout << "Price: " << price << std::endl;
    std::cout << "Tax:   " << tax << std::endl;
    std::cout << "Total: " << total << std::endl;

    std::cout << "\nFriends are common for:" << std::endl;
    std::cout << "  - operator<<" << std::endl;
    std::cout << "  - Symmetric operators" << std::endl;
    std::cout << "  - Accessing private state" << std::endl;
}

// Friend for testing
class SecretClass {
private:
    int secret_value = 42;

public:
    int public_method() { return secret_value * 2; }

    // Grant test class access
    friend class SecretTest;
};

class SecretTest {
public:
    void test_getter(SecretClass& s) {
        // Can verify internal state in tests
        assert(s.secret_value == 42);
        std::cout << "  Test passed: internal state correct" << std::endl;
    }
};

void FriendForTesting() {
    std::cout << "\n=== Friend for Testing ===" << std::endl;

    SecretClass secret;
    SecretTest test;
    test.test_getter(secret);

    std::cout << "\nTesting use case:" << std::endl;
    std::cout << "  Tests need access to internals" << std::endl;
    std::cout << "  Friend test classes" << std::endl;
    std::cout << "  But don't expose to users" << std::endl;
}

// Friendship rules
void FriendshipRules() {
    std::cout << "\n=== Friendship Rules ===" << std::endl;

    std::cout << "Friendship is:" << std::endl;
    std::cout << "  GRANTED, not taken" << std::endl;
    std::cout << "  Class gives friendship to another" << std::endl;

    std::cout << "\nFriendship is NOT:" << std::endl;
    std::cout << "  SYMMETRIC: A friends B ≠ B friends A" << std::endl;
    std::cout << "  TRANSITIVE: A friends B, B friends C ≠ A friends C" << std::endl;
    std::cout << "  INHERITED: Derived doesn't get Base's friends" << std::endl;

    std::cout << "\nUse friendship sparingly:" << std::endl;
    std::cout << "  It breaks encapsulation" << std::endl;
    std::cout << "  Increases coupling" << std::endl;
    std::cout << "  Makes refactoring harder" << std::endl;
}