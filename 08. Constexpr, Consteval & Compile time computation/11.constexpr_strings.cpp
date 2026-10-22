/*
 * ============================================================================
 * CONSTEXPR STRINGS - STRING PROCESSING AT COMPILE TIME
 * ============================================================================
 *
 * WHAT CAN WE DO WITH STRINGS AT COMPILE TIME?
 * ===========================================
 * - Compute lengths
 * - Compare strings
 * - Concatenate/build strings (in limited ways)
 * - Hash strings (for fast lookup)
 * - Parse literals (for validation)
 *
 * ============================================================================
 */


import std;
#include <iostream>
#include <string>
#include <string_view>
#include "constexpr_header.h"

void CompileTimeStringLength() {
    std::cout << "=== Compile-Time String Operations ===" << std::endl;

    // String length at compile time:
    constexpr size_t my_strlen(const char* s) {
        size_t len = 0;
        while(s[len] != '\0') ++len;
        return len;
    }

    constexpr size_t len = my_strlen("Hello, constexpr!");
    static_assert(len == 17);
    std::cout << "  Length: " << len << " (compile-time)" << std::endl;

    // String comparison at compile time:
    constexpr bool str_equal(const char* a, const char* b) {
        size_t i = 0;
        while(a[i] != '\0' && b[i] != '\0') {
            if(a[i] != b[i]) return false;
            ++i;
        }
        return a[i] == b[i];  // Both must be at end
    }

    static_assert(str_equal("hello", "hello"));
    static_assert(!str_equal("hello", "world"));
    std::cout << "  \"hello\" == \"hello\": " << str_equal("hello", "hello") << std::endl;
}

void CompileTimeHashing() {
    std::cout << "\n=== Compile-Time String Hashing ===" << std::endl;

    /*
     * STRING HASHING AT COMPILE TIME:
     * ================================================================
     * Perfect for switch-on-string patterns!
     */

    constexpr uint32_t hash_string(const char* s) {
        uint32_t hash = 5381;
        while(*s) {
            hash = ((hash << 5) + hash) + *s++;
        }
        return hash;
    }

    // Hash known at compile time:
    constexpr auto hash_hello = hash_string("hello");
    constexpr auto hash_world = hash_string("world");

    std::cout << "  hash(\"hello\") = " << hash_hello << std::endl;
    std::cout << "  hash(\"world\") = " << hash_world << std::endl;

    // Switch on strings (via their hashes):
    constexpr auto get_string_id = [](const char* s) {
        constexpr auto H_HELLO = hash_string("hello");
        constexpr auto H_WORLD = hash_string("world");
        constexpr auto H_FOO = hash_string("foo");

        uint32_t h = hash_string(s);  // Note: this is runtime in this context
        if(h == H_HELLO) return 1;
        if(h == H_WORLD) return 2;
        if(h == H_FOO) return 3;
        return 0;
        };

    std::cout << "  ID for \"hello\": " << get_string_id("hello") << std::endl;
}