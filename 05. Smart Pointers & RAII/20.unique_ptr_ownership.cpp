import std;
#include "smart_pointers_header.h"

class Owned {
public:
    explicit Owned(std::string name) : name_(std::move(name)) {
        std::cout << "  Owned('" << name_ << "') created" << std::endl;
    }

    ~Owned() {
        std::cout << "  Owned('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

// Expressing ownership in APIs
class Owner2 {
    std::unique_ptr<Owned> owned_;  // exclusive ownership

public:
    explicit Owner2(std::unique_ptr<Owned> obj)
        : owned_(std::move(obj)) {
        std::cout << "  Owner2 now owns: " << owned_->name() << std::endl;
    }

    ~Owner2() {
        std::cout << "  Owner2 destroyed" << std::endl;
    }

    // Transfer out
    std::unique_ptr<Owned> release() {
        return std::move(owned_);
    }

    // Access without transferring
    Owned& get() const { return *owned_; }
};

void OwnershipSemantics() {
    std::cout << "=== Ownership Semantics ===" << std::endl;

    std::cout << "unique_ptr clearly expresses:" << std::endl;
    std::cout << "  'I own this exclusively'" << std::endl;
    std::cout << std::endl;

    std::cout << "API design patterns:" << std::endl;
    std::cout << "  unique_ptr<T> param  → taking ownership" << std::endl;
    std::cout << "  unique_ptr<T> return → giving ownership" << std::endl;
    std::cout << "  T& param            → borrowing (no ownership)" << std::endl;
    std::cout << "  T* param            → borrowing (nullable)" << std::endl;
}

void TransferOwnership() {
    std::cout << "\n=== Transferring Ownership ===" << std::endl;

    std::cout << "Creating object:" << std::endl;
    auto owned = std::make_unique<Owned>("transferable");

    std::cout << "\nTransferring to Owner2:" << std::endl;
    Owner2 owner(std::move(owned));

    std::cout << "  owned is null: " << (owned == nullptr) << std::endl;

    std::cout << "\nOwner2 using resource:" << std::endl;
    std::cout << "  " << owner.get().name() << std::endl;

    std::cout << "\nReleasing from Owner2:" << std::endl;
    auto released = owner.release();
    std::cout << "  Got back: " << released->name() << std::endl;
}

// Factory transferring ownership
std::unique_ptr<Owned> create_owned(std::string name) {
    return std::make_unique<Owned>(std::move(name));
}

void FactoryOwnership() {
    std::cout << "\n=== Factory Pattern ===" << std::endl;

    std::cout << "Factory creates and transfers ownership:" << std::endl;
    auto obj = create_owned("factory_created");
    std::cout << "  Caller now owns: " << obj->name() << std::endl;
}

// Container of unique_ptr
void UniquePtrContainers() {
    std::cout << "\n=== unique_ptr Containers ===" << std::endl;

    std::vector<std::unique_ptr<Owned>> collection;

    std::cout << "Adding to container (ownership transfers):" << std::endl;
    collection.push_back(create_owned("item1"));
    collection.push_back(create_owned("item2"));

    std::cout << "\nContainer owns all items:" << std::endl;
    for(const auto& item : collection) {
        std::cout << "  " << item->name() << std::endl;
    }

    std::cout << "\nMoving out of container:" << std::endl;
    auto taken = std::move(collection[0]);
    std::cout << "  Taken: " << taken->name() << std::endl;
    std::cout << "  collection[0] is null: " << (collection[0] == nullptr) << std::endl;
}

// Returning references vs unique_ptr
class Service {
    std::unique_ptr<Owned> resource_;

public:
    Service() : resource_(std::make_unique<Owned>("service_resource")) {}

    // Return reference (borrowing)
    Owned& get_resource() { return *resource_; }

    // Return unique_ptr (transfer)
    std::unique_ptr<Owned> release_resource() {
        return std::move(resource_);
    }

    bool has_resource() const { return resource_ != nullptr; }
};

void ReferenceVsOwnership() {
    std::cout << "\n=== Reference vs Ownership ===" << std::endl;

    Service service;
    std::cout << "  Service has resource: " << service.has_resource() << std::endl;

    // Borrow
    Owned& ref = service.get_resource();
    std::cout << "  Borrowed: " << ref.name() << std::endl;
    std::cout << "  Service still owns: " << service.has_resource() << std::endl;

    // Take ownership
    auto owned = service.release_resource();
    std::cout << "  Service gave up: " << !service.has_resource() << std::endl;
    std::cout << "  We now own: " << owned->name() << std::endl;
}