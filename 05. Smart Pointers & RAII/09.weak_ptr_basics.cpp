import std;
#include "smart_pointers_header.h"

class WeakDemo {
public:
    explicit WeakDemo(std::string name) : name_(std::move(name)) {
        std::cout << "  WeakDemo('" << name_ << "') created" << std::endl;
    }

    ~WeakDemo() {
        std::cout << "  WeakDemo('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void WeakPtrCreation() {
    std::cout << "=== weak_ptr Creation ===" << std::endl;

    std::cout << "weak_ptr MUST be created from shared_ptr:" << std::endl;

    auto shared = std::make_shared<WeakDemo>("weak_target");
    std::weak_ptr<WeakDemo> weak = shared;

    std::cout << "  weak expired: " << weak.expired() << std::endl;
    std::cout << "  shared count: " << shared.use_count() << std::endl;

    // weak_ptr doesn't affect strong count
    std::cout << "  (weak doesn't add to strong count)" << std::endl;
}

void WeakPtrLock() {
    std::cout << "\n=== weak_ptr lock() ===" << std::endl;

    std::cout << "lock() converts weak_ptr to shared_ptr:" << std::endl;
    std::cout << "  Returns empty shared_ptr if object gone" << std::endl;

    auto shared = std::make_shared<WeakDemo>("lockable");
    std::weak_ptr<WeakDemo> weak = shared;

    // Lock: get shared_ptr
    if(auto locked = weak.lock()) {
        std::cout << "  Locked successfully: " << locked->name() << std::endl;
        std::cout << "  Count after lock: " << shared.use_count() << std::endl;
    }

    // After lock expires, count goes back
    std::cout << "  Count after lock gone: " << shared.use_count() << std::endl;
}

void WeakPtrExpired() {
    std::cout << "\n=== weak_ptr expired() ===" << std::endl;

    std::weak_ptr<WeakDemo> weak;

    {
        auto shared = std::make_shared<WeakDemo>("temporary");
        weak = shared;
        std::cout << "  Before expiry: " << (weak.expired() ? "expired" : "valid") << std::endl;
    }

    std::cout << "  After shared gone: " << (weak.expired() ? "expired" : "valid") << std::endl;

    // lock() on expired returns empty
    auto locked = weak.lock();
    std::cout << "  lock() result: " << (locked ? "valid" : "null") << std::endl;
}

void WeakPtrUseCount() {
    std::cout << "\n=== weak_ptr use_count() ===" << std::endl;

    auto shared = std::make_shared<WeakDemo>("counted");
    std::weak_ptr<WeakDemo> weak = shared;

    // weak_ptr::use_count returns STRONG count
    std::cout << "  weak.use_count(): " << weak.use_count() << std::endl;
    std::cout << "  shared.use_count(): " << shared.use_count() << std::endl;

    // Add more strong references
    auto more = shared;
    std::cout << "  After copy: " << weak.use_count() << std::endl;

    // Remove strong references
    more.reset();
    std::cout << "  After reset one: " << weak.use_count() << std::endl;

    shared.reset();
    std::cout << "  After all gone: " << weak.use_count() << std::endl;
    std::cout << "  expired: " << weak.expired() << std::endl;
}

void WeakPtrReset() {
    std::cout << "\n=== weak_ptr reset() ===" std::endl;

    auto shared = std::make_shared<WeakDemo>("reset_weak");
    std::weak_ptr<WeakDemo> weak = shared;

    std::cout << "  Before reset: " << (weak.expired() ? "expired" : "valid") << std::endl;

    weak.reset();
    std::cout << "  After reset: " << (weak.expired() ? "expired" : "valid") << std::endl;

    // Object still alive (shared still owns)
    std::cout << "  Object still alive: " << shared->name() << std::endl;
}

void WeakPtrInClasses() {
    std::cout << "\n=== weak_ptr in Classes ===" << std::endl;

    class Observer {
        std::weak_ptr<WeakDemo> watched_;

    public:
        void watch(std::shared_ptr<WeakDemo> target) {
            watched_ = target;
        }

        void report() {
            if(auto target = watched_.lock()) {
                std::cout << "  Watching: " << target->name() << std::endl;
            }
            else {
                std::cout << "  Target no longer exists" << std::endl;
            }
        }
    };

    Observer observer;
    auto target = std::make_shared<WeakDemo>("observed");

    observer.watch(target);
    observer.report();

    target.reset();  // Destroy target
    std::cout << "  After target destroyed:" << std::endl;
    observer.report();
}

void WeakPtrSwap() {
    std::cout << "\n=== weak_ptr swap ===" << std::endl;

    auto s1 = std::make_shared<WeakDemo>("first");
    auto s2 = std::make_shared<WeakDemo>("second");

    std::weak_ptr<WeakDemo> w1 = s1;
    std::weak_ptr<WeakDemo> w2 = s2;

    std::cout << "Before: w1->" << w1.lock()->name()
        << ", w2->" << w2.lock()->name() << std::endl;

    w1.swap(w2);

    std::cout << "After:  w1->" << w1.lock()->name()
        << ", w2->" << w2.lock()->name() << std::endl;
}