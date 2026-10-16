import std;
#include "smart_pointers_header.h"

class SharedResource {
public:
    explicit SharedResource(std::string name) : name_(std::move(name)) {
        std::cout << "  SharedResource('" << name_ << "') created" << std::endl;
    }

    ~SharedResource() {
        std::cout << "  SharedResource('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void SharedPtrCreation() {
    std::cout << "=== shared_ptr Creation ===" std::endl;

    // Method 1: Direct construction
    std::shared_ptr<SharedResource> p1(new SharedResource("direct"));

    // Method 2: make_shared (preferred)
    auto p2 = std::make_shared<SharedResource>("make_shared");

    std::cout << "\nReference counts:" << std::endl;
    std::cout << "  p1 count: " << p1.use_count() << std::endl;
    std::cout << "  p2 count: " << p2.use_count() << std::endl;
}

void SharedPtrCopying() {
    std::cout << "\n=== shared_ptr Copying ===" << std::endl;

    auto original = std::make_shared<SharedResource>("shared");

    std::cout << "Original count: " << original.use_count() << std::endl;

    // Copying increases reference count
    auto copy1 = original;
    std::cout << "After copy1: " << original.use_count() << std::endl;

    auto copy2 = original;
    auto copy3 = original;
    std::cout << "After 3 copies: " << original.use_count() << std::endl;

    // All point to same object
    std::cout << "All same object: "
        << (original.get() == copy1.get()) << std::endl;

    // Copies going out of scope decrease count
    {
        auto temp = original;
        std::cout << "With temp: " << original.use_count() << std::endl;
    }
    std::cout << "Temp gone: " << original.use_count() << std::endl;
}

void SharedPtrOwnership() {
    std::cout << "\n=== shared_ptr Ownership ===" << std::endl;

    std::cout << "Object destroyed when LAST owner goes away:" << std::endl;

    std::shared_ptr<SharedResource> outer;

    std::cout << "\nCreating inner scope:" << std::endl;
    {
        auto inner = std::make_shared<SharedResource>("scoped");
        outer = inner;  // Share ownership
        std::cout << "  Count in scope: " << outer.use_count() << std::endl;
    }

    std::cout << "Inner scope ended, but outer still owns:" << std::endl;
    std::cout << "  Count: " << outer.use_count() << std::endl;
    std::cout << "  Object: " << outer->name() << std::endl;

    std::cout << "\nOuter going away - object destroyed:" << std::endl;
}

void SharedPtrAssignment() {
    std::cout << "\n=== shared_ptr Assignment ===" << std::endl;

    auto p1 = std::make_shared<SharedResource>("first");
    auto p2 = std::make_shared<SharedResource>("second");

    std::cout << "Before: p1=" << p1->name() << ", p2=" << p2->name() << std::endl;
    std::cout << "  p1 count: " << p1.use_count() << std::endl;
    std::cout << "  p2 count: " << p2.use_count() << std::endl;

    // Assignment: p1 releases 'first', shares 'second'
    p1 = p2;

    std::cout << "\nAfter p1 = p2:" << std::endl;
    std::cout << "  p1 now: " << p1->name() << std::endl;
    std::cout << "  p1 count: " << p1.use_count() << std::endl;
    std::cout << "  p2 count: " << p2.use_count() << std::endl;
    // 'first' was destroyed when p1 reassigned
}

void SharedPtrReset() {
    std::cout << "\n=== shared_ptr Reset ===" << std::endl;

    auto p1 = std::make_shared<SharedResource>("reset_me");
    auto p2 = p1;

    std::cout << "Count before reset: " << p1.use_count() << std::endl;

    // Reset one owner
    p1.reset();

    std::cout << "After p1.reset():" << std::endl;
    std::cout << "  p1 is null: " << (p1 == nullptr) << std::endl;
    std::cout << "  p2 count: " << p2.use_count() << std::endl;
    std::cout << "  p2 still valid: " << p2->name() << std::endl;

    // Reset last owner
    p2.reset();
    std::cout << "After p2.reset(): object destroyed" << std::endl;
}

void SharedPtrSwap() {
    std::cout << "\n=== shared_ptr Swap ===" << std::endl;

    auto p1 = std::make_shared<SharedResource>("A");
    auto p2 = std::make_shared<SharedResource>("B");

    std::cout << "Before: p1=" << p1->name() << ", p2=" << p2->name() << std::endl;

    p1.swap(p2);

    std::cout << "After:  p1=" << p1->name() << ", p2=" << p2->name() << std::endl;
}

void SharedPtrInContainers() {
    std::cout << "\n=== shared_ptr in Containers ===" << ::endl;

    std::vector<std::shared_ptr<SharedResource>> resources;

    auto r1 = std::make_shared<SharedResource>("container_item");
    resources.push_back(r1);
    resources.push_back(r1);  // Same object twice!
    resources.push_back(std::make_shared<SharedResource>("another"));

    std::cout << "Container size: " << resources.size() << std::endl;
    std::cout << "r1 use count: " << r1.use_count() << " (vector has it twice)" << std::endl;

    // Multiple containers can share
    std::vector<std::shared_ptr<SharedResource>> more = resources;

    std::cout << "After copy to another vector:" << std::endl;
    std::cout << "r1 use count: " << r1.use_count() << std::endl;
}