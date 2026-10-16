import std;
#include "class_header.h"

// Basic mixins
template <typename Base>
class PrintableMixin : public Base {
public:
    void print() const {
        std::cout << "  " << to_string() << std::endl;
    }

    // Requires Base (or further derived) to provide this
    std::string to_string() const {
        return static_cast<const typename Base::Implemented*>(this)->to_string_impl();
    }
};

// Mixin for comparison
template <typename Base>
class ComparableMixin : public Base {
public:
    template <typename T>
    bool operator>(const T& other) const {
        return get_value() > other.get_value();
    }

    template <typename T>
    bool operator<(const T& other) const {
        return get_value() < other.get_value();
    }

    // Requires these from the class
    int get_value() const {
        return static_cast<const typename Base::Implemented*>(this)->get_value_impl();
    }
};

// CRTP-style mixins
template <typename Derived>
class Printable2 {
public:
    void print() const {
        std::cout << "  " << static_cast<const Derived*>(this)->to_string() << std::endl;
    }
};

template <typename Derived>
class Countable {
    static inline int count = 0;

public:
    Countable() { ++count; }
    ~Countable() { --count; }

    static int get_count() { return count; }
};

template <typename Derived>
class Serializable2 {
public:
    std::string serialize() const {
        return static_cast<const Derived*>(this)->to_string();
    }
};

// Compose mixins
class Document
    : public Printable2<Document>,
    public Countable<Document>,
    public Serializable2<Document> {
    std::string content;

public:
    Document(std::string c) : content(std::move(c)) {}

    std::string to_string() const {
        return "Document: " + content;
    }
};

void MixinBasics() {
    std::cout << "=== Mixin Pattern ===" << std::endl;

    std::cout << "Mixins add capabilities:" << std::endl;
    std::cout << "  Compose from small pieces" << std::endl;
    std::cout << "  More flexible than inheritance" << std::endl;

    Document doc("Hello World");

    doc.print();  // From Printable2
    std::cout << "  Serialized: " << doc.serialize() << std::endl;  // From Serializable2
    std::cout << "  Count: " << Document::get_count() << std::endl;  // From Countable
}

// Mixins for policies
template <typename ThreadPolicy>
class Container3 : private ThreadPolicy {
    std::vector<int> data;

public:
    void push(int value) {
        ThreadPolicy::lock();
        data.push_back(value);
        ThreadPolicy::unlock();
    }

    size_t size() const {
        ThreadPolicy::lock();
        size_t s = data.size();
        ThreadPolicy::unlock();
        return s;
    }
};

class SingleThreaded {
public:
    void lock() {}
    void unlock() {}
};

class MultiThreaded {
    std::mutex mtx;

public:
    void lock() { mtx.lock(); }
    void unlock() { mtx.unlock(); }
};

void PolicyMixins() {
    std::cout << "\n=== Policy-Based Design ===" << std::endl;

    std::cout << "Select behavior via template parameter:" << std::endl;

    Container3<SingleThreaded> single;
    single.push(42);
    std::cout << "  Single-threaded size: " << single.size() << std::endl;

    Container3<MultiThreaded> multi;
    multi.push(100);
    std::cout << "  Multi-threaded size: " << multi.size() << std::endl;
}

// Mixins with composition
class LogMixin {
public:
    void log(const std::string& msg) {
        std::cout << "  [LOG] " << msg << std::endl;
    }
};

class ValidationMixin {
public:
    bool validate(int value) {
        return value >= 0 && value <= 100;
    }
};

class Service : private LogMixin, private ValidationMixin {
public:
    bool process(int value) {
        log("Processing value");

        if(!validate(value)) {
            log("Validation failed");
            return false;
        }

        log("Processing successful");
        return true;
    }
};

void CompositionMixins() {
    std::cout << "\n=== Composition Mixins ===" << std::endl;

    std::cout << "Private inheritance for implementation:" << std::endl;

    Service service;
    bool result = service.process(42);
    std::cout << "  Process result: " << result << std::endl;

    bool bad_result = service.process(150);
    std::cout << "  Bad input result: " << bad_result << std::endl;
}

// Mixin benefits
void MixinBenefits() {
    std::cout << "\n=== Mixin Benefits ===" << std::endl;

    std::cout << "1. Flexibility:" << std::endl;
    std::cout << "   Pick only what you need" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Reusability:" << std::endl;
    std::cout << "   Mixins used across classes" << std::endl;
    std::cout << std::endl;

    std::cout << "3. No deep hierarchies:" << std::endl;
    std::cout << "   Flat composition" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Compile-time selection:" << std::endl;
    std::cout << "   Zero runtime overhead" << std::endl;
}