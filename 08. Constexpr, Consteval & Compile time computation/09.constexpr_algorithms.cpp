/*
 * ============================================================================
 * CONSTEXPR ALGORITHMS (C++20) - STL AT COMPILE TIME
 * ============================================================================
 *
 * WHAT CHANGED IN C++20:
 * =====================
 * Many STL algorithms became constexpr:
 * - std::sort, std::find, std::count
 * - std::transform, std::accumulate
 * - std::all_of, std::any_of, std::none_of
 * - And many more!
 *
 * THE IMPLICATION:
 * =================
 * You can sort, search, transform data AT COMPILE TIME!
 * The results are baked into your binary.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include "constexpr_header.h"

void ConstexprSorting() {
    std::cout << "=== Compile-Time Sorting ===" << std::endl;

    /*
     * SORTING AT COMPILE TIME:
     * ================================================================
     */

     // Sort at compile time:
    constexpr auto sorted_array = []() {
        std::array<int, 8> data = {42, 8, 15, 4, 23, 16, 1, 99};

        // Sort it - AT COMPILE TIME!
        std::sort(data.begin(), data.end());

        return data;
        }();

    // Verify at compile time:
    static_assert(sorted_array[0] == 1);
    static_assert(sorted_array[7] == 99);

    std::cout << "  Sorted: ";
    for(int v : sorted_array) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    std::cout << "  (Sorted during compilation!)" << std::endl;
}

void ConstexprSearching() {
    std::cout << "\n=== Compile-Time Searching ===" << std::endl;

    /*
     * SEARCH ALGORITHMS AT COMPILE TIME:
     * ================================================================
     */

    constexpr auto search_demo = []() {
        std::array<int, 10> data = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

        // Binary search (compile time!):
        bool found = std::binary_search(data.begin(), data.end(), 11);

        // Find specific element:
        auto it = std::find(data.begin(), data.end(), 13);
        bool found_13 = (it != data.end());

        // Count elements matching condition:
        auto odd_count = std::count_if(data.begin(), data.end(),
                                       [](int x) { return x % 2 != 0; });

        return found && found_13 && (odd_count == 10);
        };

    constexpr bool all_found = search_demo();
    static_assert(all_found);

    std::cout << "  All search operations succeeded at compile time!" << std::endl;
}

void ConstexprTransformation() {
    std::cout << "\n=== Compile-Time Transformations ===" << std::endl;

    /*
     * TRANSFORMATIONS AT COMPILE TIME:
     * ================================================================
     */

     // Transform at compile time:
    constexpr auto transformed = []() {
        std::array<int, 5> input = {1, 2, 3, 4, 5};
        std::array<int, 5> output{};

        // Transform: square each element
        std::transform(input.begin(), input.end(), output.begin(),
                       [](int x) { return x * x; });

        return output;
        }();

    static_assert(transformed[0] == 1);
    static_assert(transformed[4] == 25);

    std::cout << "  Squares: ";
    for(int v : transformed) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // Accumulate at compile time:
    constexpr auto sum = []() {
        std::array<int, 5> data = {1, 2, 3, 4, 5};
        return std::accumulate(data.begin(), data.end(), 0);
        }();

    static_assert(sum == 15);
    std::cout << "  Sum (accumulate): " << sum << std::endl;

    std::cout << R"(
    AVAILABLE CONSTEXPR ALGORITHMS (C++20):
    ┌─────────────────────────────────────────────────────┐
    │ • std::sort, std::stable_sort                       │
    │ • std::find, std::find_if                          │
    │ • std::count, std::count_if                        │
    │ • std::transform                                    │
    │ • std::accumulate                                   │
    │ • std::all_of, std::any_of, std::none_of           │
    │ • std::binary_search                                │
    │ • std::min_element, std::max_element               │
    │ • std::copy, std::copy_if                          │
    │ • std::equal, std::mismatch                        │
    │ • ... and many more!                                │
    └─────────────────────────────────────────────────────┘

    PRACTICAL USE:
    • Precomputed lookup tables
    • Sorted constant data
    • Validated data structures
    • Algorithm results that never change
)" << std::endl;
}