import std;
#include "smart_pointers_header.h"

void PointerTraitsBasics() {
    std::cout << "=== std::pointer_traits ===" << std::endl;

    std::cout << "std::pointer_traits provides:" << std::endl;
    std::cout << "  Uniform interface for pointer-like types" << std::endl;
    std::cout << "  Works with raw and smart pointers" << std::endl;

    // For raw pointers
    using RawTraits = std::pointer_traits<int*>;
    std::cout << "\nRaw pointer traits:" << std::endl;
    std::cout << "  element_type: " << typeid(RawTraits::element_type).name() << std::endl;
    std::cout << "  pointer: " << typeid(RawTraits::pointer).name() << std::endl;

    // For smart pointers
    using UniqueTraits = std::pointer_traits<std::unique_ptr<int>>;
    std::cout << "\nunique_ptr traits:" << std::endl;
    std::cout << "  element_type: " << typeid(UniqueTraits::element_type).name() << std::endl;
    std::cout << "  pointer: " << typeid(UniqueTraits::pointer).name() << std::endl;

    using SharedTraits = std::pointer_traits<std::shared_ptr<int>>;
    std::cout << "\nshared_ptr traits:" << std::endl;
    std::cout << "  element_type: " << typeid(SharedTraits::element_type).name() << std::endl;
}

void TypeTraitsForSmartPointers() {
    std::cout << "\n=== Type Traits for Smart Pointers ===" << std::endl;

    std::cout << "Checking smart pointer properties:" << std::endl;

    std::cout << "\nunique_ptr<int>:" << std::endl;
    std::cout << "  is_default_constructible: "
        << std::is_default_constructible_v<std::unique_ptr<int>> << std::endl;
    std::cout << "  is_copy_constructible: "
        << std::is_copy_constructible_v<std::unique_ptr<int>> << std::endl;
    std::cout << "  is_move_constructible: "
        << std::is_move_constructible_v<std::unique_ptr<int>> << std::endl;
    std::cout << "  is_swappable: "
        << std::is_swappable_v<std::unique_ptr<int>> << std::endl;

    std::cout << "\nshared_ptr<int>:" << std::endl;
    std::cout << "  is_default_constructible: "
        << std::is_default_constructible_v<std::shared_ptr<int>> << std::endl;
    std::cout << "  is_copy_constructible: "
        << std::is_copy_constructible_v<std::shared_ptr<int>> << std::endl;
    std::cout << "  is_move_constructible: "
        << std::is_move_constructible_v<std::shared_ptr<int>> << std::endl;
}

void DetectingSmartPointer() {
    std::cout << "\n=== Detecting Smart Pointers ===" << std::endl;

    // Custom trait to detect if T is a smart pointer
    template <typename T>
    struct is_smart_pointer : std::false_type {};

    template <typename T>
    struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

    template <typename T>
    struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

    template <typename T>
    struct is_smart_pointer<std::weak_ptr<T>> : std::true_type {};

    static_assert(is_smart_pointer<std::unique_ptr<int>>::value);
    static_assert(is_smart_pointer<std::shared_ptr<int>>::value);
    static_assert(!is_smart_pointer<int*>::value);

    std::cout << "Custom detection trait:" << std::endl;
    std::cout << "  is_smart_pointer<unique_ptr<int>>: "
        << is_smart_pointer<std::unique_ptr<int>>::value << std::endl;
    std::cout << "  is_smart_pointer<int*>: "
        << is_smart_pointer<int*>::value << std::endl;
}

void GenericCodeWithSmartPointers() {
    std::cout << "\n=== Generic Code with Smart Pointers ===" << std::endl;

    // Generic function that works with any pointer-like type
    template <typename Ptr>
    auto get_value(const Ptr & ptr) -> decltype(*ptr) {
        return *ptr;
    }

    auto unique = std::make_unique<int>(42);
    auto shared = std::make_shared<int>(100);
    int raw = 7;
    int* raw_ptr = &raw;

    std::cout << "Generic value extraction:" << std::endl;
    std::cout << "  unique_ptr: " << get_value(unique) << std::endl;
    std::cout << "  shared_ptr: " << get_value(shared) << std::endl;
    std::cout << "  raw ptr:    " << get_value(raw_ptr) << std::endl;
}

void MakingCustomPointerLike() {
    std::cout << "\n=== Custom Pointer-Like Type ===" << std::endl;

    std::cout << "Requirements for pointer-like type:" << std::endl;
    std::cout << "  operator* (dereference)" << std::endl;
    std::cout << "  operator-> (member access)" << std::endl;
    std::cout << "  operator bool (validity check)" << std::endl;
    std::cout << "  Can be compared to nullptr" << std::endl;

    // Example: optional as pointer-like
    std::optional<int> opt = 42;

    if(opt) {
        std::cout << "  optional has value: " << *opt << std::endl;
    }

    std::optional<int> empty;
    if(!empty) {
        std::cout << "  optional is empty" << std::endl;
    }
}