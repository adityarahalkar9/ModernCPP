import std;
#include "class_header.h"

// Demonstrating value categories in OOP context
class ValueCategory {
public:
    // Overload for lvalue reference
    void process(int& x) {
        std::cout << "  process(int&): lvalue - " << x << std::endl;
    }

    // Overload for rvalue reference
    void process(int&& x) {
        std::cout << "  process(int&&): rvalue - " << x << std::endl;
    }

    // Overload for const lvalue reference (catch-all)
    void process(const int& x) {
        std::cout << "  process(const int&): const lvalue - " << x << std::endl;
    }
};

void BasicValueCategories() {
    std::cout << "=== Value Categories ===" << std::endl;

    ValueCategory vc;

    int a = 42;           // 'a' is lvalue
    const int b = 10;     // 'b' is const lvalue

    vc.process(a);        // Calls process(int&)
    vc.process(b);        // Calls process(const int&)
    vc.process(100);      // Calls process(int&&) - temporary is rvalue
    vc.process(std::move(a));  // Calls process(int&&) - a treated as rvalue

    std::cout << "\nValue categories:" << std::endl;
    std::cout << "  lvalue: has identity, can't be moved from" << std::endl;
    std::cout << "  rvalue: can be moved from (temporaries)" << std::endl;
    std::cout << "  xvalue: expiring value (std::move result)" << std::endl;
    std::cout << "  prvalue: pure rvalue (literals, temporaries)" << std::endl;
}

// Member functions on rvalues
class Temporary {
public:
    void modify()& {          // Only on lvalues
        std::cout << "  modify() on lvalue" << std::endl;
    }

    void modify()&& {         // Only on rvalues
        std::cout << "  modify() on rvalue" << std::endl;
    }

    void read() const& {      // On lvalues and const
        std::cout << "  read() on lvalue/const" << std::endl;
    }

    void read() const&& {     // On rvalues
        std::cout << "  read() on rvalue" << std::endl;
    }
};

void MemberFunctionsOnRValues() {
    std::cout << "\n=== Member Functions on R-Values ===" << std::endl;

    Temporary temp;

    std::cout << "On named object (lvalue):" << std::endl;
    temp.modify();  // Calls modify() &
    temp.read();    // Calls read() const &

    std::cout << "\nOn temporary (rvalue):" << std::endl;
    Temporary().modify();  // Calls modify() &&
    Temporary().read();    // Calls read() const &&

    std::cout << "\nRef-qualifiers:" << std::endl;
    std::cout << "  & : this is lvalue reference" << std::endl;
    std::cout << "  && : this is rvalue reference" << std::endl;
}

// Move-aware classes
class MoveAware {
    std::string data;

public:
    MoveAware(std::string d) : data(std::move(d)) {}

    // Different behavior for lvalue/rvalue this
    void process()& {
        // 'this' is lvalue - object persists
        std::cout << "  Processing lvalue: " << data << std::endl;
    }

    void process()&& {
        // 'this' is rvalue - object about to die
        // Can safely cannibalize resources
        std::cout << "  Processing rvalue (can steal): " << data << std::endl;
        data.clear();  // Safe to modify aggressively
    }
};

void MoveAwareClass() {
    std::cout << "\n=== Move-Aware Classes ===" << std::endl;

    MoveAware obj("persistent");
    obj.process();  // lvalue version

    MoveAware("temporary").process();  // rvalue version

    std::cout << "\nCan have different behavior for:" << std::endl;
    std::cout << "  Objects that persist (lvalue)" << std::endl;
    std::cout << "  Objects about to die (rvalue)" << std::endl;
}

// Returning values
class ReturnDemo {
public:
    // Return by value (may use RVO)
    ReturnDemo create() {
        return ReturnDemo{};  // NRVO
    }

    // Return by reference (dangerous if local)
    // ReturnDemo& bad_create() {
    //     ReturnDemo local;
    //     return local;  // Dangling reference!
    // }

    ReturnDemo() { std::cout << "  Constructed" << std::endl; }
    ReturnDemo(const ReturnDemo&) { std::cout << "  Copy constructed" << std::endl; }
    ReturnDemo(ReturnDemo&&) noexcept { std::cout << "  Move constructed" << std::endl; }
};

void ReturningValues() {
    std::cout << "\n=== Returning Values ===" << std::endl;

    std::cout << "Return by value is safe:" << std::endl;
    std::cout << "  RVO eliminates copies" << std::endl;
    std::cout << "  Move semantics as fallback" << std::endl;

    ReturnDemo rd;
    auto created = rd.create();
}

// Perfect forwarding context
template <typename T>
void universal_reference(T&& x) {
    // T&& here is universal/forwarding reference
    // Binds to both lvalues and rvalues

    if constexpr(std::is_lvalue_reference_v<T>) {
        std::cout << "  Universal ref: lvalue" << std::endl;
    }
    else {
        std::cout << "  Universal ref: rvalue" << std::endl;
    }
}

void PerfectForwardingBasics() {
    std::cout << "\n=== Universal References ===" << std::endl;

    int x = 42;
    universal_reference(x);           // T deduced as int&
    universal_reference(100);         // T deduced as int
    universal_reference(std::move(x)); // T deduced as int

    std::cout << "\nT&& is universal when:" << std::endl;
    std::cout << "  T is template parameter" << std::endl;
    std::cout << "  Being deduced" << std::endl;
    std::cout << "  (not for specific type like int&&)" << std::endl;
}