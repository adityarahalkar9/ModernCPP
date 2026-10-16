import std;
#include "smart_pointers_header.h"

class Comparable {
public:
    explicit Comparable(int value) : value_(value) {}
    int value() const { return value_; }

    bool operator==(const Comparable& other) const {
        return value_ == other.value_;
    }

    bool operator<(const Comparable& other) const {
        return value_ < other.value_;
    }

private:
    int value_;
};

void ComparingPointerIdentity() {
    std::cout << "=== Comparing Pointers (identity) ===" << std::endl;

    auto p1 = std::make_shared<Comparable>(42);
    auto p2 = p1;  // Same object
    auto p3 = std::make_shared<Comparable>(42);  // Different object, same value

    std::cout << "p1 == p2 (same object): " << (p1 == p2) << std::endl;
    std::cout << "p1 == p3 (different objects): " << (p1 == p3) << std::endl;

    std::cout << "\nSmart pointer comparison compares POINTERS" << std::endl;
    std::cout << "Not the pointed-to values" << std::endl;
}

void ComparingPointedToValues() {
    std::cout << "\n=== Comparing Values ===" << std::endl;

    auto p1 = std::make_shared<Comparable>(42);
    auto p2 = std::make_shared<Comparable>(42);

    // Pointer comparison (identity)
    std::cout << "Same object: " << (p1 == p2) << std::endl;

    // Value comparison (dereference)
    std::cout << "Same value: " << (*p1 == *p2) << std::endl;

    // Using member access
    std::cout << "p1->value() == p2->value(): "
        << (p1->value() == p2->value()) << std::endl;
}

void SmartPointersInMap() {
    std::cout << "\n=== Smart Pointers in map ===" << std::endl;

    // Map with shared_ptr keys (compares by pointer value)
    std::map<std::shared_ptr<Comparable>, std::string> ptr_map;

    auto key1 = std::make_shared<Comparable>(1);
    auto key2 = std::make_shared<Comparable>(2);

    ptr_map[key1] = "first";
    ptr_map[key2] = "second";

    std::cout << "Map with pointer keys (identity):" << std::endl;
    for(const auto& [key, value] : ptr_map) {
        std::cout << "  " << key.get() << " -> " << value << std::endl;
    }

    // Map comparing pointed-to values
    struct ValueComparator {
        bool operator()(const std::shared_ptr<Comparable>& a,
                        const std::shared_ptr<Comparable>& b) const {
            return *a < *b;  // Compare values, not pointers
        }
    };

    std::map<std::shared_ptr<Comparable>, std::string, ValueComparator> value_map;
    value_map[std::make_shared<Comparable>(10)] = "ten";
    value_map[std::make_shared<Comparable>(5)] = "five";
    value_map[std::make_shared<Comparable>(10)] = "ten (replaced)";  // Same value!

    std::cout << "\nMap with value comparison:" << std::endl;
    for(const auto& [key, value] : value_map) {
        std::cout << "  " << key->value() << " -> " << value << std::endl;
    }
}

void SmartPointersInSet() {
    std::cout << "\n=== Smart Pointers in set ===" << std::endl;

    // Set compares by pointer
    std::set<std::shared_ptr<Comparable>> ptr_set;

    auto item1 = std::make_shared<Comparable>(1);
    auto item2 = std::make_shared<Comparable>(2);
    auto item1_again = item1;  // Same pointer

    ptr_set.insert(item1);
    ptr_set.insert(item2);
    ptr_set.insert(item1_again);  // Won't insert (same pointer)

    std::cout << "Set size (pointer identity): " << ptr_set.size() << std::endl;

    // Set comparing values
    struct ValueLess {
        bool operator()(const std::shared_ptr<Comparable>& a,
                        const std::shared_ptr<Comparable>& b) const {
            return *a < *b;
        }
    };

    std::set<std::shared_ptr<Comparable>, ValueLess> value_set;
    value_set.insert(std::make_shared<Comparable>(10));
    value_set.insert(std::make_shared<Comparable>(10));  // Same value, deduplicated!
    value_set.insert(std::make_shared<Comparable>(20));

    std::cout << "Set size (value comparison): " << value_set.size() << std::endl;
}

void HashingSmartPointers() {
    std::cout << "\n=== Hashing Smart Pointers ===" std::endl;

    // std::hash is specialized for smart pointers
    auto ptr = std::make_shared<Comparable>(42);

    // Hash of pointer (identity-based)
    auto ptr_hash = std::hash<std::shared_ptr<Comparable>>{}(ptr);
    std::cout << "  Pointer hash: " << ptr_hash << std::endl;

    // Same pointer, same hash
    auto same_ptr = ptr;
    auto same_hash = std::hash<std::shared_ptr<Comparable>>{}(same_ptr);
    std::cout << "  Same pointer hash: " << same_hash << " (equal)" << std::endl;

    // Different pointer, different hash (even if same value)
    auto other_ptr = std::make_shared<Comparable>(42);
    auto other_hash = std::hash<std::shared_ptr<Comparable>>{}(other_ptr);
    std::cout << "  Different pointer hash: " << other_hash << " (different)" << std::endl;

    std::cout << "\nstd::hash for smart pointers:" << std::endl;
    std::cout << "  Hashes the POINTER value" << std::endl;
    std::cout << "  Same object → same hash" << std::endl;
    std::cout << "  Different object → different hash" << std::endl;
}