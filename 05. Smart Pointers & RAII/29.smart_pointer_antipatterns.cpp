import std;
#include "smart_pointers_header.h"

// Antipattern 1: shared_ptr everywhere
class Everything {
public:
    // BAD: shared_ptr for everything
    std::shared_ptr<int> bad_count = std::make_shared<int>(0);
    std::shared_ptr<std::string> bad_name = std::make_shared<std::string>("");

    // GOOD: appropriate types
    int good_count = 0;
    std::string good_name = "";
};

void AntipatternOveruse() {
    std::cout << "=== Antipattern: Overuse ===" << std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  Using shared_ptr for simple values" << std::endl;
    std::cout << "  Adds overhead with no benefit" << std::endl;

    std::cout << "\nGOOD:" << std::endl;
    std::cout << "  Use appropriate types:" << std::endl;
    std::cout << "    int for numbers" << std::endl;
    std::cout << "    std::string for strings" << std::endl;
    std::cout << "    unique_ptr for exclusive ownership" << std::endl;
}

// Antipattern 2: get() then delete
void AntipatternGetDelete() {
    std::cout << "\n=== Antipattern: get() + delete ===" << std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  auto raw = smart.get();" << std::endl;
    std::cout << "  delete raw;  // Double delete!" << std::endl;

    std::cout << "\nGOOD:" << std::endl;
    std::cout << "  smart.reset();  // Smart pointer handles it" << std::endl;
    std::cout << "  // OR just let it go out of scope" << std::endl;
}

// Antipattern 3: shared_ptr from raw
void AntipatternSharedFromRaw() {
    std::cout << "\n=== Antipattern: shared_ptr from raw ===" << std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  Widget* raw = new Widget();" << std::endl;
    std::cout << "  std::shared_ptr<Widget> a(raw);" << std::endl;
    std::cout << "  std::shared_ptr<Widget> b(raw);  // Double delete!" << std::endl;

    std::cout << "\nGOOD:" << std::endl;
    std::cout << "  auto a = std::make_shared<Widget>();" << std::endl;
    std::cout << "  auto b = a;  // Share properly" << std::endl;
}

// Antipattern 4: Not using make_shared/make_unique
void AntipatternNoMake() {
    std::cout << "\n=== Antipattern: Not using make_* ===" << std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  std::shared_ptr<Widget> p(new Widget(args));" << std::endl;
    std::cout << "  // 2 allocations, exception safety issues" << std::endl;

    std::cout << "\nGOOD:" << std::endl;
    std::cout << "  auto p = std::make_shared<Widget>(args);" << std::endl;
    std::cout << "  // 1 allocation, exception safe" << std::endl;
}

// Antipattern 5: shared_ptr in interfaces when unique_ptr suffices
void AntipatternWrongType() {
    std::cout << "\n=== Antipattern: Wrong Smart Pointer ===" std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  void process(std::shared_ptr<Widget> w);" << std::endl;
    std::cout << "  // Implies shared ownership" << std::endl;
    std::cout << "  // Forces shared_ptr on all callers" << std::endl;

    std::cout << "\nGOOD (if just observing):" << std::endl;
    std::cout << "  void process(const Widget& w);" << std::endl;
    std::cout << "  // Clear: no ownership transfer" << std::endl;

    std::cout << "\nGOOD (if taking ownership):" << std::endl;
    std::cout << "  void process(std::unique_ptr<Widget> w);" << std::endl;
    std::cout << "  // Clear: taking exclusive ownership" << std::endl;
}

// Antipattern 6: Circular shared_ptr dependencies
class Bad1;
class Bad2;

class Bad1 {
public:
    std::shared_ptr<Bad2> other;  // Creates cycle if Bad2 also points to Bad1
    ~Bad1() { std::cout << "  Bad1 destroyed" << std::endl; }
};

class Bad2 {
public:
    std::shared_ptr<Bad1> other;  // CYCLE!
    ~Bad2() { std::cout << "  Bad2 destroyed" << std::endl; }
};

void AntipatternCycles() {
    std::cout << "\n=== Antipattern: Cycles ===" << std::endl;

    std::cout << "BAD:" << std::endl;
    std::cout << "  Both directions use shared_ptr" << std::endl;
    std::cout << "  → Memory leak" << std::endl;

    std::cout << "\nDemonstrating leak:" << std::endl;
    {
        auto a = std::make_shared<Bad1>();
        auto b = std::make_shared<Bad2>();
        a->other = b;
        b->other = a;  // Cycle!
    }
    // Note: No destroy messages → leaked!

    std::cout << "\nGOOD:" << std::endl;
    std::cout << "  One direction uses weak_ptr" << std::endl;
}

// Summary of antipatterns
void AntipatternSummary() {
    std::cout << "\n=== Summary ===" << std::endl;

    std::cout << "AVOID:" << std::endl;
    std::cout << "  1. shared_ptr for everything" << std::endl;
    std::cout << "  2. get() followed by delete" << std::endl;
    std::cout << "  3. Creating shared_ptr from same raw pointer" << std::endl;
    std::cout << "  4. Not using make_shared/make_unique" << std::endl;
    std::cout << "  5. shared_ptr when unique_ptr suffices" << std::endl;
    std::cout << "  6. Circular shared_ptr references" << std::endl;
    std::cout << std::endl;

    std::cout << "PREFER:" << std::endl;
    std::cout << "  1. unique_ptr as default" << std::endl;
    std::cout << "  2. Raw references for observation" << std::endl;
    std::cout << "  3. make_shared/make_unique" << std::endl;
    std::cout << "  4. weak_ptr to break cycles" << std::endl;
    std::cout << "  5. Clear ownership semantics in APIs" << std::endl;
}