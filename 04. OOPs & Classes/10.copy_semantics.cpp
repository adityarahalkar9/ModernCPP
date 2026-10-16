import std;
#include "class_header.h"

// Class with manual copy operations
class Copyable {
private:
    std::string* data;
    size_t size;

public:
    // Constructor
    Copyable(const std::string& str) : data(new std::string(str)), size(str.length()) {
        std::cout << "  Copyable('" << *data << "') constructed" << std::endl;
    }

    // COPY CONSTRUCTOR
    Copyable(const Copyable& other)
        : data(new std::string(*other.data)), size(other.size) {
        std::cout << "  COPY constructor: deep copy of '" << *data << "'" << std::endl;
    }

    // COPY ASSIGNMENT
    Copyable& operator=(const Copyable& other) {
        if(this != &other) {  // Self-assignment check
            std::cout << "  COPY assignment: '" << *other.data << "' to '" << *data << "'" << std::endl;
            *data = *other.data;
            size = other.size;
        }
        return *this;
    }

    // Destructor
    ~Copyable() {
        std::cout << "  Copyable('" << *data << "') destroyed" << std::endl;
        delete data;
    }

    const std::string& get() const { return *data; }
};

void CopyConstructorDemo() {
    std::cout << "=== Copy Constructor ===" << std::endl;

    std::cout << "Creating original:" << std::endl;
    Copyable original("Hello");

    std::cout << "\nCopy construction:" << std::endl;
    Copyable copy = original;  // Copy constructor

    std::cout << "\nCopy assignment:" << std::endl;
    Copyable assigned("World");
    assigned = original;  // Copy assignment

    std::cout << "\nOriginal: " << original.get() << std::endl;
    std::cout << "Copy:     " << copy.get() << std::endl;
    std::cout << "Assigned: " << assigned.get() << std::endl;
}

// Shallow vs Deep copy
class ShallowCopy {
public:
    int* value;

    ShallowCopy(int v) : value(new int(v)) {}
    ~ShallowCopy() { delete value; }

    // Default copy constructor: SHALLOW copy
    // Both objects point to SAME memory!
};

class DeepCopy {
public:
    int* value;

    DeepCopy(int v) : value(new int(v)) {}
    ~DeepCopy() { delete value; }

    // User-defined: DEEP copy
    DeepCopy(const DeepCopy& other) : value(new int(*other.value)) {}
    DeepCopy& operator=(const DeepCopy& other) {
        if(this != &other) {
            *value = *other.value;
        }
        return *this;
    }
};

void ShallowVsDeepCopy() {
    std::cout << "\n=== Shallow vs Deep Copy ===" << std::endl;

    {
        std::cout << "Shallow copy (DANGEROUS):" << std::endl;
        ShallowCopy a(42);
        ShallowCopy b = a;  // Shallow: b.value == a.value

        std::cout << "  a.value: " << a.value << " -> " << *a.value << std::endl;
        std::cout << "  b.value: " << b.value << " -> " << *b.value << std::endl;
        std::cout << "  Same pointer? " << (a.value == b.value) << std::endl;
        // When both go out of scope: DOUBLE DELETE!
    }

    std::cout << "\nDeep copy (SAFE):" << std::endl;
    {
        DeepCopy a(42);
        DeepCopy b = a;  // Deep: separate memory

        *b.value = 100;  // Doesn't affect a

        std::cout << "  a.value: " << a.value << " -> " << *a.value << std::endl;
        std::cout << "  b.value: " << b.value << " -> " << *b.value << std::endl;
        std::cout << "  Same pointer? " << (a.value == b.value) << std::endl;
        std::cout << "  a is unchanged: " << *a.value << std::endl;
    }
}

// Self-assignment
class SelfAssign {
private:
    std::string* str;

public:
    SelfAssign(const char* s) : str(new std::string(s)) {}
    ~SelfAssign() { delete str; }

    // BAD assignment (without self-check)
    SelfAssign& operator=(const SelfAssign& other) {
        delete str;  // If &other == this, we just deleted our own data!
        str = new std::string(*other.str);  // Now reading freed memory!
        return *this;
    }

    // GOOD assignment
    SelfAssign& safe_assign(const SelfAssign& other) {
        if(this != &other) {
            *str = *other.str;
        }
        return *this;
    }

    const std::string& get() const { return *str; }
};

void SelfAssignment() {
    std::cout << "\n=== Self-Assignment ===" << std::endl;

    SelfAssign a("Hello");

    std::cout << "Before: " << a.get() << std::endl;
    a = a;  // Self-assignment
    std::cout << "After (potentially UB): " << a.get() << std::endl;

    std::cout << "\nRules:" << std::endl;
    std::cout << "  1. Always check for self-assignment" << std::endl;
    std::cout << "  2. Or use copy-and-swap idiom" << std::endl;
    std::cout << "  3. Or use value semantics where possible" << std::endl;
}

// Copy-and-swap idiom
class CopyAndSwap {
private:
    std::string* data;

public:
    CopyAndSwap(const std::string& s) : data(new std::string(s)) {}
    ~CopyAndSwap() { delete data; }

    // Copy constructor
    CopyAndSwap(const CopyAndSwap& other) : data(new std::string(*other.data)) {}

    // Copy assignment using copy-and-swap
    CopyAndSwap& operator=(CopyAndSwap other) {  // Note: by value!
        std::swap(data, other.data);  // Swap with copy
        return *this;  // 'other' destroyed with old data
    }

    const std::string& get() const { return *data; }
};

void CopyAndSwapIdiom() {
    std::cout << "\n=== Copy-and-Swap Idiom ===" << std::endl;

    CopyAndSwap a("Hello");
    CopyAndSwap b("World");

    std::cout << "Before: a=" << a.get() << ", b=" << b.get() << std::endl;
    a = b;  // Copy-and-swap
    std::cout << "After:  a=" << a.get() << ", b=" << b.get() << std::endl;

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  + Exception safe" << std::endl;
    std::cout << "  + Handles self-assignment" << std::endl;
    std::cout << "  + Reuses copy constructor" << std::endl;
}

// Rule of Three
void RuleOfThree() {
    std::cout << "\n=== Rule of Three ===" << std::endl;

    std::cout << "If you define ANY of:" << std::endl;
    std::cout << "  1. Destructor" << std::endl;
    std::cout << "  2. Copy constructor" << std::endl;
    std::cout << "  3. Copy assignment" << std::endl;

    std::cout << "\nYou probably need ALL THREE!" << std::endl;

    std::cout << "\nWhy:" << std::endl;
    std::cout << "  - Custom destructor → managing resource" << std::endl;
    std::cout << "  - Managing resource → need deep copy" << std::endl;
    std::cout << "  - Default copies are shallow → dangerous" << std::endl;

    std::cout << "\nC++11 adds:" << std::endl;
    std::cout << "  4. Move constructor" << std::endl;
    std::cout << "  5. Move assignment" << std::endl;
    std::cout << "  → Rule of FIVE" << std::endl;
}