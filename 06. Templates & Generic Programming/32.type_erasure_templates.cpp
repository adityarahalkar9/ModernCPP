import std;
#include "templates_header.h"

// Type erasure with templates
class AnyPrintable {
    struct Concept {
        virtual void print() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
        virtual ~Concept() = default;
    };

    template <typename T>
    struct Model : Concept {
        T data_;

        explicit Model(T d) : data_(std::move(d)) {}

        void print() const override {
            std::cout << "  " << data_ << std::endl;
        }

        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(data_);
        }
    };

    std::unique_ptr<Concept> ptr_;

public:
    template <typename T>
    AnyPrintable(T obj)
        : ptr_(std::make_unique<Model<T>>(std::move(obj))) {
    }

    AnyPrintable(const AnyPrintable& other)
        : ptr_(other.ptr_->clone()) {
    }

    AnyPrintable& operator=(const AnyPrintable& other) {
        if(this != &other) {
            ptr_ = other.ptr_->clone();
        }
        return *this;
    }

    void print() const {
        ptr_->print();
    }
};

void BasicTypeErasure() {
    std::cout << "=== Type Erasure with Templates ===" << std::endl;

    std::cout << "Store different types in one container:" << std::endl;
    std::cout << "  No common base class needed" << std::endl;

    std::vector<AnyPrintable> items;
    items.push_back(42);
    items.push_back(3.14);
    items.push_back(std::string("hello"));

    for(const auto& item : items) {
        item.print();
    }
}

// Type erasure with multiple operations
template <typename... Ops>
class MultiOp {
    // Storage for erased type
    struct Concept {
        virtual ~Concept() = default;
    };

    template <typename T>
    struct Model : Concept {
        T data_;

        explicit Model(T d) : data_(std::move(d)) {}
    };

    std::shared_ptr<const Concept> ptr_;

public:
    template <typename T>
    MultiOp(T obj)
        : ptr_(std::make_shared<Model<T>>(std::move(obj))) {
    }

    template <typename Op>
    auto apply() const {
        // Apply operation to stored value
        return Op{}(static_cast<const Model<void>*>(ptr_.get()));
    }
};

void AdvancedTypeErasure() {
    std::cout << "\n=== Advanced Type Erasure ===" << std::endl;

    std::cout << "Multiple operations on erased types:" << std::endl;
    std::cout << "  Combination of virtual interface" << std::endl;
    std::cout << "  and template flexibility" << std::endl;
}

// std::function as type erasure
void StdFunctionTypeErasure() {
    std::cout << "\n=== std::function Type Erasure ===" << std::endl;

    std::cout << "std::function erases callable type:" << std::endl;

    std::function<int(int)> f1 = [](int x) { return x * 2; };
    std::function<int(int)> f2 = [](int x) { return x + 10; };

    std::cout << "  f1(5) = " << f1(5) << std::endl;
    std::cout << "  f2(5) = " << f2(5) << std::endl;

    // Can store function pointers, lambdas, functors
    int (*func_ptr)(int) = [](int x) { return x * 3; };
    std::function<int(int)> f3 = func_ptr;
    std::cout << "  f3(5) = " << f3(5) << std::endl;
}

// Comparison with virtual functions
void TypeErasureVsVirtual() {
    std::cout << "\n=== Type Erasure vs Virtual ===" << std::endl;

    std::cout << "Type Erasure:" << std::endl;
    std::cout << "  + Value semantics (copyable)" << std::endl;
    std::cout << "  + No inheritance required" << std::endl;
    std::cout << "  + Template-friendly" << std::endl;
    std::cout << "  - More complex implementation" << std::endl;
    std::cout << std::endl;

    std::cout << "Virtual Functions:" << std::endl;
    std::cout << "  + Simpler to implement" << std::endl;
    std::cout << "  + Runtime polymorphism" << std::endl;
    std::cout << "  - Requires inheritance" << std::endl;
    std::cout << "  - Reference semantics usually" << std::endl;
}