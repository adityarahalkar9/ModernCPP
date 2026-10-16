import std;
#include "structs_header.h"

// Large struct for demonstration
struct BigStruct {
    double data[1000];  // 8000 bytes
    int count = 0;
};

struct SmallStruct {
    int a, b;
};

// Pass by value (copies the struct)
void ProcessByValue(SmallStruct s) {
    s.a = 100;  // Modifies local copy only
    std::cout << "Inside function: a=" << s.a << std::endl;
}

// Pass by reference (no copy, can modify)
void ProcessByReference(SmallStruct& s) {
    s.a = 100;  // Modifies original
}

// Pass by const reference (no copy, can't modify)
void ProcessByConstRef(const SmallStruct& s) {
    // s.a = 100;  // Error: const reference
    std::cout << "Read-only access: a=" << s.a << std::endl;
}

void PassingStructs() {
    std::cout << "=== Passing Structs to Functions ===" << std::endl;

    SmallStruct s{1, 2};

    std::cout << "Before by-value: a=" << s.a << std::endl;
    ProcessByValue(s);
    std::cout << "After by-value: a=" << s.a << " (unchanged)" << std::endl;

    ProcessByReference(s);
    std::cout << "After by-reference: a=" << s.a << " (modified)" << std::endl;

    ProcessByConstRef(s);
    std::cout << "Const ref: a=" << s.a << std::endl;
}

// Return by value
SmallStruct MakeSmallStruct() {
    return SmallStruct{42, 24};
}

// Return by reference (dangerous if local!)
// SmallStruct& DangerousReturn() {
//     SmallStruct local{1, 2};
//     return local;  // Dangling reference!
// }

void ReturningStructs() {
    std::cout << "\n=== Returning Structs ===" << std::endl;

    // Return by value is safe (RVO/NRVO)
    SmallStruct s = MakeSmallStruct();
    std::cout << "Returned: a=" << s.a << ", b=" << s.b << std::endl;

    std::cout << "\nReturn value optimization:" << std::endl;
    std::cout << "  Compiler can eliminate the copy" << std::endl;
    std::cout << "  Struct is constructed directly in caller's storage" << std::endl;
}

// RVO demonstration
struct Tracker {
    static int constructions;
    static int copies;
    static int moves;

    int value;

    Tracker(int v) : value(v) { ++constructions; }
    Tracker(const Tracker& other) : value(other.value) { ++copies; }
    Tracker(Tracker&& other) : value(other.value) { ++moves; }
};

int Tracker::constructions = 0;
int Tracker::copies = 0;
int Tracker::moves = 0;

Tracker MakeTracker() {
    Tracker t(42);  // One construction
    return t;       // NRVO: no copy/move needed!
}

void RVODemo() {
    std::cout << "\n=== Return Value Optimization ===" << std::endl;

    Tracker::constructions = 0;
    Tracker::copies = 0;
    Tracker::moves = 0;

    Tracker t = MakeTracker();

    std::cout << "Constructions: " << Tracker::constructions << std::endl;
    std::cout << "Copies: " << Tracker::copies << std::endl;
    std::cout << "Moves: " << Tracker::moves << std::endl;

    std::cout << "\nWith NRVO, no copies or moves happen!" << std::endl;
    std::cout << "The struct is constructed directly in caller's memory" << std::endl;
}

// Performance comparison
void PerformanceComparison() {
    std::cout << "\n=== Performance: Pass by Value vs Reference ===" << std::endl;

    BigStruct big;
    big.count = 1000;
    for(int i = 0; i < 1000; ++i) {
        big.data[i] = i * 1.5;
    }

    constexpr int iterations = 100000;

    // Pass by value (copies 8KB each time)
    auto by_value = [](BigStruct s) {
        return s.count;
        };

    // Pass by reference (no copy)
    auto by_ref = [](const BigStruct& s) {
        return s.count;
        };

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        by_value(big);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto value_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        by_ref(big);
    }
    end = std::chrono::high_resolution_clock::now();
    auto ref_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "By value:    " << value_time << " μs" << std::endl;
    std::cout << "By const ref: " << ref_time << " μs" << std::endl;
    std::cout << "Difference: " << (value_time - ref_time) << " μs" << std::endl;

    std::cout << "\nRules of thumb:" << std::endl;
    std::cout << "  Small structs (<= 2-3 words): pass by value" << std::endl;
    std::cout << "  Large structs: pass by const reference" << std::endl;
    std::cout << "  Need to modify: pass by reference" << std::endl;
}

// Struct as function parameters with defaults
struct Options {
    int verbosity = 1;
    bool debug = false;
    std::string log_file = "log.txt";
};

void Configure(const Options& opts = Options{}) {
    std::cout << "verbosity: " << opts.verbosity << std::endl;
    std::cout << "debug: " << opts.debug << std::endl;
    std::cout << "log_file: " << opts.log_file << std::endl;
}

void DefaultParameters() {
    std::cout << "\n=== Struct as Default Parameters ===" << std::endl;

    std::cout << "Default options:" << std::endl;
    Configure();

    std::cout << "\nCustom options:" << std::endl;
    Options custom{3, true, "debug.log"};
    Configure(custom);
}