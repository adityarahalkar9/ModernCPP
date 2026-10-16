import std;
#include "structs_header.h"

// Test structures
struct SimpleStruct {
    int a;
    double b;
};

struct StructWithConstructor {
    StructWithConstructor() {}
    int a;
};

struct StructWithVirtual {
    virtual void foo() {}
    int a;
};

struct StructWithPrivate {
private:
    int a;
public:
    double b;
};

struct EmptyStruct {};

void BasicTypeTraits() {
    std::cout << "=== Type Traits for Structs ===" << std::endl;

    std::cout << "SimpleStruct:" << std::endl;
    std::cout << "  is_aggregate: " << std::is_aggregate_v<SimpleStruct> << std::endl;
    std::cout << "  is_trivial: " << std::is_trivial_v<SimpleStruct> << std::endl;
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<SimpleStruct> << std::endl;
    std::cout << "  is_pod: " << std::is_pod_v<SimpleStruct> << std::endl;

    std::cout << "\nStructWithConstructor:" << std::endl;
    std::cout << "  is_aggregate: " << std::is_aggregate_v<StructWithConstructor> << std::endl;
    std::cout << "  is_trivial: " << std::is_trivial_v<StructWithConstructor> << std::endl;

    std::cout << "\nStructWithVirtual:" << std::endl;
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<StructWithVirtual> << std::endl;
    std::cout << "  is_pod: " << std::is_pod_v<StructWithVirtual> << std::endl;

    std::cout << "\nStructWithPrivate:" << std::endl;
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<StructWithPrivate> << std::endl;
}

// sizeof and alignment traits
void SizeAndAlignmentTraits() {
    std::cout << "\n=== Size and Alignment ===" << std::endl;

    std::cout << "SimpleStruct:" << std::endl;
    std::cout << "  sizeof: " << sizeof(SimpleStruct) << std::endl;
    std::cout << "  alignof: " << alignof(SimpleStruct) << std::endl;

    std::cout << "\nEmptyStruct:" << std::endl;
    std::cout << "  sizeof: " << sizeof(EmptyStruct) << std::endl;
    std::cout << "  (must be at least 1)" << std::endl;

    std::cout << "\nStructWithVirtual:" << std::endl;
    std::cout << "  sizeof: " << sizeof(StructWithVirtual) << std::endl;
    std::cout << "  (includes vtable pointer)" << std::endl;
}

// Using traits in templates
template <typename T>
void AnalyzeType() {
    std::cout << "\nAnalyzing " << typeid(T).name() << ":" << std::endl;

    if constexpr(std::is_aggregate_v<T>) {
        std::cout << "  - Is aggregate (can use {} initialization)" << std::endl;
    }
    else {
        std::cout << "  - NOT aggregate" << std::endl;
    }

    if constexpr(std::is_trivially_copyable_v<T>) {
        std::cout << "  - Is trivially copyable (can memcpy)" << std::endl;
    }
    else {
        std::cout << "  - NOT trivially copyable" << std::endl;
    }

    if constexpr(std::is_standard_layout_v<T>) {
        std::cout << "  - Has standard layout (C compatible)" << std::endl;
    }
    else {
        std::cout << "  - Does NOT have standard layout" << std::endl;
    }

    if constexpr(std::is_empty_v<T>) {
        std::cout << "  - Is empty (no data members)" << std::endl;
    }
}

void UsingTraitsInTemplates() {
    std::cout << "\n=== Using Traits in Templates ===" << std::endl;

    AnalyzeType<SimpleStruct>();
    AnalyzeType<StructWithVirtual>();
    AnalyzeType<EmptyStruct>();
}

// Compile-time checks
void CompileTimeChecks() {
    std::cout << "\n=== Compile-Time Checks ===" << std::endl;

    // static_assert with traits
    static_assert(std::is_aggregate_v<SimpleStruct>,
                  "SimpleStruct must be aggregate");

    static_assert(std::is_trivially_copyable_v<SimpleStruct>,
                  "SimpleStruct must be trivially copyable");

    static_assert(std::is_standard_layout_v<SimpleStruct>,
                  "SimpleStruct must have standard layout");

    std::cout << "All compile-time checks passed!" << std::endl;

    std::cout << "\nUse static_assert to enforce design requirements:" << std::endl;
    std::cout << "  - Ensure types are POD for serialization" << std::endl;
    std::cout << "  - Ensure C compatibility" << std::endl;
    std::cout << "  - Document type requirements" << std::endl;
}

// Conditional compilation based on traits
template <typename T>
void SerializeImpl(const T& obj) {
    if constexpr(std::is_trivially_copyable_v<T>) {
        std::cout << "Fast path: memcpy " << sizeof(T) << " bytes" << std::endl;
    }
    else {
        std::cout << "Slow path: member-by-member serialization" << std::endl;
    }
}

void ConditionalCompilation() {
    std::cout << "\n=== Conditional Code Based on Traits ===" << std::endl;

    SimpleStruct s{1, 2.0};
    StructWithConstructor sc;

    std::cout << "Serializing SimpleStruct:" << std::endl;
    SerializeImpl(s);

    std::cout << "\nSerializing StructWithConstructor:" << std::endl;
    SerializeImpl(sc);

    std::cout << "\nif constexpr chooses code path at compile time" << std::endl;
    std::cout << "No runtime overhead for the check" << std::endl;
}