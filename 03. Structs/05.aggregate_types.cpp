#include <iostream>
#include <string>
#include <type_traits>
#include "structs_header.h"

// Aggregate types
struct Aggregate1 {
    int a;
    double b;
    std::string c;
    // No user-provided constructors
    // No private/protected members
    // No virtual functions
    // No base classes (until C++17)
};

struct NotAggregate1 {
    NotAggregate1(int x) : a(x) {}  // User-provided constructor!
    int a;
};

struct NotAggregate2 {
private:
    int a;  // Private member
};

struct NotAggregate3 {
    virtual void foo() {}  // Virtual function
    int a;
};

// C++17: aggregates can have base classes
struct Base { int base_val; };
struct DerivedAggregate : Base {  // OK in C++17
    int derived_val;
};

void AggregateBasics() {
    std::cout << "=== Aggregate Types ===" << std::endl;

    static_assert(std::is_aggregate_v<Aggregate1>);
    static_assert(!std::is_aggregate_v<NotAggregate1>);
    static_assert(!std::is_aggregate_v<NotAggregate2>);
    static_assert(!std::is_aggregate_v<NotAggregate3>);

    std::cout << "Aggregate requirements:" << std::endl;
    std::cout << "  1. No user-provided constructors" << std::endl;
    std::cout << "  2. No private/protected non-static data members" << std::endl;
    std::cout << "  3. No virtual functions" << std::endl;
    std::cout << "  4. (C++17) Can have base classes (must not be virtual)" << std::endl;

    // Aggregate initialization
    Aggregate1 agg{42, 3.14, "hello"};
    std::cout << "\nagg: a=" << agg.a << ", b=" << agg.b
        << ", c=" << agg.c << std::endl;
}

void AggregateWithBase() {
    std::cout << "\n=== Aggregates with Base Classes (C++17) ===" << std::endl;

    static_assert(std::is_aggregate_v<DerivedAggregate>);

    // Initialize including base class members
    DerivedAggregate da{{100}, 200};  // {base_val}, derived_val

    std::cout << "da.base_val = " << da.base_val << std::endl;
    std::cout << "da.derived_val = " << da.derived_val << std::endl;

    // Designated initializers (C++20) with base classes
    // Note: Can't use designators for base class members
    DerivedAggregate da2{{1}, 2};
    std::cout << "\nda2: base=" << da2.base_val << ", derived=" << da2.derived_val << std::endl;
}

// Arrays are aggregates
void ArrayAggregates() {
    std::cout << "\n=== Array Aggregates ===" << std::endl;

    // Built-in arrays are aggregates
    int arr[] = {1, 2, 3, 4, 5};
    std::cout << "sizeof(arr) = " << sizeof(arr) << std::endl;

    // Nested aggregates
    Aggregate1 nested[] = {
        {1, 1.1, "one"},
        {2, 2.2, "two"},
        {3, 3.3, "three"}
    };

    std::cout << "\nNested aggregate array:" << std::endl;
    for(const auto& a : nested) {
        std::cout << "  " << a.a << " " << a.b << " " << a.c << std::endl;
    }
}

// Losing aggregate status
struct LostAggregate {
    int a;
    // LostAggregate() = default;  // This is OK - not "user-provided"
    LostAggregate(int x) : a(x) {}  // This makes it NOT aggregate
};

void LosingAggregateStatus() {
    std::cout << "\n=== Losing Aggregate Status ===" << std::endl;

    std::cout << "Things that make a struct NOT an aggregate:" << std::endl;
    std::cout << "  1. Any user-provided constructor (even private)" << std::endl;
    std::cout << "  2. Private/protected non-static data members" << std::endl;
    std::cout << "  3. Virtual functions" << std::endl;
    std::cout << "  4. Virtual base classes" << std::endl;

    std::cout << "\nThings that DON'T affect aggregate status:" << std::endl;
    std::cout << "  - Defaulted constructors/destructors" << std::endl;
    std::cout << "  - Static members" << std::endl;
    std::cout << "  - Member functions" << std::endl;
    std::cout << "  - Private member functions" << std::endl;
}

// Practical implications
void PracticalImplications() {
    std::cout << "\n=== Practical Implications ===" << std::endl;

    std::cout << "Aggregates support:" << std::endl;
    std::cout << "  - Brace initialization: T obj{args...}" << std::endl;
    std::cout << "  - Designated initializers (C++20): T obj{.field = val}" << std::endl;
    std::cout << "  - No need for constructors" << std::endl;

    std::cout << "\nNon-aggregates require:" << std::endl;
    std::cout << "  - Constructor call: T obj(args...)" << std::endl;
    std::cout << "  - Cannot use designated initializers" << std::endl;

    std::cout << "\nDesign tip:" << std::endl;
    std::cout << "  Keep data-only structs as aggregates" << std::endl;
    std::cout << "  Use classes for types with invariants" << std::endl;
}