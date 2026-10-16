import std;
#include "templates_header.h"

// Primary type categories
void PrimaryTypeCategories() {
    std::cout << "=== Primary Type Categories ===" << std::endl;

    std::cout << "std::is_void: " << std::is_void_v<void> << std::endl;
    std::cout << "std::is_integral: " << std::is_integral_v<int> << std::endl;
    std::cout << "std::is_floating_point: " << std::is_floating_point_v<double> << std::endl;
    std::cout << "std::is_array: " << std::is_array_v<int[]> << std::endl;
    std::cout << "std::is_pointer: " << std::is_pointer_v<int*> << std::endl;
    std::cout << "std::is_reference: " << std::is_reference_v<int&> << std::endl;
    std::cout << "std::is_class: " << std::is_class_v<std::string> << std::endl;
}

// Composite type categories
void CompositeTypeCategories() {
    std::cout << "\n=== Composite Type Categories ===" << std::endl;

    std::cout << "std::is_arithmetic: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "std::is_fundamental: " << std::is_fundamental_v<int> << std::endl;
    std::cout << "std::is_object: " << std::is_object_v<int> << std::endl;
    std::cout << "std::is_scalar: " << std::is_scalar_v<int> << std::endl;
    std::cout << "std::is_compound: " << std::is_compound_v<std::string> << std::endl;
}

// Type properties
void TypeProperties() {
    std::cout << "\n=== Type Properties ===" << std::endl;

    std::cout << "std::is_const: " << std::is_const_v<const int> << std::endl;
    std::cout << "std::is_volatile: " << std::is_volatile_v<volatile int> << std::endl;

    std::cout << "\nTriviality:" << std::endl;
    std::cout << "std::is_trivial: " << std::is_trivial_v<int> << std::endl;
    std::cout << "std::is_trivially_copyable: "
        << std::is_trivially_copyable_v<int> << std::endl;

    std::cout << "\nStandard layout:" << std::endl;
    std::cout << "std::is_standard_layout: "
        << std::is_standard_layout_v<int> << std::endl;

    std::cout << "\nPOD (deprecated): " << std::endl;
    std::cout << "std::is_pod: " << std::is_pod_v<int> << std::endl;
}

// Type features (constructibility, etc.)
void TypeFeatures() {
    std::cout << "\n=== Type Features ===" << std::endl;

    std::cout << "Default constructible: "
        << std::is_default_constructible_v<int> << std::endl;
    std::cout << "Copy constructible: "
        << std::is_copy_constructible_v<int> << std::endl;
    std::cout << "Move constructible: "
        << std::is_move_constructible_v<int> << std::endl;

    std::cout << "\nAssignable:" << std::endl;
    std::cout << "Copy assignable: "
        << std::is_copy_assignable_v<int> << std::endl;
    std::cout << "Move assignable: "
        << std::is_move_assignable_v<int> << std::endl;

    std::cout << "\nDestructible: "
        << std::is_destructible_v<int> << std::endl;
}

// Type relationships
void TypeRelationships() {
    std::cout << "\n=== Type Relationships ===" << std::endl;

    class Base {};
    class Derived : public Base {};

    std::cout << "std::is_same: "
        << std::is_same_v<int, int32_t> << std::endl;
    std::cout << "std::is_base_of: "
        << std::is_base_of_v<Base, Derived> << std::endl;
    std::cout << "std::is_convertible: "
        << std::is_convertible_v<Derived*, Base*> << std::endl;
}

// Type transformations
void TypeTransformations() {
    std::cout << "\n=== Type Transformations ===" << std::endl;

    std::cout << "std::remove_const:" << std::endl;
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);

    std::cout << "std::remove_reference:" << std::endl;
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);

    std::cout << "std::remove_pointer:" << std::endl;
    static_assert(std::is_same_v<std::remove_pointer_t<int*>, int>);

    std::cout << "std::add_const:" << std::endl;
    static_assert(std::is_same_v<std::add_const_t<int>, const int>);

    std::cout << "std::decay:" << std::endl;
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
}

// Building custom type traits
template <typename T>
struct IsContainer : std::false_type {};

template <typename T>
struct IsContainer<std::vector<T>> : std::true_type {};

template <typename T>
struct IsContainer<std::list<T>> : std::true_type {};

void CustomTypeTraits() {
    std::cout << "\n=== Custom Type Traits ===" << std::endl;

    std::cout << "IsContainer<vector<int>>: "
        << IsContainer<std::vector<int>>::value << std::endl;
    std::cout << "IsContainer<list<double>>: "
        << IsContainer<std::list<double>>::value << std::endl;
    std::cout << "IsContainer<int>: "
        << IsContainer<int>::value << std::endl;
}

// Using type traits in templates
template <typename T>
void ProcessType(T value) {
    if constexpr(std::is_integral_v<T>) {
        std::cout << "  Integral: " << value << std::endl;
    }
    else if constexpr(std::is_floating_point_v<T>) {
        std::cout << "  Floating: " << value << std::endl;
    }
    else if constexpr(std::is_same_v<T, const char*>) {
        std::cout << "  String: " << value << std::endl;
    }
    else {
        std::cout << "  Other type" << std::endl;
    }
}

void UsingTypeTraitsInTemplates() {
    std::cout << "\n=== Using Traits in Templates ===" << std::endl;

    ProcessType(42);
    ProcessType(3.14);
    ProcessType("hello");
    ProcessType(std::vector<int>{});
}