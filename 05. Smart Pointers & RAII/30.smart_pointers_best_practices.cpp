import std;
#include "smart_pointers_header.h"

void BestPracticesSummary() {
    std::cout << "=== Smart Pointer Best Practices ===" << std::endl;

    std::cout << "1. DEFAULT to unique_ptr:" << std::endl;
    std::cout << "   auto ptr = std::make_unique<T>(args);" << std::endl;
    std::cout << "   Zero overhead, clear ownership" << std::endl;

    std::cout << "2. Use shared_ptr ONLY when needed:" << std::endl;
    std::cout << "   When ownership is genuinely shared" << std::endl;
    std::cout << "   When lifetime is unclear" << std::endl;

    std::cout << "3. Use weak_ptr for non-owning observation:" << std::endl;
    std::cout << "   Breaks cycles" << std::endl;
    std::cout << "   Cache references" << std::endl;

    std::cout << "4. Always use make_* functions:" << std::endl;
    std::cout << "   make_unique (C++14)" << std::endl;
    std::cout << "   make_shared (always available)" << std::endl;

    std::cout << "5. Express ownership in APIs:" << std::endl;
    std::cout << "   unique_ptr<T> param → taking ownership" << std::endl;
    std::cout << "   shared_ptr<T> param → sharing ownership" << std::endl;
    std::cout << "   T&/T* param → just observing" << std::endl;
}

void OwnershipGuidelines() {
    std::cout << "\n=== Ownership Guidelines ===" << std::endl;

    std::cout << "Choose based on ownership:" << std::endl;
    std::cout << "  Exclusive ownership → unique_ptr" << std::endl;
    std::cout << "  Shared ownership → shared_ptr" << std::endl;
    std::cout << "  Non-owning observation → raw ref/ptr or weak_ptr" << std::endl;

    std::cout << "Ask yourself:" << std::endl;
    std::cout << "  'Who owns this object?'" << std::endl;
    std::cout << "  'Who is responsible for deleting it?'" << std::endl;
    std::cout << "  'When should it be deleted?'" << std::endl;
}

void APIDesignGuidelines() {
    std::cout << "\n=== API Design Guidelines ===" << std::endl;

    std::cout << "Function parameters:" << std::endl;
    std::cout << "  Take ownership: void f(unique_ptr<T>)" << std::endl;
    std::cout << "  Share ownership: void f(shared_ptr<T>)" << std::endl;
    std::cout << "  Observe only: void f(const T&)" << std::endl;
    std::cout << std::endl;

    std::cout << "Return values:" << std::endl;
    std::cout << "  Factory: unique_ptr<T> create()" << std::endl;
    std::cout << "  Shared resource: shared_ptr<T> get()" << std::endl;
    std::cout << "  Observation: const T& get() (careful with lifetime!)" << std::endl;

    std::cout << "Member variables:" << std::endl;
    std::cout << "  'HAS-A' relationship → unique_ptr member" << std::endl;
    std::cout << "  Shared component → shared_ptr member" << std::endl;
    std::cout << "  Reference to parent → weak_ptr member" << std::endl;
}

void CommonPatterns() {
    std::cout << "\n=== Common Patterns ===" << std::endl;

    std::cout << "1. Factory pattern:" << std::endl;
    std::cout << "   auto create() {" << std::endl;
    std::cout << "       return std::make_unique<T>(args);" << std::endl;
    std::cout << "   }" << std::endl;

    std::cout << "2. PIMPL:" << std::endl;
    std::cout << "   class Widget {" << std::endl;
    std::cout << "       struct Impl;" << std::endl;
    std::cout << "       std::unique_ptr<Impl> pimpl;" << std::endl;
    std::cout << "   };" << std::endl;

    std::cout << "3. Observer with weak_ptr:" << std::endl;
    std::cout << "   std::vector<std::weak_ptr<Observer>> observers;" << std::endl;

    std::cout << "4. Self-reference (enable_shared_from_this):" << std::endl;
    std::cout << "   class T : std::enable_shared_from_this<T> {" << std::endl;
    std::cout << "       auto get_self() { return shared_from_this(); }" << std::endl;
    std::cout << "   };" << std::endl;
}

void DecisionTree() {
    std::cout << "\n=== Decision Tree ===" << std::endl;

    std::cout << "Choosing smart pointer type:" << std::endl;

    std::cout << "  Is ownership exclusive?" << std::endl;
    std::cout << "    YES → unique_ptr" << std::endl;
    std::cout << "    NO ↓" << std::endl;
    std::cout << std::endl;
    std::cout << "  Is ownership shared?" << std::endl;
    std::cout << "    YES → shared_ptr" << std::endl;
    std::cout << "    NO ↓" << std::endl;
    std::cout << std::endl;
    std::cout << "  Just observing?" << std::endl;
    std::cout << "    YES → raw reference/pointer OR weak_ptr" << std::endl;
    std::cout << std::endl;

    std::cout << "Additional considerations:" << std::endl;
    std::cout << "  Cyclic reference? → one side uses weak_ptr" << std::endl;
    std::cout << "  Need self shared_ptr? → enable_shared_from_this" << std::endl;
    std::cout << "  C API interop? → custom deleter" << std::endl;
}

void MigrationGuide() {
    std::cout << "\n=== Migration from Raw Pointers ===" << std::endl;

    std::cout << "Old code → New code:" << std::endl;

    std::cout << "  T* p = new T();" << std::endl;
    std::cout << "  → auto p = std::make_unique<T>();" << std::endl;

    std::cout << "  delete p;" << std::endl;
    std::cout << "  → p.reset(); (or let scope end)" << std::endl;

    std::cout << "  T* p = new T[n];" << std::endl;
    std::cout << "  → auto p = std::make_unique<T[]>(n);" << std::endl;

    std::cout << "  // Or better:" << std::endl;
    std::cout << "  → std::vector<T> v(n);" << std::endl;

    std::cout << "  Ownership transfer: return p;" << std::endl;
    std::cout << "  → return std::move(p); (with unique_ptr)" << std::endl;

    std::cout << "  Observation: const T& ref = *p;" << std::endl;
    std::cout << "  → Same, but lifetime guaranteed by smart pointer" << std::endl;
}