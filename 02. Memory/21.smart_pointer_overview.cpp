#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "memory_header.h"

// unique_ptr basics
void UniquePtrBasics() {
    std::cout << "=== unique_ptr ===" << std::endl;

    // Creation
    auto p1 = std::make_unique<int>(42);
    std::cout << "Value: " << *p1 << std::endl;

    // Cannot copy
    // auto p2 = p1;  // Error: copy constructor deleted

    // Can move
    auto p3 = std::move(p1);
    std::cout << "After move, p3 = " << *p3 << std::endl;
    std::cout << "p1 is null: " << (!p1 ? "yes" : "no") << std::endl;

    // Manual reset
    p3.reset();  // Deletes object
    std::cout << "After reset, p3 is null: " << (!p3 ? "yes" : "no") << std::endl;

    // Array version
    auto arr = std::make_unique<int[]>(5);
    for(size_t i = 0; i < 5; ++i) {
        arr[i] = static_cast<int>(i * 10);
    }
    std::cout << "Array: ";
    for(size_t i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// unique_ptr with custom deleter
void UniquePtrCustomDeleter() {
    std::cout << "\n=== unique_ptr with Custom Deleter ===" << std::endl;

    // Custom deleter for FILE*
    struct FileCloser {
        void operator()(FILE* f) const {
            if(f) {
                std::cout << "  Closing file..." << std::endl;
                fclose(f);
            }
        }
    };

    // Note: This is just a demonstration
    // std::unique_ptr<FILE, FileCloser> file(fopen("test.txt", "w"));

    // Lambda deleter
    auto deleter = [](int* p) {
        std::cout << "  Custom delete for: " << *p << std::endl;
        delete p;
        };

    std::unique_ptr<int, decltype(deleter)> custom(new int(100), deleter);
    std::cout << "Value: " << *custom << std::endl;
}  // Custom deleter called

// shared_ptr basics
void SharedPtrBasics() {
    std::cout << "\n=== shared_ptr ===" << std::endl;

    // Creation
    auto p1 = std::make_shared<int>(42);
    std::cout << "p1 count: " << p1.use_count() << std::endl;

    // Copying increases count
    {
        auto p2 = p1;
        auto p3 = p1;
        std::cout << "Count with 3 owners: " << p1.use_count() << std::endl;
    }

    std::cout << "Count after scope: " << p1.use_count() << std::endl;

    // All point to same object
    auto p4 = p1;
    std::cout << "Same object: " << (p1.get() == p4.get()) << std::endl;
}

// shared_ptr aliasing constructor
void SharedPtrAliasing() {
    std::cout << "\n=== shared_ptr Aliasing ===" << std::endl;

    struct Person {
        std::string name = "Alice";
        int age = 30;
    };

    auto person = std::make_shared<Person>();

    // Aliasing: shares ownership but points to member
    auto name_ptr = std::shared_ptr<std::string>(person, &person->name);
    auto age_ptr = std::shared_ptr<int>(person, &person->age);

    std::cout << "Name: " << *name_ptr << std::endl;
    std::cout << "Age: " << *age_ptr << std::endl;
    std::cout << "Count: " << person.use_count() << std::endl;

    // All share ownership of Person
    // Person destroyed when all are gone
}

// weak_ptr basics
void WeakPtrBasics() {
    std::cout << "\n=== weak_ptr ===" << std::endl;

    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;

    std::cout << "Before expiry: " << (weak.expired() ? "expired" : "valid") << std::endl;

    // Must lock() to access
    if(auto locked = weak.lock()) {
        std::cout << "Value: " << *locked << std::endl;
    }

    shared.reset();
    std::cout << "After reset: " << (weak.expired() ? "expired" : "valid") << std::endl;

    // lock() returns empty shared_ptr
    auto locked = weak.lock();
    std::cout << "lock() result: " << (locked ? "valid" : "null") << std::endl;
}

// Breaking cycles with weak_ptr
struct Node {
    std::string name;
    std::shared_ptr<Node> next;       // Creates cycle if shared
    std::weak_ptr<Node> prev;          // Use weak to avoid cycle

    explicit Node(std::string n) : name(std::move(n)) {
        std::cout << "  Node('" << name << "') created" << std::endl;
    }
    ~Node() {
        std::cout << "  Node('" << name << "') destroyed" << std::endl;
    }
};

void BreakingCycles() {
    std::cout << "\n=== Breaking Cycles ===" << std::endl;

    std::cout << "Creating linked nodes:" << std::endl;
    auto node1 = std::make_shared<Node>("first");
    auto node2 = std::make_shared<Node>("second");

    node1->next = node2;      // node1 owns node2
    node2->prev = node1;      // node2 doesn't own node1 (weak)

    std::cout << "node1 count: " << node1.use_count() << std::endl;
    std::cout << "node2 count: " << node2.use_count() << std::endl;

    std::cout << "Resetting node1:" << std::endl;
    node1.reset();  // node1 destroyed, but node2 still holds weak ref

    std::cout << "node2 still alive" << std::endl;
    std::cout << "node2->prev expired: " << (node2->prev.expired() ? "yes" : "no") << std::endl;

    node2.reset();  // Now node2 destroyed
    std::cout << "All nodes destroyed" << std::endl;
}

// enable_shared_from_this
class SharedObject : public std::enable_shared_from_this<SharedObject> {
public:
    SharedObject() {
        std::cout << "  SharedObject created" << std::endl;
    }
    ~SharedObject() {
        std::cout << "  SharedObject destroyed" << std::endl;
    }

    std::shared_ptr<SharedObject> GetShared() {
        return shared_from_this();  // Get shared_ptr to self
    }
};

void EnableSharedFromThis() {
    std::cout << "\n=== enable_shared_from_this ===" << std::endl;

    auto obj = std::make_shared<SharedObject>();
    std::cout << "Count: " << obj.use_count() << std::endl;

    auto obj2 = obj->GetShared();
    std::cout << "Count after GetShared: " << obj.use_count() << std::endl;
    std::cout << "Same object: " << (obj.get() == obj2.get()) << std::endl;
}

// Performance considerations
void PerformanceNotes() {
    std::cout << "\n=== Performance Notes ===" << std::endl;

    std::cout << "unique_ptr:" << std::endl;
    std::cout << "  + Zero overhead (same size as raw pointer)" << std::endl;
    std::cout << "  + No reference counting" << std::endl;
    std::cout << "  + Can be inlined/optimized away" << std::endl;

    std::cout << "\nshared_ptr:" << std::endl;
    std::cout << "  + Reference counting (atomic operations)" << std::endl;
    std::cout << "  + Control block allocation (extra memory)" << std::endl;
    std::cout << "  + make_shared allocates object+control together" << std::endl;
    std::cout << "  - Slower than unique_ptr" << std::endl;

    std::cout << "\nweak_ptr:" << std::endl;
    std::cout << "  + Same size as shared_ptr" << std::endl;
    std::cout << "  + lock() has overhead (atomic check)" << std::endl;

    std::cout << "\nRecommendation:" << std::endl;
    std::cout << "  - Default to unique_ptr" << std::endl;
    std::cout << "  - Use shared_ptr only when needed" << std::endl;
    std::cout << "  - Use weak_ptr for non-owning observations" << std::endl;
}