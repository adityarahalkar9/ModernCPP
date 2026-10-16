import std;
#include "templates_header.h"

// Basic class template
template <typename T>
class Container {
    std::vector<T> data;

public:
    void push(const T& value) {
        data.push_back(value);
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }

    void print() const {
        std::cout << "  [";
        for(size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if(i + 1 < data.size()) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

void BasicClassTemplate() {
    std::cout << "=== Basic Class Templates ===" << std::endl;

    Container<int> int_container;
    int_container.push(1);
    int_container.push(2);
    int_container.push(3);

    std::cout << "Int container:" << std::endl;
    int_container.print();
    std::cout << "  Size: " << int_container.size() << std::endl;

    Container<std::string> str_container;
    str_container.push("Hello");
    str_container.push("World");

    std::cout << "\nString container:" << std::endl;
    str_container.print();
}

// Class template with multiple parameters
template <typename Key, typename Value>
class Dictionary {
    std::vector<std::pair<Key, Value>> entries;

public:
    void insert(const Key& key, const Value& value) {
        // Remove existing entry with same key
        entries.erase(
            std::remove_if(entries.begin(), entries.end(),
                           [&key](const auto& entry) { return entry.first == key; }),
            entries.end()
        );
        entries.emplace_back(key, value);
    }

    Value* find(const Key& key) {
        for(auto& [k, v] : entries) {
            if(k == key) return &v;
        }
        return nullptr;
    }

    void print() const {
        for(const auto& [key, value] : entries) {
            std::cout << "  " << key << " -> " << value << std::endl;
        }
    }
};

void MultipleParameters() {
    std::cout << "\n=== Multiple Template Parameters ===" << std::endl;

    Dictionary<std::string, int> ages;
    ages.insert("Alice", 30);
    ages.insert("Bob", 25);
    ages.insert("Charlie", 35);

    ages.print();

    if(auto* age = ages.find("Alice")) {
        std::cout << "  Found Alice: " << *age << std::endl;
    }
}

// Member functions of class templates
template <typename T>
class Stack {
    std::vector<T> data;

public:
    // Member functions defined inside class (implicit inline)
    void push(T value) {
        data.push_back(std::move(value));
    }

    // Member function defined outside class
    T pop();
    const T& top() const { return data.back(); }
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

// Definition outside class requires template repetition
template <typename T>
T Stack<T>::pop() {
    T value = std::move(data.back());
    data.pop_back();
    return value;
}

void MemberFunctionDefinition() {
    std::cout << "\n=== Member Function Definitions ===" << std::endl;

    std::cout << "Inside class: implicit 'inline'" << std::endl;
    std::cout << "Outside class: need 'template <typename T>'" << std::endl;
    std::cout << "  template <typename T>" << std::endl;
    std::cout << "  T Stack<T>::pop() { ... }" << std::endl;

    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    std::cout << "\nUsing stack:" << std::endl;
    while(!stack.empty()) {
        std::cout << "  Popped: " << stack.pop() << std::endl;
    }
}

// Nested types in class templates
template <typename T>
class Wrapper {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    explicit Wrapper(T value) : value_(std::move(value)) {}

    value_type get() const { return value_; }
    reference get_ref() { return value_; }
    const_reference get_const_ref() const { return value_; }

private:
    value_type value_;
};

void NestedTypes() {
    std::cout << "\n=== Nested Types in Class Templates ===" << std::endl;

    std::cout << "Standard pattern for container types:" << std::endl;
    std::cout << "  using value_type = T;" << std::endl;
    std::cout << "  using reference = T&;" << std::endl;
    std::cout << "  using const_reference = const T&;" << std::endl;

    Wrapper<int> w(42);
    std::cout << "\nWrapper<int>: " << w.get() << std::endl;

    Wrapper<int>::value_type value = w.get();
    std::cout << "Using nested type: " << value << std::endl;
}

// Template class with default operations
template <typename T>
class Resource {
    std::unique_ptr<T> ptr_;

public:
    explicit Resource(T value) : ptr_(std::make_unique<T>(std::move(value))) {}

    // Default copy (works if T is copyable)
    Resource(const Resource&) = default;
    Resource& operator=(const Resource&) = default;

    // Default move
    Resource(Resource&&) noexcept = default;
    Resource& operator=(Resource&&) noexcept = default;

    T& operator*() { return *ptr_; }
    const T& operator*() const { return *ptr_; }
};

void DefaultOperations() {
    std::cout << "\n=== Default Operations ===" << std::endl;

    std::cout << "Compiler generates operations based on T:" << std::endl;
    std::cout << "  If T is copyable → copy works" << std::endl;
    std::cout << "  If T is movable → move works" << std::endl;
    std::cout << "  If T has neither → both deleted" << std::endl;

    Resource<int> r1(42);
    Resource<int> r2 = r1;  // Copy (int is copyable)
    Resource<int> r3 = std::move(r2);  // Move

    std::cout << "  r1: " << *r1 << ", r3: " << *r3 << std::endl;
}