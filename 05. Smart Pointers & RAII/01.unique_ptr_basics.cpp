import std;
#include "smart_pointers_header.h"

/*
	1. WHAT IS RAII?
		RAII = Resource Acquisition Is Initialization
	   • A resource (memory, file handle, lock, socket, ...) is acquired
	     inside an object’s constructor.
	   • That same resource is released inside the object’s destructor.
	   • The resource’s lifetime is exactly the object’s lifetime.
	   • Because C++ guarantees that stack-allocated objects have their
	     destructors called when they go out of scope (even during stack
	     unwinding from an exception), resources are *never* leaked.
	 In short: Tie resource management to object lifetime.
*/


// Simple resource class for demonstration
class Resource {
public:
    explicit Resource(std::string name) : name_(std::move(name)) {
        std::cout << "  Resource('" << name_ << "') acquired" << std::endl;
    }

    ~Resource() {
        std::cout << "  Resource('" << name_ << "') released" << std::endl;
    }

    void use() const {
        std::cout << "  Using resource: " << name_ << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void UniquePtrCreation() {
    std::cout << "=== unique_ptr Creation ===" << std::endl;

    // Method 1: Direct construction
    std::unique_ptr<Resource> p1(new Resource("direct"));

    // Method 2: make_unique (preferred - C++14)
    auto p2 = std::make_unique<Resource>("make_unique");

    // Method 3: Construct with nullptr
    std::unique_ptr<Resource> p3(nullptr);

    std::cout << "\np1 owns: " << p1->name() << std::endl;
    std::cout << "p2 owns: " << p2->name() << std::endl;
    std::cout << "p3 is null: " << (p3 == nullptr) << std::endl;
}

void UniquePtrAccess() {
    std::cout << "\n=== unique_ptr Access ===" << std::endl;

    auto ptr = std::make_unique<Resource>("access_demo");

    // Dereference with *
    (*ptr).use();

    // Member access with ->
    ptr->use();

    // Get raw pointer (non-owning)
    Resource* raw = ptr.get();
    raw->use();

    // Check if valid
    if(ptr) {
        std::cout << "  ptr is valid" << std::endl;
    }

    std::cout << "  ptr.get() == raw: " << (ptr.get() == raw) << std::endl;
}

void UniquePtrTransfer() {
    std::cout << "\n=== unique_ptr Transfer ===" << std::endl;

    std::cout << "Creating owner:" << std::endl;
    auto owner = std::make_unique<Resource>("transferable");
    std::cout << "  owner owns: " << owner->name() << std::endl;

    std::cout << "\nTransferring (move):" << std::endl;
    auto new_owner = std::move(owner);

    std::cout << "  owner is now null: " << (owner == nullptr) << std::endl;
    std::cout << "  new_owner owns: " << new_owner->name() << std::endl;

    std::cout << "\nCannot copy unique_ptr:" << std::endl;
    std::cout << "  auto copy = owner;  // COMPILE ERROR!" << std::endl;
}

void UniquePtrReset() {
    std::cout << "\n=== unique_ptr Reset ===" << std::endl;

    auto ptr = std::make_unique<Resource>("original");
    std::cout << "  ptr owns: " << ptr->name() << std::endl;

    std::cout << "\nResetting (destroys original):" << std::endl;
    ptr.reset();
    std::cout << "  ptr is null: " << (ptr == nullptr) << std::endl;

    std::cout << "\nReset with new resource:" << std::endl;
    ptr.reset(new Resource("replacement"));
    std::cout << "  ptr owns: " << ptr->name() << std::endl;

    // reset() with no args is same as assigning nullptr
    ptr.reset();
    std::cout << "  After reset(): " << (ptr == nullptr) << std::endl;
}

void UniquePtrRelease() {
    std::cout << "\n=== unique_ptr Release ===" << std::endl;

    std::cout << "release() gives up ownership WITHOUT destroying:" << std::endl;

    auto ptr = std::make_unique<Resource>("released");
    std::cout << "  ptr owns: " << ptr->name() << std::endl;

    // release() returns raw pointer, unique_ptr becomes null
    Resource* raw = ptr.release();

    std::cout << "  ptr is now null: " << (ptr == nullptr) << std::endl;
    std::cout << "  raw points to: " << raw->name() << std::endl;

    // We must manually delete
    std::cout << "  Must manually delete: ";
    delete raw;
}

void UniquePtrSwap() {
    std::cout << "\n=== unique_ptr Swap ===" << std::endl;

    auto ptr1 = std::make_unique<Resource>("first");
    auto ptr2 = std::make_unique<Resource>("second");

    std::cout << "Before swap:" << std::endl;
    std::cout << "  ptr1: " << ptr1->name() << std::endl;
    std::cout << "  ptr2: " << ptr2->name() << std::endl;

    ptr1.swap(ptr2);

    std::cout << "After swap:" << std::endl;
    std::cout << "  ptr1: " << ptr1->name() << std::endl;
    std::cout << "  ptr2: " << ptr2->name() << std::endl;

    // Also works with std::swap
    std::swap(ptr1, ptr2);
    std::cout << "After std::swap:" << std::endl;
    std::cout << "  ptr1: " << ptr1->name() << std::endl;
    std::cout << "  ptr2: " << ptr2->name() << std::endl;
}

/*
	The RAII contract - Every RAII contract obeys
	1. Constructor: acquire the resource, or throw - never leave the object half-constructed
	2. Destructor: release the resource never throw
	3. Copy: either forbid, deep-copy 
	4. Move: transfer ownership and reset the source to a safe empty state
	5. No manual release: the user of the type should not have to remember everything
*/

