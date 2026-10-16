import std;
#include "smart_pointers_header.h"

class SharedObject {
public:
    explicit SharedObject(std::string name) : name_(std::move(name)) {
        std::cout << "  SharedObject('" << name_ << "') constructed" << std::endl;
    }

    ~SharedObject() {
        std::cout << "  SharedObject('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void MakeSharedBasics() {
    std::cout << "=== make_shared ===" << std::endl;

    std::cout << "make_shared advantages over new:" << std::endl;
    std::cout << "  1. Single allocation" << std::endl;
    std::cout << "  2. Better cache locality" << std::endl;
    std::cout << "  3. Exception safe" << std::endl;
    std::cout << "  4. More concise" << std::endl;

    std::cout << "\nComparison:" << std::endl;
    std::cout << "  std::shared_ptr<T> a(new T(args));  // 2 allocations" << std::endl;
    std::cout << "  auto b = std::make_shared<T>(args);  // 1 allocation" << std::endl;

    std::cout << "\nUsing make_shared:" << std::endl;
    auto obj = std::make_shared<SharedObject>("efficient");
    std::cout << "  Created: " << obj->name() << std::endl;
}

void MakeSharedAllocation() {
    std::cout << "\n=== Allocation Details ===" << std::endl;

    std::cout << "With new:" << std::endl;
    std::cout << "  [Object allocation] ← T* ptr" << std::endl;
    std::cout << "  [Control block]      ← separate allocation" << std::endl;
    std::cout << "  shared_ptr stores both pointers" << std::endl;

    std::cout << "\nWith make_shared:" << std::endl;
    std::cout << "  [Object + Control block] ← single allocation" << std::endl;
    std::cout << "  shared_ptr points into same block" << std::endl;

    std::cout << "\nMemory layout (make_shared):" << std::endl;
    std::cout << "  [strong count | weak count | deleter | allocator | OBJECT]" << std::endl;
    std::cout << "  All in one contiguous block" << std::endl;
}

void MakeSharedLifetime() {
    std::cout << "\n=== Lifetime Consideration ===" << std::endl;

    std::cout << "Caveat with make_shared:" << std::endl;
    std::cout << "  Memory for OBJECT not freed until ALL weak_ptrs gone" << std::endl;
    std::cout << std::endl;
    std::cout << "  Even when shared count reaches 0:" << std::endl;
    std::cout << "  - Object destructor runs" << std::endl;
    std::cout << "  - But memory stays (for control block)" << std::endl;

    std::cout << "\nDemonstration:" << std::endl;
    std::weak_ptr<SharedObject> weak;

    {
        auto strong = std::make_shared<SharedObject>("sticky");
        weak = strong;

        std::cout << "  strong exists, weak valid: " << !weak.expired() << std::endl;
    }

    std::cout << "  strong gone, weak expired: " << weak.expired() << std::endl;
    std::cout << "  But MEMORY still allocated (for weak count)" << std::endl;
    std::cout << "  Only object destructor ran" << std::endl;
}

void MakeSharedWithArgs() {
    std::cout << "\n=== make_shared with Arguments ===" << std::endl;

    class Config {
    public:
        Config(std::string host, int port, bool debug)
            : host_(std::move(host)), port_(port), debug_(debug) {
            std::cout << "  Config(" << host_ << ":" << port_
                << ", debug=" << debug_ << ")" << std::endl;
        }

    private:
        std::string host_;
        int port_;
        bool debug_;
    };

    // Perfect forwarding
    auto config = std::make_shared<Config>("localhost", 8080, true);
}

void MakeSharedArrays() {
    std::cout << "\n=== make_shared for Arrays ===" << std::endl;

    std::cout << "NOTE: make_shared does NOT support arrays!" << std::endl;
    std::cout << "  auto arr = std::make_shared<int[]>(10);  // ERROR (pre-C++20)" << std::endl;

    std::cout << "\nAlternatives:" << std::endl;
    std::cout << "  1. std::vector<T>" << std::endl;
    std::cout << "  2. unique_ptr<T[]> with make_unique" << std::endl;

    // C++20: shared_ptr supports arrays
    std::cout << "\nC++20: shared_ptr for arrays" << std::endl;
    // auto arr = std::make_shared<int[]>(10);  // C++20
}

void MakeSharedBestPractices() {
    std::cout << "\n=== Best Practices ===" << std::endl;

    std::cout << "ALWAYS use make_shared when possible:" << std::endl;
    std::cout << "  auto p = std::make_shared<T>(args...);" << std::endl;

    std::cout << "\nAvoid:" << std::endl;
    std::cout << "  std::shared_ptr<T> p(new T(args...));" << std::endl;

    std::cout << "\nOnly use new when:" << std::endl;
    std::cout << "  Custom deleter needed" << std::endl;
    std::cout << "  Aliasing constructor" << std::endl;
    std::cout << "  Custom allocator required" << std::endl;
}