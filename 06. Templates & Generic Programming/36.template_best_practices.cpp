import std;
#include "templates_header.h"

void BestPracticesList() {
    std::cout << "=== Template Best Practices ===" << std::endl;

    std::cout << "1. Prefer concepts over SFINAE (C++20):" << std::endl;
    std::cout << "   Better error messages, clearer intent" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Use auto for template parameters (C++20):" << std::endl;
    std::cout << "   void func(auto x) instead of template" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Keep templates small:" << std::endl;
    std::cout << "   Large templates are hard to debug" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Use type aliases:" << std::endl;
    std::cout << "   Improve readability" << std::endl;
    std::cout << std::endl;

    std::cout << "5. Provide good error messages:" << std::endl;
    std::cout << "   static_assert with helpful text" << std::endl;
}

void DesignGuidelines() {
    std::cout << "\n=== Design Guidelines ===" << std::endl;

    std::cout << "Template design:" << std::endl;
    std::cout << "  1. Start with concrete implementation" << std::endl;
    std::cout << "  2. Then generalize" << std::endl;
    std::cout << "  3. Don't template prematurely" << std::endl;
    std::cout << std::endl;

    std::cout << "Interface design:" << std::endl;
    std::cout << "  1. Minimize requirements on T" << std::endl;
    std::cout << "  2. Document requirements" << std::endl;
    std::cout << "  3. Use concepts to enforce" << std::endl;
}

void ErrorHandlingBestPractices() {
    std::cout << "\n=== Error Handling ===" << std::endl;

    std::cout << "Good template errors:" << std::endl;
    std::cout << "  static_assert(condition, \"Helpful message\");" << std::endl;
    std::cout << std::endl;
    std::cout << "Concepts give better errors automatically" << std::endl;
    std::cout << "  'constraint not satisfied: Integral<T>'" << std::endl;

    std::cout << "\nBad template errors (SFINAE):" << std::endl;
    std::cout << "  Pages of instantiation errors" << std::endl;
    std::cout << "  Hard to find root cause" << std::endl;
}

void PerformanceBestPractices() {
    std::cout << "\n=== Performance ===" << std::endl;

    std::cout << "Templates and performance:" << std::endl;
    std::cout << "  1. Zero-cost abstractions possible" << std::endl;
    std::cout << "  2. Can be inlined aggressively" << std::endl;
    std::cout << "  3. Specialized for each type" << std::endl;
    std::cout << std::endl;

    std::cout << "Watch out for:" << std::endl;
    std::cout << "  1. Code bloat (many instantiations)" << std::endl;
    std::cout << "  2. Longer compile times" << std::endl;
    std::cout << "  3. Larger binaries" << std::endl;
}

void ModernGuidelines() {
    std::cout << "\n=== Modern C++ Guidelines ===" << std::endl;

    std::cout << "Evolution:" << std::endl;
    std::cout << "  C++98: Basic templates" << std::endl;
    std::cout << "  C++11: Variadic, alias templates" << std::endl;
    std::cout << "  C++14: Variable templates, auto return" << std::endl;
    std::cout << "  C++17: CTAD, fold expressions" << std::endl;
    std::cout << "  C++20: Concepts, abbreviated templates" << std::endl;

    std::cout << "\nUse latest features:" << std::endl;
    std::cout << "  Concepts > SFINAE" << std::endl;
    std::cout << "  auto params > explicit templates" << std::endl;
    std::cout << "  if constexpr > tag dispatch (sometimes)" << std::endl;
}