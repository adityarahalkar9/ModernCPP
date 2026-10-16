import std;
#include "class_header.h"

// Class with all special member functions
class AllSpecial {
public:
    // 1. Default constructor
    AllSpecial() {
        std::cout << "  Default constructor" << std::endl;
    }

    // 2. Destructor
    ~AllSpecial() {
        std::cout << "  Destructor" << std::endl;
    }

    // 3. Copy constructor
    AllSpecial(const AllSpecial& other) {
        std::cout << "  Copy constructor" << std::endl;
    }

    // 4. Copy assignment
    AllSpecial& operator=(const AllSpecial& other) {
        std::cout << "  Copy assignment" << std::endl;
        return *this;
    }

    // 5. Move constructor
    AllSpecial(AllSpecial&& other) noexcept {
        std::cout << "  Move constructor" << std::endl;
    }

    // 6. Move assignment
    AllSpecial& operator=(AllSpecial&& other) noexcept {
        std::cout << "  Move assignment" << std::endl;
        return *this;
    }
};

void AllSpecialMembers() {
    std::cout << "=== Special Member Functions ===" << std::endl;

    std::cout << "The 6 special members:" << std::endl;
    std::cout << "  1. Default constructor" << std::endl;
    std::cout << "  2. Destructor" << std::endl;
    std::cout << "  3. Copy constructor" << std::endl;
    std::cout << "  4. Copy assignment" << std::endl;
    std::cout << "  5. Move constructor" << std::endl;
    std::cout << "  6. Move assignment" << std::endl;

    std::cout << "\nWhen each is called:" << std::endl;
    AllSpecial a;              // Default constructor
    AllSpecial b = a;          // Copy constructor
    AllSpecial c = std::move(a);  // Move constructor
    b = c;                     // Copy assignment
    b = std::move(c);          // Move assignment
}

// Defaulting special members
class WithDefaults {
public:
    WithDefaults() = default;                              // Default constructor
    WithDefaults(const WithDefaults&) = default;            // Copy constructor
    WithDefaults& operator=(const WithDefaults&) = default; // Copy assignment
    WithDefaults(WithDefaults&&) noexcept = default;         // Move constructor
    WithDefaults& operator=(WithDefaults&&) noexcept = default; // Move assignment
    ~WithDefaults() = default;                              // Destructor
};

void DefaultingSpecialMembers() {
    std::cout << "\n=== Defaulting Special Members ===" << std::endl;

    std::cout << "= default:" << std::endl;
    std::cout << "  Use compiler-generated version" << std::endl;
    std::cout << "  More efficient than empty body" << std::endl;
    std::cout << "  Can be constexpr/trivial" << std::endl;

    WithDefaults a, b;
    b = a;  // Uses defaulted copy assignment
}

// Deleting special members
class NonCopyable {
public:
    NonCopyable() = default;
    ~NonCopyable() = default;

    // Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // Keep move operations
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

void DeletingSpecialMembers() {
    std::cout << "\n=== Deleting Special Members ===" << std::endl;

    std::cout << "= delete:" << std::endl;
    std::cout << "  Prevents use of that function" << std::endl;
    std::cout << "  Compile error if used" << std::endl;

    NonCopyable a, b;
    // a = b;  // ERROR: copy assignment deleted

    NonCopyable c = std::move(a);  // OK: move allowed
    b = std::move(c);              // OK: move assignment
}

// Generation rules
void GenerationRules() {
    std::cout << "\n=== Generation Rules ===" << std::endl;

    std::cout << "If you declare... compiler generates..." << std::endl;
    std::cout << std::endl;
    std::cout << "  Nothing:        All 6 (implicitly)" << std::endl;
    std::cout << "  Destructor:     Copy, Move (deprecated)" << std::endl;
    std::cout << "  Copy ctor:      Copy assignment" << std::endl;
    std::cout << "  Copy assign:    Copy constructor" << std::endl;
    std::cout << "  Move ctor:      Nothing (copy deleted)" << std::endl;
    std::cout << "  Move assign:    Nothing (copy deleted)" << std::endl;
    std::cout << "  Any ctor:       Default ctor NOT generated" << std::endl;

    std::cout << "\nKey rule:" << std::endl;
    std::cout << "  Declaring move operations deletes copy operations" << std::endl;
    std::cout << "  Declaring copy operations suppresses move operations" << std::endl;
}

// Detection idiom
template <typename T>
void AnalyzeSpecialMembers() {
    std::cout << "\nAnalyzing " << typeid(T).name() << ":" << std::endl;

    std::cout << "  Default constructible: "
        << std::is_default_constructible_v<T> << std::endl;
    std::cout << "  Copy constructible:     "
        << std::is_copy_constructible_v<T> << std::endl;
    std::cout << "  Move constructible:     "
        << std::is_move_constructible_v<T> << std::endl;
    std::cout << "  Copy assignable:        "
        << std::is_copy_assignable_v<T> << std::endl;
    std::cout << "  Move assignable:        "
        << std::is_move_assignable_v<T> << std::endl;
    std::cout << "  Trivially copyable:     "
        << std::is_trivially_copyable_v<T> << std::endl;
}

void DetectionIdiom() {
    std::cout << "\n=== Detection Idiom ===" << std::endl;

    class SimpleClass {};
class Deleted { public: Deleted(const Deleted&) = delete; };

                      AnalyzeSpecialMembers<SimpleClass>();
                      AnalyzeSpecialMembers<Deleted>();
}

// Rule of Zero example
class Zero {
    std::string data;        // Manages itself
    std::vector<int> vec;    // Manages itself

public:
    Zero(std::string d) : data(std::move(d)), vec{1, 2, 3} {}

    // All special members auto-generated and correct!
};

void RuleOfZeroExample() {
    std::cout << "\n=== Rule of Zero ===" << std::endl;

    std::cout << "PREFER approach:" << std::endl;
    std::cout << "  Members manage their own resources" << std::endl;
    std::cout << "  No manual special member functions" << std::endl;
    std::cout << "  Let compiler generate optimal code" << std::endl;

    Zero z1("hello");
    Zero z2 = z1;      // Copy works
    Zero z3 = std::move(z1);  // Move works
}