import std;
#include "class_header.h"

// Class with move operations
class Movable {
private:
    std::string* data;
    int* resource;

public:
    // Constructor
    Movable(const std::string& s) : data(new std::string(s)), resource(new int(42)) {
        std::cout << "  Movable('" << *data << "') constructed" << std::endl;
    }

    // Copy constructor
    Movable(const Movable& other)
        : data(new std::string(*other.data)), resource(new int(*other.resource)) {
        std::cout << "  COPY: expensive deep copy" << std::endl;
    }

    // MOVE CONSTRUCTOR
    Movable(Movable&& other) noexcept
        : data(other.data), resource(other.resource) {
        std::cout << "  MOVE: cheap pointer steal" << std::endl;
        other.data = nullptr;      // Leave source in valid state
        other.resource = nullptr;
    }

    // Copy assignment
    Movable& operator=(const Movable& other) {
        if(this != &other) {
            *data = *other.data;
            *resource = *other.resource;
        }
        return *this;
    }

    // MOVE ASSIGNMENT
    Movable& operator=(Movable&& other) noexcept {
        if(this != &other) {
            delete data;
            delete resource;
            data = other.data;
            resource = other.resource;
            other.data = nullptr;
            other.resource = nullptr;
        }
        return *this;
    }

    ~Movable() {
        if(data) {
            std::cout << "  Movable('" << *data << "') destroyed" << std::endl;
        }
        else {
            std::cout << "  Movable(moved-from) destroyed" << std::endl;
        }
        delete data;
        delete resource;
    }

    const std::string& get() const { return *data; }
};

void MoveConstructorDemo() {
    std::cout << "=== Move Constructor ===" << std::endl;

    std::cout << "Creating original:" << std::endl;
    Movable original("Hello");

    std::cout << "\nCopy construction:" << std::endl;
    Movable copy = original;  // Copy

    std::cout << "\nMove construction:" << std::endl;
    Movable moved = std::move(original);  // Move

    std::cout << "\nOriginal after move: " << (original.get().empty() ? "empty" : "still has data") << std::endl;
    std::cout << "Moved: " << moved.get() << std::endl;
}

// Move vs Copy performance
class BigData {
private:
    std::vector<double> data;

public:
    BigData(size_t n) : data(n, 3.14159) {
        std::cout << "  BigData(" << n << ") constructed" << std::endl;
    }

    // Copy
    BigData(const BigData& other) : data(other.data) {
        std::cout << "  COPY: " << data.size() << " elements" << std::endl;
    }

    // Move
    BigData(BigData&& other) noexcept : data(std::move(other.data)) {
        std::cout << "  MOVE: stole " << data.size() << " elements" << std::endl;
        other.data.clear();
    }

    BigData& operator=(BigData&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }

    size_t size() const { return data.size(); }
};

void MoveVsCopyPerformance() {
    std::cout << "\n=== Move vs Copy Performance ===" << std::endl;

    std::cout << "Creating big data (1M elements):" << std::endl;
    BigData big(1'000'000);

    std::cout << "\nCopying (slow - copies all data):" << std::endl;
    BigData copy = big;

    std::cout << "\nMoving (fast - steals pointer):" << std::endl;
    BigData moved = std::move(big);

    std::cout << "\nAfter move, original size: " << big.size() << std::endl;
    std::cout << "Moved size: " << moved.size() << std::endl;
}

// Move in containers
void MoveInContainers() {
    std::cout << "\n=== Move in Containers ===" << std::endl;

    std::vector<Movable> vec;

    std::cout << "Pushing (creates temp, then moves):" << std::endl;
    vec.push_back(Movable("First"));   // Temporary moved in
    vec.push_back(Movable("Second"));  // May reallocate: old elements moved

    std::cout << "\nContents:" << std::endl;
    for(const auto& m : vec) {
        std::cout << "  " << m.get() << std::endl;
    }
}

// noexcept importance
class NoExceptMovable {
    std::string data;

public:
    NoExceptMovable(std::string s) : data(std::move(s)) {}

    // Without noexcept, vector may copy instead of move!
    NoExceptMovable(NoExceptMovable&& other) noexcept
        : data(std::move(other.data)) {
        std::cout << "  Move (noexcept)" << std::endl;
    }

    NoExceptMovable(const NoExceptMovable& other) : data(other.data) {
        std::cout << "  Copy (fallback)" << std::endl;
    }
};

void NoExceptImportance() {
    std::cout << "\n=== noexcept Importance ===" << std::endl;

    std::cout << "Move constructors MUST be noexcept:" << std::endl;
    std::cout << "  vector needs strong exception guarantee" << std::endl;
    std::cout << "  If move can throw, vector uses copy instead" << std::endl;

    std::vector<NoExceptMovable> vec;
    vec.reserve(2);  // Prevent reallocation for demo

    std::cout << "\nAdding elements:" << std::endl;
    vec.emplace_back("Hello");
    vec.emplace_back("World");

    std::cout << "\nWithout noexcept, these would COPY!" << std::endl;
}

// Moved-from state
class MoveAware {
    std::string name;

public:
    MoveAware(std::string n) : name(std::move(n)) {}

    MoveAware(MoveAware&& other) noexcept
        : name(std::move(other.name)) {
        // other.name is now in "valid but unspecified state"
        // For std::string, typically empty
    }

    bool is_valid() const { return !name.empty(); }
    const std::string& get_name() const { return name; }
};

void MovedFromState() {
    std::cout << "\n=== Moved-From State ===" << std::endl;

    MoveAware original("Hello");
    std::cout << "Original valid: " << original.is_valid() << std::endl;

    MoveAware moved = std::move(original);
    std::cout << "After move:" << std::endl;
    std::cout << "  Original valid: " << original.is_valid() << std::endl;
    std::cout << "  Moved valid: " << moved.is_valid() << std::endl;

    std::cout << "\nMoved-from object:" << std::endl;
    std::cout << "  - In 'valid but unspecified state'" << std::endl;
    std::cout << "  - Can be destroyed" << std::endl;
    std::cout << "  - Can be assigned to" << std::endl;
    std::cout << "  - Should NOT be read from" << std::endl;
}

// Rule of Five
void RuleOfFive() {
    std::cout << "\n=== Rule of Five ===" << std::endl;

    std::cout << "If you manage resources, define ALL:" << std::endl;
    std::cout << "  1. Destructor" << std::endl;
    std::cout << "  2. Copy constructor" << std::endl;
    std::cout << "  3. Copy assignment" << std::endl;
    std::cout << "  4. Move constructor" << std::endl;
    std::cout << "  5. Move assignment" << std::endl;

    std::cout << "\nOr better: use Rule of ZERO" << std::endl;
    std::cout << "  Let members handle themselves" << std::endl;
    std::cout << "  std::string, std::vector, unique_ptr" << std::endl;
}

// Rule of Zero
class ZeroRule {
    std::string data;  // Members handle themselves
    std::vector<int> numbers;

public:
    ZeroRule(std::string d) : data(std::move(d)), numbers{1, 2, 3} {}

    // All special members are auto-generated
    // And they do the right thing!
};

void RuleOfZero() {
    std::cout << "\n=== Rule of Zero ===" << std::endl;

    std::cout << "PREFER Rule of Zero:" << std::endl;
    std::cout << "  Use members that manage themselves" << std::endl;
    std::cout << "  std::string, std::vector, unique_ptr" << std::endl;

    std::cout << "\nBenefits:" << std::endl;
    std::cout << "  + Less code to write" << std::endl;
    std::cout << "  + Fewer bugs" << std::endl;
    std::cout << "  + Optimal performance" << std::endl;
    std::cout << "  + Exception safety for free" << std::endl;
}