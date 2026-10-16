import std;
#include "smart_pointers_header.h"

class Resource2 {
public:
    explicit Resource2(std::string name) : name_(std::move(name)) {
        std::cout << "  Resource2('" << name_ << "') created" << std::endl;
    }

    ~Resource2() {
        std::cout << "  Resource2('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

// Function taking unique_ptr by value (takes ownership)
void take_ownership(std::unique_ptr<Resource2> res) {
    std::cout << "  Took ownership of: " << res->name() << std::endl;
    // Resource destroyed when function exits
}

// Function taking unique_ptr by rvalue reference (may take ownership)
void maybe_take_ownership(std::unique_ptr<Resource2>&& res) {
    if(res->name().size() > 5) {
        std::cout << "  Taking ownership (long name): " << res->name() << std::endl;
        // res is destroyed when function exits
    }
    else {
        std::cout << "  Not taking ownership (short name): " << res->name() << std::endl;
        // res stays alive, caller keeps ownership
    }
}

void UniquePtrParameters() {
    std::cout << "=== unique_ptr as Parameters ===" << std::endl;

    std::cout << "By value (transfer ownership):" << std::endl;
    auto res = std::make_unique<Resource2>("transferred");
    take_ownership(std::move(res));
    std::cout << "  After call, res is null: " << (res == nullptr) << std::endl;

    std::cout << "\nBy rvalue reference (conditional):" << std::endl;
    auto res2 = std::make_unique<Resource2>("short");
    maybe_take_ownership(std::move(res2));
    std::cout << "  res2 still owns: " << (res2 != nullptr) << std::endl;

    auto res3 = std::make_unique<Resource2>("very_long_name");
    maybe_take_ownership(std::move(res3));
    std::cout << "  res3 is null (ownership taken): " << (res3 == nullptr) << std::endl;
}

// Function returning unique_ptr
std::unique_ptr<Resource2> create_resource(std::string name) {
    return std::make_unique<Resource2>(std::move(name));
}

void UniquePtrReturn() {
    std::cout << "\n=== unique_ptr Return Values ===" << std::endl;

    std::cout << "Factory function pattern:" << std::endl;
    auto res = create_resource("factory_made");
    std::cout << "  Got: " << res->name() << std::endl;

    std::cout << "\nReturn type is part of the contract:" << std::endl;
    std::cout << "  unique_ptr return → caller owns" << std::endl;
    std::cout << "  shared_ptr return → shared ownership" << std::endl;
    std::cout << "  raw ptr/reference → borrowing only" << std::endl;
}

// Smart pointers and std::function
void SmartPtrWithStdFunction() {
    std::cout << "\n=== Smart Pointers with std::function ===" << std::endl;

    std::cout << "Capturing smart pointers in lambdas:" << std::endl;

    auto resource = std::make_unique<Resource2>("captured");

    // Capture by move (lambda owns it)
    auto lambda_owning = [owned = std::move(resource)]() {
        std::cout << "  Lambda owns: " << owned->name() << std::endl;
        };

    lambda_owning();
    std::cout << "  resource is null: " << (resource == nullptr) << std::endl;

    // shared_ptr can be copied into lambda
    auto shared_res = std::make_shared<Resource2>("shared_capture");
    auto lambda_shared = [shared_res]() {
        std::cout << "  Lambda shares: " << shared_res->name() << std::endl;
        std::cout << "  Count in lambda: " << shared_res.use_count() << std::endl;
        };

    lambda_shared();
    std::cout << "  Count after lambda: " << shared_res.use_count() << std::endl;
}

// Callbacks with smart pointers
void CallbacksWithSmartPointers() {
    std::cout << "\n=== Callbacks with Smart Pointers ===" << std::endl;

    std::cout << "Keep objects alive in callbacks:" << std::endl;

    class CallbackHolder {
        std::vector<std::function<void()>> callbacks_;

    public:
        void add_callback(std::function<void()> cb) {
            callbacks_.push_back(std::move(cb));
        }

        void run_all() {
            for(auto& cb : callbacks_) {
                cb();
            }
        }
    };

    CallbackHolder holder;

    {
        auto keep_alive = std::make_shared<Resource2>("callback_resource");

        // Lambda captures shared_ptr, keeps object alive
        holder.add_callback([keep_alive]() {
            std::cout << "  Callback using: " << keep_alive->name() << std::endl;
                            });

        std::cout << "Resource still alive in lambda" << std::endl;
    }

    std::cout << "Running callbacks (resource alive via lambda):" << std::endl;
    holder.run_all();
}