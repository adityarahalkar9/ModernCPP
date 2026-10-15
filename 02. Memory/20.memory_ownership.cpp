#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "memory_header.h"

// Ownership models overview
void OwnershipModels() {
    std::cout << "=== Memory Ownership Models ===" << std::endl;

    std::cout << "1. Exclusive ownership (unique_ptr)" << std::endl;
    std::cout << "   - One owner at a time" << std::endl;
    std::cout << "   - Owner responsible for deletion" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Shared ownership (shared_ptr)" << std::endl;
    std::cout << "   - Multiple owners" << std::endl;
    std::cout << "   - Deleted when last owner releases" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Non-owning (raw pointer, reference, weak_ptr)" << std::endl;
    std::cout << "   - No responsibility for deletion" << std::endl;
    std::cout << "   - Must ensure object outlives reference" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Transferred ownership" << std::endl;
    std::cout << "   - Ownership passed from one entity to another" << std::endl;
    std::cout << "   - Common in factory functions" << std::endl;
}

// Exclusive ownership
void ExclusiveOwnership() {
    std::cout << "\n=== Exclusive Ownership ===" << std::endl;

    // unique_ptr: single owner
    auto owner = std::make_unique<int>(42);
    std::cout << "Owner has value: " << *owner << std::endl;

    // Transfer ownership (move)
    auto new_owner = std::move(owner);
    std::cout << "New owner has value: " << *new_owner << std::endl;

    if(!owner) {
        std::cout << "Original owner is null (ownership transferred)" << std::endl;
    }

    // Cannot copy unique_ptr
    // auto copy = new_owner;  // Error: deleted copy constructor
}

// Shared ownership
void SharedOwnership() {
    std::cout << "\n=== Shared Ownership ===" << std::endl;

    auto owner1 = std::make_shared<int>(100);
    std::cout << "Count after creation: " << owner1.use_count() << std::endl;

    {
        auto owner2 = owner1;  // Share ownership
        auto owner3 = owner1;  // Share ownership
        std::cout << "Count with 3 owners: " << owner1.use_count() << std::endl;

        std::cout << "All owners see same value: "
            << *owner1 << " == " << *owner2 << std::endl;
    }  // owner2 and owner3 destroyed

    std::cout << "Count after scope: " << owner1.use_count() << std::endl;
    std::cout << "Object still alive (owner1 remains)" << std::endl;
}

// Non-owning references
void NonOwningReferences() {
    std::cout << "\n=== Non-Owning References ===" << std::endl;

    // Raw pointer (non-owning convention)
    auto owner = std::make_unique<int>(42);
    int* raw = owner.get();  // Non-owning access

    std::cout << "Value through raw pointer: " << *raw << std::endl;
    std::cout << "Owner still controls lifetime" << std::endl;

    // Reference (non-owning)
    int& ref = *owner;
    ref = 100;  // Can modify through reference
    std::cout << "Modified through reference: " << *owner << std::endl;

    // weak_ptr: non-owning smart reference
    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;

    std::cout << "Before reset: " << (weak.expired() ? "expired" : "valid") << std::endl;

    shared.reset();  // Object destroyed
    std::cout << "After reset: " << (weak.expired() ? "expired" : "valid") << std::endl;

    // Must lock() to use
    auto shared2 = std::make_shared<int>(100);
    std::weak_ptr<int> weak2 = shared2;

    if(auto locked = weak2.lock()) {
        std::cout << "Locked value: " << *locked << std::endl;
    }
}

// Ownership in containers
class Widget {
public:
    explicit Widget(int id) : id_(id) {
        std::cout << "  Widget(" << id << ") created" << std::endl;
    }
    ~Widget() {
        std::cout << "  Widget(" << id_ << ") destroyed" << std::endl;
    }

    int id() const { return id_; }

private:
    int id_;
};

void OwnershipInContainers() {
    std::cout << "\n=== Ownership in Containers ===" << std::endl;

    // Container owns objects (value semantics)
    std::vector<Widget> widgets;
    widgets.emplace_back(1);
    widgets.emplace_back(2);
    std::cout << "Container owns widgets" << std::endl;

    // Container of unique_ptr (ownership transferred to container)
    std::vector<std::unique_ptr<Widget>> unique_widgets;
    unique_widgets.push_back(std::make_unique<Widget>(3));
    unique_widgets.push_back(std::make_unique<Widget>(4));
    std::cout << "Container owns via unique_ptr" << std::endl;

    // Container of shared_ptr (shared ownership)
    std::vector<std::shared_ptr<Widget>> shared_widgets;
    auto w5 = std::make_shared<Widget>(5);
    shared_widgets.push_back(w5);
    shared_widgets.push_back(w5);  // Same object twice!
    std::cout << "w5 use_count: " << w5.use_count() << std::endl;
}

// Transfer of ownership patterns
class Resource {
public:
    explicit Resource(std::string name) : name_(std::move(name)) {
        std::cout << "  Resource('" << name_ << "') acquired" << std::endl;
    }
    ~Resource() {
        std::cout << "  Resource('" << name_ << "') released" << std::endl;
    }

private:
    std::string name_;
};

// Factory function transfers ownership to caller
std::unique_ptr<Resource> AcquireResource(std::string name) {
    return std::make_unique<Resource>(std::move(name));
}

// Function takes ownership
void ConsumeResource(std::unique_ptr<Resource> res) {
    std::cout << "  Consuming resource..." << std::endl;
    // Resource destroyed when function exits
}

void OwnershipTransfer() {
    std::cout << "\n=== Ownership Transfer ===" << std::endl;

    std::cout << "Factory creates and transfers:" << std::endl;
    auto res = AcquireResource("my_resource");

    std::cout << "Passing to consumer (transfers ownership):" << std::endl;
    ConsumeResource(std::move(res));

    if(!res) {
        std::cout << "Original no longer owns resource" << std::endl;
    }
}

// Best practices
void OwnershipBestPractices() {
    std::cout << "\n=== Ownership Best Practices ===" << std::endl;

    std::cout << "1. Express ownership in types:" << std::endl;
    std::cout << "   - unique_ptr: I own this exclusively" << std::endl;
    std::cout << "   - shared_ptr: I share ownership" << std::endl;
    std::cout << "   - raw ptr/ref: I don't own (just observing)" << std::endl;

    std::cout << "\n2. Prefer unique_ptr over shared_ptr" << std::endl;
    std::cout << "   (shared_ptr has overhead, can create cycles)" << std::endl;

    std::cout << "\n3. Use make_unique/make_shared" << std::endl;
    std::cout << "   (avoids raw new, more efficient)" << std::endl;

    std::cout << "\n4. Pass by unique_ptr to transfer ownership" << std::endl;
    std::cout << "   (by value, not by reference)" << std::endl;

    std::cout << "\n5. Return unique_ptr from factories" << std::endl;

    std::cout << "\n6. Use weak_ptr to break shared_ptr cycles" << std::endl;
}