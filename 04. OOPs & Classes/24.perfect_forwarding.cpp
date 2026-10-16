import std;
#include "class_header.h"

// Perfect forwarding in constructors
class Resource {
public:
    Resource(int i, double d) {
        std::cout << "  Resource(" << i << ", " << d << ")" << std::endl;
    }

    Resource(std::string s) {
        std::cout << "  Resource(\"" << s << "\")" << std::endl;
    }
};

// Without perfect forwarding (tedious)
class WithoutForwarding {
    Resource res;
public:
    WithoutForwarding(int i, double d) : res(i, d) {
        std::cout << "  WithoutForwarding(int, double)" << std::endl;
    }

    WithoutForwarding(std::string s) : res(std::move(s)) {
        std::cout << "  WithoutForwarding(string)" << std::endl;
    }

    // Must write constructor for each combination!
};

void WithoutForwardingDemo() {
    std::cout << "=== Without Perfect Forwarding ===" << std::endl;

    std::cout << "Must write constructor for each type:" << std::endl;
    std::cout << "  Tedious" << std::endl;
    std::cout << "  Error-prone" << std::endl;
    std::cout << "  Hard to maintain" << std::endl;

    WithoutForwarding w1(42, 3.14);
    WithoutForwarding w2("hello");
}

// With perfect forwarding (elegant)
class WithForwarding {
    Resource res;

public:
    template <typename... Args>
    explicit WithForwarding(Args&&... args)
        : res(std::forward<Args>(args)...) {
        std::cout << "  WithForwarding(forwarded " << sizeof...(args) << " args)" << std::endl;
    }
};

void WithForwardingDemo() {
    std::cout << "\n=== With Perfect Forwarding ===" << std::endl;

    std::cout << "One constructor handles all cases:" << std::endl;

    WithForwarding w1(42, 3.14);
    WithForwarding w2("hello");
    WithForwarding w3(std::string("temporary string"));
}

// Forwarding to member functions
class Logger {
public:
    template <typename T>
    void log(T&& value) {
        std::cout << "  Log: " << value << std::endl;
    }

    template <typename... Args>
    void log_formatted(const std::string& format, Args&&... args) {
        std::cout << "  Formatted: " << format;
        if constexpr(sizeof...(args) > 0) {
            std::cout << " (" << sizeof...(args) << " args)";
        }
        std::cout << std::endl;
    }
};

void ForwardingToMembers() {
    std::cout << "\n=== Forwarding to Member Functions ===" << std::endl;

    Logger logger;
    logger.log("simple message");
    logger.log(42);
    logger.log(3.14);

    logger.log_formatted("fmt");
    logger.log_formatted("fmt", 1, 2, 3);
}

// Make functions with perfect forwarding
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_advanced(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void MakeFunctions() {
    std::cout << "\n=== Make Functions ===" << std::endl;

    auto ptr = make_unique_advanced<Resource>(42, 3.14);
    auto ptr2 = make_unique_advanced<Resource>("string arg");

    std::cout << "\nstd::make_unique uses perfect forwarding" << std::endl;
}

// Emplace with perfect forwarding
class Container2 {
    std::vector<Resource> resources;

public:
    template <typename... Args>
    void emplace(Args&&... args) {
        resources.emplace_back(std::forward<Args>(args)...);
    }

    size_t size() const { return resources.size(); }
};

void EmplaceForwarding() {
    std::cout << "\n=== Emplace with Forwarding ===" << std::endl;

    Container2 c;
    c.emplace(42, 3.14);
    c.emplace("string");

    std::cout << "Container has " << c.size() << " resources" << std::endl;
}

// Why std::forward is needed
void demonstrate_forward(int& x) {
    std::cout << "  lvalue: " << x << std::endl;
}

void demonstrate_forward(int&& x) {
    std::cout << "  rvalue: " << x << std::endl;
}

template <typename T>
void without_forward(T&& x) {
    // x has a name, so it's lvalue here
    // Even if T is rvalue reference!
    demonstrate_forward(x);  // Always calls lvalue version
}

template <typename T>
void with_forward(T&& x) {
    demonstrate_forward(std::forward<T>(x));  // Preserves value category
}

void WhyForwardNeeded() {
    std::cout << "\n=== Why std::forward Needed ===" << std::endl;

    std::cout << "Inside function, named rvalue reference is lvalue:" << std::endl;
    std::cout << "  T&& x = ...;  // x is rvalue ref" << std::endl;
    std::cout << "  use(x);       // but x is lvalue here!" << std::endl;

    std::cout << "\nstd::forward restores original category:" << std::endl;

    int value = 42;
    std::cout << "Without forward:" << std::endl;
    without_forward(value);        // T=int&, x is lvalue
    without_forward(100);          // T=int, x acts as lvalue inside

    std::cout << "With forward:" << std::endl;
    with_forward(value);           // Forwards as lvalue
    with_forward(100);             // Forwards as rvalue
}