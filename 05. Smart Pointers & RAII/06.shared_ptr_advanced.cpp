import std;
#include "smart_pointers_header.h"

class Advanced {
public:
    explicit Advanced(std::string name) : name_(std::move(name)) {
        std::cout << "  Advanced('" << name_ << ") constructed" << std::endl;
    }

    ~Advanced() {
        std::cout << "  Advanced('" << name_ << ") destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

    void process() const {
        std::cout << "  Processing " << name_ << std::endl;
    }

private:
    std::string name_;
};

// shared_ptr with custom deleter
void SharedPtrCustomDeleter() {
    std::cout << "=== shared_ptr with Custom Deleter ===" << std::endl;

    auto custom_delete = [](Advanced* p) {
        std::cout << "  Custom deleter for: " << p->name() << std::endl;
        delete p;
        };

    std::shared_ptr<Advanced> ptr(new Advanced("custom"), custom_delete);
    ptr->process();
}

// shared_ptr from unique_ptr
void SharedFromUnique() {
    std::cout << "\n=== shared_ptr from unique_ptr ===" << std::endl;

    std::cout << "Convert unique_ptr to shared_ptr:" << std::endl;

    auto unique = std::make_unique<Advanced>("convertible");
    std::cout << "  unique owns: " << unique->name() << std::endl;

    // Convert (unique_ptr must be moved)
    std::shared_ptr<Advanced> shared = std::move(unique);

    std::cout << "  unique is now null: " << (unique == nullptr) << std::endl;
    std::cout << "  shared owns: " << shared->name() << std::endl;

    // Cannot go back: shared_ptr to unique_ptr
    std::cout << "\nCannot convert shared back to unique!" << std::endl;
}

// Multiple shared_ptrs sharing
void MultipleOwnership() {
    std::cout << "\n=== Multiple Ownership ===" << std::endl;

    std::cout << "Passing to functions that keep a copy:" << std::endl;

    auto keeper = [](std::shared_ptr<Advanced> a) {
        std::cout << "  Keeper received: " << a->name() << std::endl;
        std::cout << "  Count inside: " << a.use_count() << std::endl;
        // 'a' keeps ownership after function returns
        // (in this case, it goes out of scope)
        };

    auto owner = std::make_shared<Advanced>("multi_owner");
    std::cout << "Before call: " << owner.use_count() << std::endl;

    keeper(owner);
    std::cout << "After call: " << owner.use_count() << std::endl;
}

// shared_ptr in class members
class Owner {
    std::shared_ptr<Advanced> resource_;

public:
    explicit Owner(std::shared_ptr<Advanced> r) : resource_(std::move(r)) {
        std::cout << "  Owner constructed with " << resource_->name() << std::endl;
    }

    ~Owner() {
        std::cout << "  Owner destroyed" << std::endl;
    }

    void use_resource() {
        if(resource_) {
            resource_->process();
        }
    }

    std::shared_ptr<Advanced> get_resource() const {
        return resource_;  // Shares ownership
    }
};

void SharedPtrInClasses() {
    std::cout << "\n=== shared_ptr in Classes ===" << std::endl;

    auto shared_res = std::make_shared<Advanced>("class_resource");

    {
        Owner owner1(shared_res);
        Owner owner2(shared_res);

        std::cout << "Count with 2 owners: " << shared_res.use_count() << std::endl;

        owner1.use_resource();
        owner2.use_resource();
    }

    std::cout << "After owners destroyed, count: "
        << shared_res.use_count() << std::endl;
}

// shared_ptr reference counting details
void ReferenceCountingDetails() {
    std::cout << "\n=== Reference Counting Details ===" << std::endl;

    std::cout << "shared_ptr uses ATOMIC reference counting:" << std::endl;
    std::cout << "  Thread-safe for counter itself" << std::endl;
    std::cout << "  NOT thread-safe for pointed-to object" << std::endl;

    auto ptr = std::make_shared<Advanced>("counted");

    // Copy in different scopes
    std::vector<std::shared_ptr<Advanced>> holders;

    std::cout << "\nCount: " << ptr.use_count() << std::endl;
    holders.push_back(ptr);
    std::cout << "After push_back: " << ptr.use_count() << std::endl;

    // Copy to another container
    std::vector<std::shared_ptr<Advanced>> more_holders;
    more_holders.push_back(ptr);
    std::cout << "After second push_back: " << ptr.use_count() << std::endl;

    // Clear one vector
    holders.clear();
    std::cout << "After clearing first: " << ptr.use_count() << std::endl;
}

// shared_ptr comparison
void SharedPtrComparison() {
    std::cout << "\n=== shared_ptr Comparison ===" << std::endl;

    auto p1 = std::make_shared<Advanced>("same");
    auto p2 = p1;  // Shares
    auto p3 = std::make_shared<Advanced>("same");

    std::cout << "p1 == p2 (same object): " << (p1 == p2) << std::endl;
    std::cout << "p1 == p3 (different objects): " << (p1 == p3) << std::endl;
    std::cout << "p1 != p3: " << (p1 != p3) << std::endl;

    // Comparing underlying objects
    std::cout << "*p1 == *p3 (same name): "
        << (p1->name() == p3->name()) << std::endl;
}

// Factory returning shared_ptr
std::shared_ptr<Advanced> create_advanced(std::string name) {
    // Return shared from factory
    return std::make_shared<Advanced>(std::move(name));
}

void SharedPtrFactory() {
    std::cout << "\n=== shared_ptr Factory ===" << std::endl;

    auto created = create_advanced("factory_made");
    std::cout << "  Factory returned: " << created->name() << std::endl;
    std::cout << "  Count: " << created.use_count() << std::endl;
}