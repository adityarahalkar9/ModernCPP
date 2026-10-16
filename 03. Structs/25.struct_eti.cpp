import std;
#include "structs_header.h"

// Empty Base Optimization (EBO/EIT)
struct Empty {};

// Without EBO
struct WithoutEBO {
    Empty e;   // Takes at least 1 byte!
    int value;
    // sizeof: typically 8 bytes (1 for Empty + 3 padding + 4 for int)
};

// With EBO (inheritance)
struct WithEBO : Empty {  // Empty as base class
    int value;
    // sizeof: typically 4 bytes (base can share storage)
};

// C++20 [[no_unique_address]]
struct WithNoUniqueAddress {
    [[no_unique_address]] Empty e;  // May take zero bytes
    int value;
};

void EmptyBaseOptimization() {
    std::cout << "=== Empty Base Optimization ===" << std::endl;

    std::cout << "sizeof(Empty) = " << sizeof(Empty) << " (must be >= 1)" << std::endl;

    std::cout << "\nWithout EBO (member):" << std::endl;
    std::cout << "sizeof(WithoutEBO) = " << sizeof(WithoutEBO) << std::endl;

    std::cout << "\nWith EBO (inheritance):" << std::endl;
    std::cout << "sizeof(WithEBO) = " << sizeof(WithEBO) << std::endl;

    std::cout << "\nWith [[no_unique_address]] (C++20):" << std::endl;
    std::cout << "sizeof(WithNoUniqueAddress) = " << sizeof(WithNoUniqueAddress) << std::endl;
}

// Practical use: stateless comparators
template <typename T>
struct DefaultComparator {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Container {
    // Comparator might be stateless (empty struct)
    // EBO means it takes no space!
    [[no_unique_address]] Comparator comp_;

public:
    void sort(std::vector<T>& data) {
        std::sort(data.begin(), data.end(), comp_);
    }
};

void StatelessFunctors() {
    std::cout << "\n=== Stateless Functors ===" << std::endl;

    std::cout << "sizeof(DefaultComparator<int>) = "
        << sizeof(DefaultComparator<int>) << std::endl;

    Container<int> c;
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
    c.sort(data);

    std::cout << "Sorted: ";
    for(int i : data) std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\nEBO allows stateless functors to be free:" << std::endl;
    std::cout << "  No space overhead" << std::endl;
    std::cout << "  Can be inlined" << std::endl;
    std::cout << "  Better than function pointers" << std::endl;
}

// Multiple empty bases
struct Empty1 {};
struct Empty2 {};
struct Empty3 {};

struct MultipleEmpty : Empty1, Empty2, Empty3 {
    int data;
};

void MultipleEmptyBases() {
    std::cout << "\n=== Multiple Empty Bases ===" << std::endl;

    std::cout << "sizeof(MultipleEmpty) = " << sizeof(MultipleEmpty) << std::endl;
    std::cout << "(All three empty bases take no space)" << std::endl;

    // Check that addresses are different
    MultipleEmpty me;
    std::cout << "\nAddresses:" << std::endl;
    std::cout << "  &me:       " << &me << std::endl;
    std::cout << "  as Empty1: " << static_cast<Empty1*>(&me) << std::endl;
    std::cout << "  as Empty2: " << static_cast<Empty2*>(&me) << std::endl;
    std::cout << "  as Empty3: " << static_cast<Empty3*>(&me) << std::endl;

    std::cout << "\nEmpty bases can share the same address" << std::endl;
    std::cout << "as long as they're different types" << std::endl;
}

// When EBO doesn't work
struct NonEmptyBase {
    int data = 42;
};

struct DerivedFromNonEmpty : NonEmptyBase {
    // Base has data, can't be optimized away
    double value;
};

void WhenEboDoesntWork() {
    std::cout << "\n=== When EBO Doesn't Work ===" << std::endl;

    std::cout << "EBO only works for:" << std::endl;
    std::cout << "  - Truly empty base classes" << std::endl;
    std::cout << "  - No data members" << std::endl;
    std::cout << "  - No virtual functions" << std::endl;

    std::cout << "\nEBO fails when:" << std::endl;
    std::cout << "  - Base has data members" << std::endl;
    std::cout << "  - First member is same type as base" << std::endl;
    std::cout << "  - Multiple bases of same type at same address" << std::endl;

    std::cout << "\nsizeof(DerivedFromNonEmpty) = "
        << sizeof(DerivedFromNonEmpty) << std::endl;
    std::cout << "(Base has data, so no optimization)" << std::endl;
}

// Modern alternative: [[no_unique_address]]
struct ModernEBO {
    [[no_unique_address]] Empty state;
    int value;
};

void ModernAlternative() {
    std::cout << "\n=== [[no_unique_address]] (C++20) ===" << std::endl;

    std::cout << "sizeof(ModernEBO) = " << sizeof(ModernEBO) << std::endl;

    std::cout << "\n[[no_unique_address]] advantages:" << std::endl;
    std::cout << "  + Works with empty members (not just bases)" << std::endl;
    std::cout << "  + More explicit intent" << std::endl;
    std::cout << "  + No inheritance needed" << std::endl;
    std::cout << "  + Same effect as EBO for empty types" << std::endl;

    std::cout << "\nWhen to use:" << std::endl;
    std::cout << "  - Empty comparator/functor members" << std::endl;
    std::cout << "  - Empty allocator members" << std::endl;
    std::cout << "  - Tag types for dispatch" << std::endl;
    std::cout << "  - Empty policy classes" << std::endl;
}