/*
    CONSTEXPR CONTAINERS (C++20)
    
    THE C++20 REVOLUTION:

 * C++20 made std::vector, std::string, and other containers
 * usable in constexpr contexts!
 *
 * WHAT THIS MEANS:
 * ================
 * Before C++20:
 *   - Only fixed-size containers (std::array) could be constexpr
 *   - std::vector was IMPOSSIBLE at compile time
 *
 * With C++20:
 *   - std::vector works in constexpr (within limits)
 *   - std::string works in constexpr
 *   - You can build containers AT COMPILE TIME!
 *
 * THE CATCH:
 * ==========
 * The container can't "escape" the constexpr context.
 * You can use it during compile-time computation, but the final
 * result must be converted to something persistent.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "constexpr_header.h"

void ConstexprVectorBasics() {
    std::cout << "=== constexpr std::vector (C++20) ===" << std::endl;

    /*
     * WHAT CHANGED IN C++20:
     * ================================================================
     * Many std::vector operations became constexpr:
     * - push_back, emplace_back
     * - size, empty
     * - operator[], at
     * - begin, end
     * - etc.
     */

     // This function USES a vector during compile-time evaluation
    constexpr auto compute_with_vector = []() {
        std::vector<int> vec;  // Vector at COMPILE TIME!

        // Build it up:
        for(int i = 1; i <= 10; ++i) {
            vec.push_back(i * i);  // Squares: 1, 4, 9, 16...
        }

        // Use it:
        int sum = 0;
        for(int v : vec) {
            sum += v;
        }

        return sum;
        // Note: 'vec' itself doesn't survive to runtime
        // But we computed what we needed from it!
        };

    // Call it at compile time:
    constexpr int sum_of_squares = compute_with_vector();
    static_assert(sum_of_squares == 385);  // 1+4+9+16+25+36+49+64+81+100 = 385

    std::cout << "  Sum of first 10 squares: " << sum_of_squares << std::endl;
    std::cout << "  (Vector was used at compile time!)" << std::endl;

    /*
     * THE LIMITATION:
     * ===============
     * The vector exists only DURING compile-time evaluation.
     * It can't be the final result (where would the memory live?).
     * But you can compute things WITH it and return those results.
     */
}

void ConstexprString() {
    std::cout << "\n=== constexpr std::string (C++20) ===" << std::endl;

    /*
     * STRING MANIPULATION AT COMPILE TIME:
     * ================================================================
     */

     // String processing at compile time:
    constexpr auto process_string = []() {
        std::string s = "Hello, World!";

        // Transform at compile time:
        for(char& c : s) {
            if(c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';  // Convert to lowercase
            }
        }

        // Count vowels:
        int vowels = 0;
        for(char c : s) {
            if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                ++vowels;
            }
        }

        return vowels;
        };

    constexpr int vowel_count = process_string();
    static_assert(vowel_count == 3);  // "hello, world!" has 3 vowels (e,o,o)

    std::cout << "  Vowels in 'Hello, World!' (lowercased): " << vowel_count << std::endl;

    // String building at compile time:
    constexpr auto build_string = []() {
        std::string result;
        for(int i = 1; i <= 5; ++i) {
            result += std::to_string(i);  // "1" + "2" + "3" + "4" + "5"
            if(i < 5) result += ",";
        }
        return result.size();  // "1,2,3,4,5" -> length 9
        };

    constexpr size_t length = build_string();
    static_assert(length == 9);

    std::cout << "  Built string length: " << length << std::endl;
}

void FromVectorToArray() {
    std::cout << "\n=== Pattern: Vector → Array Conversion ===" << std::endl;

    /*
     * THE PATTERN: Since vectors can't survive to runtime,
     * we convert them to std::array (which CAN be constexpr).
     *
     * This is the standard workaround!
     */

     // Helper: Convert a compile-time vector computation to an array
    template <size_t N>
    constexpr auto make_fibonacci_array() {
        // First compute with a vector (dynamic, easy to use)
        std::vector<int> fib;
        fib.push_back(0);
        fib.push_back(1);
        while(fib.size() < N) {
            fib.push_back(fib[fib.size() - 1] + fib[fib.size() - 2]);
        }

        // Now convert to array (can persist to runtime)
        std::array<int, N> result{};
        for(size_t i = 0; i < N; ++i) {
            result[i] = fib[i];
        }

        return result;  // std::array CAN be constexpr!
    }

    // Use it: Fibonacci table computed at compile time
    constexpr auto fib10 = make_fibonacci_array<10>();

    // Verify at compile time:
    static_assert(fib10[0] == 0);
    static_assert(fib10[1] == 1);
    static_assert(fib10[9] == 34);

    std::cout << "  Fibonacci (first 10): ";
    for(int f : fib10) {
        std::cout << f << " ";
    }
    std::cout << std::endl;
    std::cout << "  (Computed at compile time, stored as array!)" << std::endl;

    std::cout << R"(
    THE PATTERN:
    ┌─────────────────────────────────────────────────────┐
    │ 1. Use std::vector for BUILDING (at compile time)   │
    │ 2. Convert to std::array for STORING (to runtime)   │
    │                                                     │
    │ constexpr auto build_table() {                      │
    │     std::vector<T> temp;  // Build phase            │
    │     // ... fill temp ...                           │
    │     std::array<T, SIZE> result;  // Storage phase   │
    │     // ... copy from temp ...                      │
    │     return result;  // This CAN be constexpr!       │
    │ }                                                   │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}