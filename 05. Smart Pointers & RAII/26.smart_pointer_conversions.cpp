import std;
#include "smart_pointers_header.h"

class Base3 {
public:
    virtual ~Base3() = default;
    virtual void describe() const {
        std::cout << "  Base3" << std::endl;
    }
};

class Derived4 : public Base3 {
public:
    void describe() const override {
        std::cout << "  Derived4" << std::endl;
    }

    void derived_only() const {
        std::cout << "  Derived4-specific method" << std::endl;
    }
};

void UniqueToShared() {
    std::cout << "=== unique_ptr to shared_ptr ===" << std::endl;

    std::cout << "Converting (one-way):" << std::endl;

    auto unique = std::make_unique<Derived4>();

    // Convert (unique must be moved)
    std::shared_ptr<Derived4> shared = std::move(unique);

    std::cout << "  unique is null: " << (unique == nullptr) << std::endl;
    std::cout << "  shared valid: " << (shared != nullptr) << std::endl;

    std::cout << "\nCannot convert back!" << std::endl;
    std::cout << "  shared_ptr → unique_ptr is not possible" << std::endl;
    std::cout << "  (Multiple owners may exist)" << std::endl;
}

void StaticPointerCast() {
    std::cout << "\n=== static_pointer_cast ===" << std::endl;

    auto derived = std::make_shared<Derived4>();

    // Upcast to base
    std::shared_ptr<Base3> base = std::static_pointer_cast<Base3>(derived);

    base->describe();  // Calls Derived4::describe (virtual)

    // Downcast back to derived (no runtime check!)
    std::shared_ptr<Derived4> back =
        std::static_pointer_cast<Derived4>(base);

    back->derived_only();
    std::cout << "  static_cast: no runtime check" << std::endl;
}

void DynamicPointerCast() {
    std::cout << "\n=== dynamic_pointer_cast ===" << std::endl;

    std::shared_ptr<Base3> base = std::make_shared<Derived4>();

    // Try to cast to derived
    auto derived = std::dynamic_pointer_cast<Derived4>(base);
    if(derived) {
        std::cout << "  Cast to Derived4 succeeded" << std::endl;
        derived->derived_only();
    }

    // Try to cast to unrelated type (will fail)
    class Unrelated : public Base3 {};
    auto unrelated = std::dynamic_pointer_cast<Unrelated>(base);
    if(!unrelated) {
        std::cout << "  Cast to Unrelated failed (null)" << std::endl;
    }
}

void ConstPointerCast() {
    std::cout << "\n=== const_pointer_cast ===" << std::endl;

    auto mutable_ptr = std::make_shared<int>(42);

    // Add const
    std::shared_ptr<const int> const_ptr = mutable_ptr;

    std::cout << "  Value: " << *const_ptr << std::endl;

    // Remove const (dangerous if originally const!)
    auto mutable_again = std::const_pointer_cast<int>(const_ptr);
    *mutable_again = 100;

    std::cout << "  After modification: " << *mutable_ptr << std::endl;
    std::cout << "  (Same object, different pointers)" << std::endl;
}

void ReinterpretPointerCast() {
    std::cout << "\n=== reinterpret_pointer_cast ===" << std::endl;

    std::cout << "reinterpret_pointer_cast (C++17):" << std::endl;
    std::cout << "  Dangerous, use with extreme caution" << std::endl;

    // Example: treating memory as bytes
    auto value = std::make_shared<int>(0x41424344);

    // Note: This is implementation-defined behavior
    // auto bytes = std::reinterpret_pointer_cast<unsigned char>(value);

    std::cout << "  Use only for bit-level inspection" << std::endl;
    std::cout << "  Prefer std::bit_cast (C++20) for values" << std::endl;
}

void ConversionBestPractices() {
    std::cout << "\n=== Best Practices ===" << std::endl;

    std::cout << "Conversion hierarchy (safest to most dangerous):" << std::endl;
    std::cout << "  1. dynamic_pointer_cast (runtime checked)" << std::endl;
    std::cout << "  2. static_pointer_cast (compile-time checked)" << std::endl;
    std::cout << "  3. const_pointer_cast (removes protection)" << std::endl;
    std::cout << "  4. reinterpret_pointer_cast (no checks!)" << std::endl;

    std::cout << "\nRules:" << std::endl;
    std::cout << "  Use dynamic_cast when unsure of type" << std::endl;
    std::cout << "  Use static_cast when type is guaranteed" << std::endl;
    std::cout << "  Avoid reinterpret_cast unless absolutely necessary" << std::endl;
    std::cout << std::endl;

    std::cout << "unique_ptr → shared_ptr: OK (one-way)" << std::endl;
    std::cout << "shared_ptr → unique_ptr: NEVER possible" << std::endl;
}