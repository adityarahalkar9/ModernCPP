import std;
#include "templates_header.h"

// Compile-time string processing
constexpr size_t CTStringLength(const char* str) {
    size_t len = 0;
    while(str[len] != '\0') ++len;
    return len;
}

void BasicCompileTimeStrings() {
    std::cout << "=== Compile-Time Strings ===" << std::endl;

    constexpr size_t len = CTStringLength("Hello, World!");
    static_assert(len == 13);

    std::cout << "  Length of \"Hello, World!\": " << len << std::endl;
}

// Compile-time string operations
constexpr bool CTEquals(std::string_view a, std::string_view b) {
    if(a.size() != b.size()) return false;
    for(size_t i = 0; i < a.size(); ++i) {
        if(a[i] != b[i]) return false;
    }
    return true;
}

void CompileTimeStringComparison() {
    std::cout << "\n=== String Comparison ===" << std::endl;

    constexpr bool equal = CTEquals("hello", "hello");
    constexpr bool not_equal = CTEquals("hello", "world");

    static_assert(equal);
    static_assert(!not_equal);

    std::cout << "  \"hello\" == \"hello\": " << equal << std::endl;
    std::cout << "  \"hello\" == \"world\": " << not_equal << std::endl;
}

// Compile-time string concatenation
template <size_t N1, size_t N2>
constexpr auto Concat(const char(&s1)[N1], const char(&s2)[N2]) {
    std::array<char, N1 + N2 - 1> result{};
    size_t idx = 0;

    for(size_t i = 0; i < N1 - 1; ++i) {
        result[idx++] = s1[i];
    }
    for(size_t i = 0; i < N2 - 1; ++i) {
        result[idx++] = s2[i];
    }

    return result;
}

void CompileTimeConcat() {
    std::cout << "\n=== String Concatenation ===" std::endl;

    constexpr auto combined = Concat("Hello, ", "World!");
    static_assert(combined.size() == 13);

    std::cout << "  Combined: ";
    for(char c : combined) {
        std::cout << c;
    }
    std::cout << std::endl;
}

// Compile-time string hashing
constexpr uint32_t ConstexprHash(const char* str) {
    uint32_t hash = 5381;
    while(*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash;
}

void CompileTimeHashing() {
    std::cout << "\n=== Compile-Time Hashing ===" << std::endl;

    constexpr auto hash1 = ConstexprHash("hello");
    constexpr auto hash2 = ConstexprHash("hello");
    constexpr auto hash3 = ConstexprHash("world");

    static_assert(hash1 == hash2);
    static_assert(hash1 != hash3);

    std::cout << "  hash(\"hello\") = " << hash1 << std::endl;
    std::cout << "  hash(\"world\") = " << hash3 << std::endl;
}

// String templates
template <char... Chars>
struct StringTemplate {
    static constexpr const char* value = Chars...;
    static constexpr size_t size = sizeof...(Chars);
};

void StringTemplates() {
    std::cout << "\n=== String Templates ===" << std::endl;

    std::cout << "C++20 allows class types as non-type params:" << std::endl;
    std::cout << "  template <char... Chars>" << std::endl;
    std::cout << "  struct StringTemplate { ... };" << std::endl;
}