import std;
#include "class_header.h"

// Const member functions
class Temperature {
private:
    double celsius;
    mutable int access_count = 0;  // mutable: modifiable in const functions

public:
    Temperature(double c) : celsius(c) {}

    // Const member function: promises not to modify object
    double get_celsius() const {
        ++access_count;  // OK: mutable can be modified
        return celsius;
    }

    double get_fahrenheit() const {
        return celsius * 9.0 / 5.0 + 32.0;
    }

    // Non-const member function: may modify object
    void set_celsius(double c) {
        celsius = c;
    }

    // Const can call other const functions
    void print() const {
        std::cout << "  " << get_celsius() << "°C ("
            << get_fahrenheit() << "°F)" << std::endl;
    }

    int get_access_count() const { return access_count; }

    // ERROR: const function cannot modify non-mutable members
    // void bad_const() const {
    //     celsius = 100;  // ERROR!
    // }
};

void ConstMemberFunctions() {
    std::cout << "=== Const Member Functions ===" << std::endl;

    Temperature t(25.0);
    t.print();

    const Temperature const_t(0.0);
    const_t.print();  // OK: const functions on const object

    // const_t.set_celsius(100);  // ERROR: non-const on const object

    std::cout << "\nRules:" << std::endl;
    std::cout << "  const objects can only call const functions" << std::endl;
    std::cout << "  non-const objects can call anything" << std::endl;
    std::cout << "  Mark functions const if they don't modify" << std::endl;
}

// Mutable members
class Cache {
private:
    mutable int cache_hits = 0;
    mutable int cache_misses = 0;
    mutable std::string cached_result;  // Can be modified in const functions
    bool cache_valid = false;

public:
    std::string compute() const {  // Const but can modify mutable members
        if(cache_valid) {
            ++cache_hits;
            std::cout << "  Cache hit!" << std::endl;
            return cached_result;
        }

        ++cache_misses;
        std::cout << "  Cache miss - computing..." << std::endl;

        cached_result = "expensive_result";  // OK: mutable
        cache_valid = true;  // NOT OK if not mutable!

        return cached_result;
    }

    void invalidate() {  // Non-const: can modify everything
        cache_valid = false;
        cached_result.clear();
    }

    int get_hits() const { return cache_hits; }
    int get_misses() const { return cache_misses; }
};

void MutableMembers() {
    std::cout << "\n=== Mutable Members ===" << std::endl;

    Cache c;

    std::cout << "First call:" << std::endl;
    c.compute();

    std::cout << "\nSecond call:" << std::endl;
    c.compute();

    std::cout << "\nHits: " << c.get_hits() << ", Misses: " << c.get_misses() << std::endl;

    std::cout << "\nmutable use cases:" << std::endl;
    std::cout << "  - Caches" << std::endl;
    std::cout << "  - Access counters" << std::endl;
    std::cout << "  - Debug info" << std::endl;
    std::cout << "  - Synchronization (mutexes)" << std::endl;
}

// const this pointer
class ConstThis {
public:
    void non_const_method() {
        // this is ConstThis*
        std::cout << "  non-const method: this = " << this << std::endl;
    }

    void const_method() const {
        // this is const ConstThis*
        std::cout << "  const method: this = " << this << std::endl;
    }
};

void ConstThisPointer() {
    std::cout << "\n=== const this Pointer ===" << std::endl;

    ConstThis obj;
    const ConstThis const_obj;

    obj.non_const_method();  // this = ConstThis*
    obj.const_method();       // this = const ConstThis* (in const method)

    const_obj.const_method();  // Only const methods available
    // const_obj.non_const_method();  // ERROR!

    std::cout << "\n'const' after member function:" << std::endl;
    std::cout << "  Makes 'this' pointer const" << std::endl;
    std::cout << "  Function cannot modify members" << std::endl;
}

// Returning const references
class Data {
private:
    std::vector<int> values;

public:
    Data(std::initializer_list<int> init) : values(init) {}

    // Return const reference: caller can read but not modify
    const std::vector<int>& get_values() const {
        return values;
    }

    // Return non-const reference: caller can modify
    std::vector<int>& get_mutable_values() {
        return values;
    }
};

void ReturningConstRefs() {
    std::cout << "\n=== Returning const References ===" << std::endl;

    Data d{1, 2, 3, 4, 5};

    // Const reference: read-only access
    const auto& const_vals = d.get_values();
    std::cout << "Values: ";
    for(int v : const_vals) std::cout << v << " ";
    std::cout << std::endl;

    // const_vals.push_back(6);  // ERROR: const reference

    // Non-const reference: can modify
    auto& mutable_vals = d.get_mutable_values();
    mutable_vals.push_back(6);
    std::cout << "After modification: " << mutable_vals.size() << " elements" << std::endl;
}

// Logical const vs bitwise const
class LogicalConst {
private:
    std::string cached_data;
    mutable bool cache_valid = false;

public:
    // Logically const: observable state doesn't change
    // But internal cache might update
    const std::string& get_data() const {
        if(!cache_valid) {
            cached_data = "computed";  // Modifies internal state
            cache_valid = true;         // But logically same result
        }
        return cached_data;
    }
};

void LogicalVsBitwiseConst() {
    std::cout << "\n=== Logical vs Bitwise const ===" << std::endl;

    std::cout << "Bitwise const (physical const):" << std::endl;
    std::cout << "  No bits in object change" << std::endl;
    std::cout << "  Very strict" << std::endl;

    std::cout << "\nLogical const (observable const):" << std::endl;
    std::cout << "  Observable behavior doesn't change" << std::endl;
    std::cout << "  Internal state may change (mutable)" << std::endl;
    std::cout << "  This is what 'const' should mean" << std::endl;

    LogicalConst lc;
    const std::string& result = lc.get_data();
    std::cout << "  Result: " << result << std::endl;
}

// Const propagation
class Outer {
private:
    Inner inner;

public:
    class Inner {
    public:
        void modify() { std::cout << "  Inner::modify" << std::endl; }
        void read() const { std::cout << "  Inner::read" << std::endl; }
    };

    // Non-const method: returns non-const reference
    Inner& get_inner() { return inner; }

    // Const method: returns const reference
    const Inner& get_inner() const { return inner; }
};

void ConstPropagation() {
    std::cout << "\n=== Const Propagation ===" << std::endl;

    Outer o;
    const Outer& const_o = o;

    // Non-const object: full access
    o.get_inner().modify();
    o.get_inner().read();

    // Const object: only const access
    const_o.get_inner().read();  // OK: const method
    // const_o.get_inner().modify();  // ERROR: non-const method

    std::cout << "\nConst propagates through returned references" << std::endl;
}