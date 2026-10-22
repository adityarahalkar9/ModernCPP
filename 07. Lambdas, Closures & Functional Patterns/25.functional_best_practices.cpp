import std;

#include <iostream>
#include <string>
#include <vector>
#include "functional_header.h"

void BestPracticesSummary() {
    std::cout << "=== Lambda Best Practices Summary ===" << std::endl;

    std::cout << R"(
    ╔═════════════════════════════════════════════════════╗
    ║        LAMBDA BEST PRACTICES CHEAT SHEET            ║
    ╚═════════════════════════════════════════════════════╝

    CAPTURE GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ • Be EXPLICIT: [x, &y] beats [=, &]                │
    │ • Small types → capture by value                   │
    │ • Large objects → capture by reference (careful!)  │
    │ • Lambda outlives variable → MUST capture by value │
    │ • In loops → capture loop variable by VALUE        │
    │ • In classes → prefer init-capture of members      │
    └─────────────────────────────────────────────────────┘

    PERFORMANCE GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ • Use auto for immediate use (enables inlining)    │
    │ • Use std::function only for STORAGE              │
    │ • Keep captures minimal                           │
    │ • Empty lambdas are FREE                          │
    │ • Prefer references for large captures            │
    │ • Hot loops: avoid std::function                  │
    └─────────────────────────────────────────────────────┘

    SAFETY GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ • NEVER return lambda with [&] captures           │
    │ • Be careful with [this] - the #1 crash source    │
    │ • Use shared_ptr for long-lived callbacks         │
    │ • In classes: use enable_shared_from_this         │
    │ • Document when lambda modifies captures          │
    │ • Use AddressSanitizer in development             │
    └─────────────────────────────────────────────────────┘

    READABILITY GUIDELINES:
    ┌─────────────────────────────────────────────────────┐
    │ • Keep lambda bodies SHORT                         │
    │ • Long logic → named function instead              │
    │ • Use meaningful names in init-captures           │
    │ • Comment non-obvious captures                    │
    │ • Group related lambdas together                  │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void WhenToUseWhat() {
    std::cout << "\n=== Decision Guide: When to Use What ===" << std::endl;

    std::cout << R"(
    DECISION TREE FOR LAMBDAS:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │  Is the logic SHORT (1-3 lines)?                    │
    │    YES → Lambda at call site                       │
    │    NO  → Named function                            │
    │                                                     │
    │  Is it used ONCE?                                   │
    │    YES → Inline lambda                             │
    │    NO  → Named function or variable                 │
    │                                                     │
    │  Does it need to CAPTURE state?                     │
    │    YES → Lambda with captures                       │
    │    NO  → Regular function works fine               │
    │                                                     │
    │  Will it be STORED for later?                       │
    │    YES → std::function (or template parameter)     │
    │    NO  → auto (better performance)                 │
    │                                                     │
    │  Is it RECURSIVE?                                   │
    │    YES → Named function (or C++23 deducing this)   │
    │    NO  → Lambda is fine                             │
    │                                                     │
    └─────────────────────────────────────────────────────┘

    FUNCTIONAL PATTERNS - WHEN TO USE:
    ┌─────────────────────────────────────────────────────┐
    │ COMPOSITION: When building pipelines               │
    │ CURRYING: When creating specialized functions      │
    │ MONADIC: When handling chains of fallible ops      │
    │ PATTERN MATCHING: When handling variant types      │
    │ LAZY EVALUATION: When computation is expensive     │
    │ MEMOIZATION: When same inputs → same outputs       │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void CommonMistakes() {
    std::cout << "\n=== Top Mistakes to Avoid ===" std::endl;

    std::cout << R"(
    THE HALL OF SHAME:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │ 1. Returning lambda with [&] captures              │
    │    → Dangling reference crash                       │
    │                                                     │
    │ 2. [this] in long-lived callbacks                  │
    │    → Object destroyed, lambda crashes              │
    │                                                     │
    │ 3. [=] with large objects in scope                  │
    │    → Accidental expensive copies                   │
    │                                                     │
    │ 4. [&loop_var] in loops                             │
    │    → All iterations share final value              │
    │                                                     │
    │ 5. std::function in hot loops                       │
    │    → 10-100x slower than direct call               │
    │                                                     │
    │ 6. mutable lambda expecting to modify original     │
    │    → Confusion: modifies copy, not original        │
    │                                                     │
    │ 7. Deeply nested lambdas                           │
    │    → Unreadable code (use named functions)         │
    │                                                     │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}