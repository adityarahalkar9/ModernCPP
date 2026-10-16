import std;
#include "smart_pointers_header.h"

// Basic enable_shared_from_this
class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity() {
        std::cout << "  Entity constructed" << std::endl;
    }

    ~Entity() {
        std::cout << "  Entity destroyed" << std::endl;
    }

    // WRONG: returning 'this' as shared_ptr
    std::shared_ptr<Entity> bad_get_shared() {
        // This creates NEW control block → DOUBLE DELETION!
        return std::shared_ptr<Entity>(this);  // NEVER DO THIS!
    }

    // CORRECT: use shared_from_this()
    std::shared_ptr<Entity> get_shared() {
        return shared_from_this();  // Uses existing control block
    }

    void demonstrate() {
        auto self = shared_from_this();
        std::cout << "  Got self: " << self.get() << std::endl;
        std::cout << "  use_count: " << self.use_count() << std::endl;
    }
};

void EnableSharedBasics() {
    std::cout << "=== enable_shared_from_this ===" << std::endl;

    std::cout << "Problem: getting shared_ptr to 'this'" << std::endl;
    std::cout << "  this is raw pointer, not shared_ptr" << std::endl;
    std::cout << "  shared_ptr<T>(this) creates NEW control block" << std::endl;
    std::cout << "  → Two control blocks for same object" << std::endl;
    std::cout << "  → DOUBLE DELETION!" << std::endl;

    std::cout << "\nSolution: inherit enable_shared_from_this<T>" << std::endl;
    std::cout << "  Object stores weak_ptr to itself" << std::endl;
    std::cout << "  shared_from_this() uses existing control block" << std::endl;

    auto entity = std::make_shared<Entity>();

    std::cout << "\nGetting shared_ptr to self:" << std::endl;
    auto self = entity->get_shared();

    std::cout << "  Same object: " << (self.get() == entity.get()) << std::endl;
    std::cout << "  use_count: " << entity.use_count() << std::endl;
}

void SharedFromThisInside() {
    std::cout << "\n=== shared_from_this() Inside Methods ===" << std::endl;

    auto entity = std::make_shared<Entity>();

    std::cout << "Calling method that uses shared_from_this():" << std::endl;
    entity->demonstrate();
}

// Common use case: async operations
class AsyncOperation : public std::enable_shared_from_this<AsyncOperation> {
public:
    AsyncOperation() {
        std::cout << "  AsyncOperation created" << std::endl;
    }

    ~AsyncOperation() {
        std::cout << "  AsyncOperation destroyed" << std::endl;
    }

    // WRONG: capturing 'this' in lambda
    void bad_async() {
        // If object destroyed before callback runs → dangling this
        // auto self = this;  // DANGEROUS
    }

    // CORRECT: capture shared_from_this()
    void good_async() {
        auto self = shared_from_this();
        // Now lambda holds shared_ptr
        // Object stays alive until callback runs
        std::cout << "  Captured self in lambda (keeps alive)" << std::endl;
    }
};

void AsyncUseCase() {
    std::cout << "\n=== Async Operations Use Case ===" << std::endl;

    std::cout << "Problem with 'this' in async code:" << std::endl;
    std::cout << "  Object may be destroyed before callback runs" << std::endl;
    std::cout << "  → Dangling pointer, undefined behavior" << std::endl;

    std::cout << "\nSolution: capture shared_from_this():" << std::endl;
    std::cout << "  Lambda holds shared_ptr" << std::endl;
    std::cout << "  Object guaranteed alive during callback" << std::endl;

    auto op = std::make_shared<AsyncOperation>();
    op->good_async();
}

// Rules for enable_shared_from_this
void EnableSharedRules() {
    std::cout << "\n=== Rules for enable_shared_from_this ===" << std::endl;

    std::cout << "1. Object MUST be managed by shared_ptr" << std::endl;
    std::cout << "   Entity e;  // Stack object" << std::endl;
    std::cout << "   e.shared_from_this();  // ERROR or UB!" << std::endl;
    std::endl;

    std::cout << "2. shared_from_this() requires existing shared_ptr" << std::endl;
    std::cout << "   Cannot call in constructor!" << std::endl;
    std::cout << "   (Object not yet managed by shared_ptr)" << std::endl;
    std::endl;

    std::cout << "3. Returns shared_ptr that shares control block" << std::endl;
    std::cout << "   Same reference count as original" << std::endl;
    std::endl;

    std::cout << "4. weak_from_this() also available (C++17)" << std::endl;
    std::cout << "   Returns weak_ptr instead" << std::endl;
}

// weak_from_this demonstration
class WeakEntity : public std::enable_shared_from_this<WeakEntity> {
public:
    void demonstrate_weak() {
        auto weak_self = weak_from_this();
        std::cout << "  Got weak_ptr to self" << std::endl;
        std::cout << "  expired: " << weak_self.expired() << std::endl;

        if(auto shared_self = weak_self.lock()) {
            std::cout << "  Locked: " << shared_self.get() << " == " << this << std::endl;
        }
    }
};

void WeakFromThisDemo() {
    std::cout << "\n=== weak_from_this (C++17) ===" << std::endl;

    auto entity = std::make_shared<WeakEntity>();
    entity->demonstrate_weak();
}