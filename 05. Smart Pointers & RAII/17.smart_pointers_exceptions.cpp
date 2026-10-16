import std;
#include "smart_pointers_header.h"

class ExceptionResource {
public:
    explicit ExceptionResource(std::string name) : name_(std::move(name)) {
        std::cout << "  ExceptionResource('" << name_ << "') created" << std::endl;
    }

    ~ExceptionResource() {
        std::cout << "  ExceptionResource('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void ExceptionSafety() {
    std::cout << "=== Smart Pointers and Exceptions ===" << std::endl;

    std::cout << "Problem with raw pointers:" << std::endl;
    std::cout << "  Resource* res = new Resource();" << std::endl;
    std::cout << "  may_throw();  // If this throws..." << std::endl;
    std::cout << "  delete res;   // ...this never runs → LEAK!" << std::endl;

    std::cout << "\nWith smart pointers:" << std::endl;
    std::cout << "  auto res = std::make_unique<Resource>();" << std::endl;
    std::cout << "  may_throw();  // If this throws..." << std::endl;
    std::cout << "  // ...unique_ptr destructor still runs → NO LEAK!" << std::endl;
}

void ExceptionDuringConstruction() {
    std::cout << "\n=== Exceptions During Construction ===" << std::endl;

    std::cout << "If constructor throws, no object exists:" << std::endl;
    std::cout << "  Destructor NOT called" << std::endl;
    std::cout << "  Memory automatically freed" << std::endl;

    class ThrowingConstructor {
    public:
        ThrowingConstructor() {
            std::cout << "  Constructor started..." << std::endl;
            throw std::runtime_error("Construction failed");
        }

        ~ThrowingConstructor() {
            std::cout << "  Destructor (should NOT appear)" << std::endl;
        }
    };

    try {
        auto obj = std::make_unique<ThrowingConstructor>();
    }
    catch(const std::exception& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
        std::cout << "  (Destructor never called - no object)" << std::endl;
    }
}

void CleanupWithExceptions() {
    std::cout << "\n=== Cleanup With Exceptions ===" << std::endl;

    auto risky_operation = [](bool should_throw) {
        auto res1 = std::make_unique<ExceptionResource>("first");
        auto res2 = std::make_unique<ExceptionResource>("second");

        std::cout << "  Resources acquired" << std::endl;

        if(should_throw) {
            throw std::runtime_error("Something went wrong");
        }

        std::cout << "  Operation successful" << std::endl;
        };

    std::cout << "Normal operation:" << std::endl;
    try {
        risky_operation(false);
    }
    catch(...) {
        std::cout << "  Unexpected!" << std::endl;
    }

    std::cout << "\nOperation with exception:" << std::endl;
    try {
        risky_operation(true);
    }
    catch(const std::exception& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
        std::cout << "  (Resources still cleaned up properly)" << std::endl;
    }
}

void StrongExceptionGuarantee() {
    std::cout << "\n=== Strong Exception Guarantee ===" << std::endl;

    std::cout << "Smart pointers help provide strong guarantee:" << std::endl;
    std::cout << "  If operation fails, state unchanged" << std::endl;

    class Transaction {
        std::unique_ptr<ExceptionResource> backup_;
        std::unique_ptr<ExceptionResource> current_;

    public:
        explicit Transaction(std::string name)
            : current_(std::make_unique<ExceptionResource>(std::move(name))) {
        }

        void risky_update(bool fail) {
            // Save current state
            backup_ = std::move(current_);

            try {
                current_ = std::make_unique<ExceptionResource>("updated");
                if(fail) {
                    throw std::runtime_error("Update failed");
                }
                std::cout << "  Update successful" << std::endl;
                backup_.reset();  // Don't need backup
            }
            catch(...) {
                std::cout << "  Restoring backup..." << std::endl;
                current_ = std::move(backup_);  // Restore
                throw;  // Re-throw
            }
        }

        const std::string& current_name() const { return current_->name(); }
    };

    Transaction txn("original");
    std::cout << "  Current: " << txn.current_name() << std::endl;

    try {
        txn.risky_update(true);  // Will fail
    }
    catch(const std::exception& e) {
        std::cout << "  After failure: " << txn.current_name() << " (restored)" << std::endl;
    }
}

void NoexceptDestructors() {
    std::cout << "\n=== noexcept Destructors ===" << std::endl;

    std::cout << "Destructors should NEVER throw:" << std::endl;
    std::cout << "  Implicitly noexcept" << std::endl;
    std::cout << "  Throwing during stack unwind → std::terminate" << std::endl;

    std::cout << "\nSmart pointer destructors are noexcept:" << std::endl;
    std::cout << "  unique_ptr<T>::~unique_ptr() noexcept" << std::endl;
    std::cout << "  shared_ptr<T>::~shared_ptr() noexcept" << std::endl;

    // static_assert(std::is_nothrow_destructible_v<std::unique_ptr<int>>);
    std::cout << "  (Verified at compile time)" << std::endl;
}