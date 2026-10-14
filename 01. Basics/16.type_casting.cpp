import std;
#include "basic_header.h"

// Static cast: compile-time checked conversion
void StaticCastDemo(){
    // Numeric conversions
    int i = 42;
    double d = static_cast<double>(i);
    std::cout << "int to double: " << d << std::endl;

    char c = static_cast<char>(65);     // 'A'
    std::cout << "int to char: " << c << std::endl;

    // Void pointer recovery
    void* voidPtr = &i;
    int* intPtr = static_cast<int*>(voidPtr);
    std::cout << "Recovered value: " << *intPtr << std::endl;

    // Upcast (derived to base)
    // Downcast (base to derived) - dangerous but sometimes necessary
    // Note: static_cast doesn't check at runtime
}


// Dynamic cast: runtime checked conversion (requires polymorphic types)
class Base{
public:
    virtual ~Base() = default;
    virtual void describe() const{
        std::cout << "I am Base" << std::endl;
    }
};
class Derived : public Base {
public:
    void describe() const override{
        std::cout << "I am Derived" << std::endl;
    }
    void specialFunction() const {
        std::cout << "Derived special function" << std::endl;
    }
};
class OtherDerived : public Base{
public:
    void describe() const override{
        std::cout << "I am OtherDerived" << std::endl;
    }
};
void DynamicCastDemo(){
    Base* basePtr = new Derived();

    // Safe downcast
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    if(derivedPtr) {
        std::cout << "Downcast successful!" << std::endl;
        derivedPtr->specialFunction();
    }

    // Failed downcast returns nullptr
    OtherDerived* otherPtr = dynamic_cast<OtherDerived*>(basePtr);
    if(!otherPtr) {
        std::cout << "Downcast to OtherDerived failed (returns nullptr)" << std::endl;
    }

    // Dynamic cast with references throws std::bad_cast
    try{
        Base& baseRef = *basePtr;
        OtherDerived& otherRef = dynamic_cast<OtherDerived&>(baseRef);
    }
    catch(const std::bad_cast& e){
        std::cout << "Caught bad_cast: " << e.what() << std::endl;
    }

    delete basePtr;
}


// Const cast: add/remove const
void ConstCastDemo(){
    const int constValue = 42;

    // Removing const (undefined behavior if original is truly const)
    int* mutablePtr = const_cast<int*>(&constValue);
    std::cout << "Value through const_cast: " << *mutablePtr << std::endl;

    // Legitimate use: calling legacy API that doesn't declare const correctly
    auto legacyApi = [](char* str){
            std::cout << "Legacy API called with: " << str << std::endl;
        };

    const char* constStr = "Hello";
    // legacyApi(constStr);                     // Error: cannot convert const char* to char*
    legacyApi(const_cast<char*>(constStr));     // OK (if we know it doesn't modify)
}


/*
    // Reinterpret cast: bit-level reinterpretation
    void ReinterpretCastDemo(){
        int i = 42;
        // Reinterpret int pointer as char pointer
        char* charPtr = reinterpret_cast<char*>(&i);

        std::cout << "Bytes of 42: ";
        for(size_t j = 0; j < sizeof(int); ++j) {
            std::cout << static_cast<int>(static_cast<unsigned char>(charPtr[j])) << " ";
        }
        std::cout << std::endl;

        // Function pointer conversion (very dangerous, use with caution)
        // auto fp = reinterpret_cast<void(*)(int)>(someFunctionPointer);

        // Pointer to integer (for hashing/address manipulation)
        uintptr_t addr = reinterpret_cast<uintptr_t>(&i);
        std::cout << "Address as integer: " << addr << std::endl;
    }
*/


// C-style cast (avoid in modern C++)
void CStyleCastDemo() {
    int i = 42;

    // C-style cast tries (in order):
    // 1. const_cast
    // 2. static_cast
    // 3. reinterpret_cast
    // 4. combination of the above

    double d = (double)i;  // C-style cast
    std::cout << "C-style cast result: " << d << std::endl;

    // Modern C++ prefers explicit named casts for clarity
    double d2 = static_cast<double>(i);
    std::cout << "static_cast result: " << d2 << std::endl;

    // Warning: C-style casts can accidentally remove const or do
    // reinterpret_cast without making it obvious
}


// Implicit conversions vs explicit casts
void ImplicitVsExplicit(){
    int i = 42;
    long l = i;           // Implicit conversion (widening)
    double d = i;         // Implicit conversion (widening)

    std::cout << "Implicit: long = " << l << ", double = " << d << std::endl;

    // Narrowing conversions (potential data loss)
    double bigD = 3.14159265358979;
    int truncated = bigD;  // Implicit, but loses precision
    std::cout << "Truncated: " << truncated << std::endl;

    // Use brace initialization to catch narrowing at compile time
    // int bad = {bigD};  // Error: narrowing conversion

    // Explicit narrowing is clearer
    int explicitTrunc = static_cast<int>(bigD);
    std::cout << "Explicit truncation: " << explicitTrunc << std::endl;
}


// Type casting with smart pointers
void SmartPointerCasts(){
    std::shared_ptr<Base> basePtr = std::make_shared<Derived>();

    // static_pointer_cast for static_cast on shared_ptr
    auto derivedPtr = std::static_pointer_cast<Derived>(basePtr);
    derivedPtr->specialFunction();

    // dynamic_pointer_cast for dynamic_cast on shared_ptr
    auto dynDerived = std::dynamic_pointer_cast<Derived>(basePtr);
    if(dynDerived) {
        std::cout << "dynamic_pointer_cast successful" << std::endl;
    }

    // const_pointer_cast for const_cast on shared_ptr
    std::shared_ptr<const Base> constBasePtr = basePtr;
    auto mutableBase = std::const_pointer_cast<Base>(constBasePtr);
    std::cout << "const_pointer_cast successful" << std::endl;

    // reinterpret_pointer_cast (C++17) for reinterpret_cast on shared_ptr
    // auto reinterpreted = std::reinterpret_pointer_cast<OtherDerived>(basePtr);
}