import std;
#include "smart_pointers_header.h"

class Pitfall {
public:
    Pitfall() { std::cout << "  Pitfall created" << std::endl; }
    ~Pitfall() { std::cout << "  Pitfall destroyed" << std::endl; }
};

// Pitfall 1: Using shared_ptr(this)
void PitfallSharedThis() {
    std::cout << "=== Pitfall: shared_ptr(this) ===" << std::endl;

    std::cout << "PROBLEM:" << std::endl;
    std::cout << "  Pitfall* raw = this;" << std::endl;
    std::cout << "  std::shared_ptr<Pitfall> sp(raw);  // BAD!" << std::endl;
    std::cout << "  Creates NEW control block" << std::endl;
    std::cout << "  → Double deletion!" << std::endl;

    std::cout << "\nSOLUTION:" << std::endl;
    std::cout << "  Inherit enable_shared_from_this<T>" << std::endl;
    std::cout << "  Use shared_from_this()" << std::endl;
}

// Pitfall 2: Cycles
class Node2 {
public:
    std::shared_ptr<Node2> next;  // Can create cycle!
    ~Node2() { std::cout << "  Node2 destroyed" << std::endl; }
};

void PitfallCycles() {
    std::cout << "\n=== Pitfall: Reference Cycles ===" << std::endl;

    std::cout << "PROBLEM:" << std::endl;
    std::cout << "  A->next = B (shared)" << std::endl;
    std::cout << "  B->next = A (shared)  // CYCLE!" << std::endl;
    std::cout << "  Neither can be destroyed" << std::endl;

    std::cout << "\nDemonstrating (leaks):" << std::endl;
    {
        auto a = std::make_shared<Node2>();
        auto b = std::make_shared<Node2>();
        a->next = b;
        b->next = a;  // Cycle!
    }
    // Note: No "destroyed" messages → leaked!

    std::cout << "SOLUTION: Use weak_ptr for one direction" << std::endl;
}

// Pitfall 3: Dangling references
class Owner3 {
public:
    std::unique_ptr<Pitfall> owned = std::make_unique<Pitfall>();

    // DANGEROUS: returning reference to owned object
    Pitfall& get_ref() { return *owned; }
};

void PitfallDangling() {
    std::cout << "\n=== Pitfall: Dangling References ===" << std::endl;

    std::cout << "PROBLEM:" << std::endl;
    std::cout << "  Owner has unique_ptr<T>" << std::endl;
    std::cout << "  Caller gets T& reference" << std::endl;
    std::cout << "  Owner destroyed → reference dangles!" << std::endl;

    Pitfall* dangling = nullptr;

    {
        Owner3 owner;
        Pitfall& ref = owner.get_ref();
        dangling = &ref;
        std::cout << "  Got reference inside scope" << std::endl;
    }

    // dangling now points to destroyed object
    // std::cout << *dangling;  // UB!

    std::cout << "  Reference is now dangling" << std::endl;
    std::cout << "SOLUTION: Return shared_ptr, or keep owner alive" << std::endl;
}

// Pitfall 4: Mixing raw and smart pointers
void PitfallMixed() {
    std::cout << "\n=== Pitfall: Mixing Raw and Smart ===" << std::endl;

    std::cout << "PROBLEM:" << std::endl;
    std::cout << "  Pitfall* raw = new Pitfall();" << std::endl;
    std::cout << "  std::unique_ptr<Pitfall> smart(raw);" << std::endl;
    std::cout << "  delete raw;  // DOUBLE DELETE!" << std::endl;

    std::cout << "\nRULE:" << std::endl;
    std::cout << "  Once managed by smart pointer:" << std::endl;
    std::cout << "  NEVER use delete on it" << std::endl;
    std::cout << "  NEVER create another smart_ptr from raw" << std::endl;
}

// Pitfall 5: get() misuse
void PitfallGetMisuse() {
    std::cout << "\n=== Pitfall: get() Misuse ===" << std::endl;

    std::cout << "PROBLEM with get():" << std::endl;
    std::cout << "  auto raw = smart.get();" << std::endl;
    std::cout << "  delete raw;  // Smart pointer will delete AGAIN!" << std::endl;

    std::cout << "\nget() is ONLY for:" << std::endl;
    std::cout << "  Passing to C APIs that don't take ownership" << std::endl;
    std::cout << "  Temporary observation" << std::endl;

    std::cout << "\nNever:" << std::endl;
    std::cout << "  Store get() result long-term" << std::endl;
    std::cout << "  Delete the raw pointer" << std::endl;
    std::cout << "  Create new smart_ptr from it" << std::endl;
}

// Pitfall 6: shared_ptr in multithreading
void PitfallThreading() {
    std::cout << "\n=== Pitfall: Threading ===" << std::endl;

    std::cout << "MYTH: shared_ptr makes object access thread-safe" << std::endl;
    std::cout << "FACT: Only reference counting is thread-safe" << std::endl;

    std::cout << "\nStill need to synchronize:" << std::endl;
    std::cout << "  Access to the pointed-to object" << std::endl;
    std::cout << "  Reading/writing same shared_ptr variable" << std::endl;
}

// Pitfall 7: Overusing shared_ptr
void PitfallOveruse() {
    std::cout << "\n=== Pitfall: Overuse ===" << std::endl;

    std::cout << "shared_ptr is NOT a default choice:" << std::endl;
    std::cout << "  Has overhead (atomic counting, control block)" << std::endl;
    std::cout << "  Can hide ownership design problems" << std::endl;

    std::cout << "\nPrefer:" << std::endl;
    std::cout << "  unique_ptr by default" << std::endl;
    std::cout << "  Raw references for observation" << std::endl;
    std::cout << "  shared_ptr only when truly needed" << std::endl;

    std::cout << "\nAsk yourself:" << std::endl;
    std::cout << "  'Is ownership actually shared?'" << std::endl;
    std::cout << "  'Who is responsible for lifetime?'" << std::endl;
}