import std;
#include "smart_pointers_header.h"

class Tracked2 {
public:
    explicit Tracked2(int value) : value_(value) {
        std::cout << "  Tracked2(" << value << ") constructed" << std::endl;
    }

    ~Tracked2() {
        std::cout << "  Tracked2(" << value_ << ") destroyed" << std::endl;
    }

    int value() const { return value_; }

private:
    int value_;
};

void MakeUniqueBasics() {
    std::cout << "=== make_unique ===" << std::endl;

    std::cout << "make_unique (C++14):" << std::endl;
    std::cout << "  Safer than direct new" << std::endl;
    std::cout << "  More concise" << std::endl;
    std::cout << "  Exception safe" << std::endl;

    std::cout << "\nOld way:" << std::endl;
    std::unique_ptr<Tracked2> old(new Tracked2(42));

    std::cout << "\nNew way:" << std::endl;
    auto modern = std::make_unique<Tracked2>(42);

    std::cout << "\nBoth result in same thing:" << std::endl;
    std::cout << "  old->value(): " << old->value() << std::endl;
    std::cout << "  modern->value(): " << modern->value() << std::endl;
}

void MakeUniqueVariants() {
    std::cout << "\n=== make_unique Variants ===" << std::endl;

    // Single object with args
    auto single = std::make_unique<Tracked2>(100);

    // Array of default-constructed
    auto array = std::make_unique<Tracked2[]>(3);
    // Note: can't initialize array elements directly

    std::cout << "  Single: " << single->value() << std::endl;
    std::cout << "  Array size: 3 (default constructed)" << std::endl;
}

void MakeUniqueExceptionSafety() {
    std::cout << "\n=== Exception Safety ===" << std::endl;

    std::cout << "Problem with new:" << std::endl;
    std::cout << "  function(new T, new U);" << std::endl;
    std::cout << "  If first new succeeds, second throws:" << std::endl;
    std::cout << "  → First allocation LEAKS!" << std::endl;

    std::cout << "\nWith make_unique:" << std::cout;
    std::cout << "  function(make_unique<T>(), make_unique<U>());" << std::endl;
    std::cout << "  If second throws, first is already managed" << std::endl;
    std::cout << "  → No leak!" << std::endl;
}

void MakeUniqueWithCustomTypes() {
    std::cout << "\n=== make_unique with Custom Types ===" << std::endl;

    class Complex {
    public:
        Complex(int a, double b, std::string c)
            : a_(a), b_(b), c_(std::move(c)) {
            std::cout << "  Complex(" << a_ << ", " << b_ << ", \"" << c_ << "\")" << std::endl;
        }

        void describe() const {
            std::cout << "  a=" << a_ << " b=" << b_ << " c=" << c_ << std::endl;
        }

    private:
        int a_;
        double b_;
        std::string c_;
    };

    // Perfect forwarding to constructor
    auto complex = std::make_unique<Complex>(42, 3.14, "hello");
    complex->describe();
}

void MakeUniquePerformance() {
    std::cout << "\n=== Performance Note ===" << std::endl;

    std::cout << "make_unique has NO performance advantage over new" << std::endl;
    std::cout << "Unlike make_shared (which is faster)" << std::endl;

    std::cout << "\nWhy use it anyway:" << std::endl;
    std::cout << "  1. Consistency (make_shared exists)" << std::endl;
    std::cout << "  2. Exception safety" << std::endl;
    std::cout << "  3. No 'new' keyword visible" << std::endl;
    std::cout << "  4. Clearer intent" << std::endl;
}

void MakeUniqueGuidelines() {
    std::cout << "\n=== Guidelines ===" << std::endl;

    std::cout << "ALWAYS prefer make_unique:" << std::endl;
    std::cout << "  auto p = std::make_unique<T>(args...);" << std::endl;

    std::cout << "\nAvoid raw new:" << std::endl;
    std::cout << "  std::unique_ptr<T> p(new T(args...));  // Old style" << std::endl;

    std::cout << "\nWhen you MUST use new:" << std::endl;
    std::cout << "  Custom deleter required" << std::endl;
    std::cout << "  Custom allocator" << std::endl;
    std::cout << "  Interop with C APIs" << std::endl;
}