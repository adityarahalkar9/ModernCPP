import std;
/*
 * ============================================================================
 * FUNCTIONAL PATTERNS WITH STL ALGORITHMS
 * ============================================================================
 *
 * THE FUNCTIONAL APPROACH TO ALGORITHMS:
 * =====================================
 * Instead of writing loops manually, compose standard algorithms
 * with lambdas. This leads to:
 *   - Less code
 *   - Fewer bugs
 *   - Clearer intent
 *   - Often better performance (algorithms are optimized)
 *
 * THE KEY INSIGHT:
 * ================
 * Almost every loop you write is doing one of these:
 *   1. Transform each element     → std::transform
 *   2. Select some elements       → std::copy_if / remove_if
 *   3. Combine elements           → std::accumulate
 *   4. Find something             → std::find_if
 *   5. Check something            → std::all_of / any_of / none_of
 *   6. Do something for each      → std::for_each
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "functional_header.h"

void TransformExamples() {
    std::cout << "=== Transform Patterns ===" << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // PATTERN: Transform each element
    std::vector<int> doubled;
    std::transform(numbers.begin(), numbers.end(),
                   std::back_inserter(doubled),
                   [](int n) { return n * 2; });

    std::cout << "  Doubled: ";
    for(int n : doubled) std::cout << n << " ";
    std::cout << std::endl;

    // TRANSFORM IN PLACE: modify original
    std::transform(numbers.begin(), numbers.end(), numbers.begin(),
                   [](int n) { return n * n; });

    std::cout << "  Squared (in place): ";
    for(int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // TRANSFORM WITH INDEX: need position
    std::vector<std::string> indexed;
    std::transform(numbers.begin(), numbers.end(),
                   std::back_inserter(indexed),
                   [i = 0](int n) mutable {
                       return "#" + std::to_string(i++) + ":" + std::to_string(n);
                   });

    std::cout << "  With indices: ";
    for(const auto& s : indexed) std::cout << s << " ";
    std::cout << std::endl;
}

void FilterExamples() {
    std::cout << "\n=== Filter Patterns ===" << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // PATTERN: Keep elements matching a condition
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });

    std::cout << "  Evens: ";
    for(int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // MULTIPLE CONDITIONS: complex predicates
    std::vector<int> special;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(special),
                 [](int n) {
                     return n % 2 == 0 && n > 5;  // Even AND greater than 5
                 });

    std::cout << "  Even and > 5: ";
    for(int n : special) std::cout << n << " ";
    std::cout << std::endl;

    // FILTERING WITH STATE: use captured variables
    int threshold = 5;
    std::vector<int> above_threshold;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(above_threshold),
                 [threshold](int n) { return n > threshold; });

    std::cout << "  Above " << threshold << ": ";
    for(int n : above_threshold) std::cout << n << " ";
    std::cout << std::endl;
}

void FindExamples() {
    std::cout << "\n=== Find Patterns ===" << std::endl;

    std::vector<std::string> words = {
        "apple", "banana", "cherry", "date", "elderberry"
    };

    // FIND FIRST MATCHING: custom predicate
    auto it = std::find_if(words.begin(), words.end(),
                           [](const std::string& w) {
                               return w.length() > 6;  // First word longer than 6
                           });

    if(it != words.end()) {
        std::cout << "  First long word: " << *it << std::endl;
    }

    // FIND WITH CAPTURED STATE:
    std::string prefix = "ban";
    auto prefix_match = std::find_if(words.begin(), words.end(),
                                     [&prefix](const std::string& w) {
                                         return w.substr(0, prefix.length()) == prefix;
                                     });
    std::cout << "  Starts with 'ban': " << *prefix_match << std::endl;

    // COUNT MATCHING:
    auto count_e = std::count_if(words.begin(), words.end(),
                                 [](const std::string& w) {
                                     return w.find('e') != std::string::npos;
                                 });
    std::cout << "  Words containing 'e': " << count_e << std::endl;
}

void CheckExamples() {
    std::cout << "\n=== Check Patterns (all_of, any_of, none_of) ===" << std::endl;

    std::vector<int> numbers = {2, 4, 6, 8, 10};
    std::vector<int> mixed = {1, 2, 3, 4, 5};

    // ALL_OF: every element satisfies the predicate?
    bool all_even = std::all_of(numbers.begin(), numbers.end(),
                                [](int n) { return n % 2 == 0; });
    std::cout << "  All even in {2,4,6,8,10}: " << all_even << std::endl;

    bool mixed_all_even = std::all_of(mixed.begin(), mixed.end(),
                                      [](int n) { return n % 2 == 0; });
    std::cout << "  All even in {1,2,3,4,5}: " << mixed_all_even << std::endl;

    // ANY_OF: at least one satisfies?
    bool any_even = std::any_of(mixed.begin(), mixed.end(),
                                [](int n) { return n % 2 == 0; });
    std::cout << "  Any even in {1,2,3,4,5}: " << any_even << std::endl;

    // NONE_OF: no element satisfies?
    bool none_negative = std::none_of(numbers.begin(), numbers.end(),
                                      [](int n) { return n < 0; });
    std::cout << "  None negative in numbers: " << none_negative << std::endl;

    /*
     * THESE ARE EXTREMELY USEFUL FOR VALIDATION:
     * =========================================
     * if (std::all_of(items.begin(), items.end(), isValid)) {
     *     // All items are valid - proceed
     * }
     */
}

void FoldExamples() {
    std::cout << "\n=== Fold/Reduce Patterns ===" << std::endl;

    std::vector<std::string> words = {"Hello", " ", "World", "!", " How", " are", " you?"};

    // FOLD: combine all elements into one
    // std::accumulate with a custom operation

    // Sum of numbers:
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "  Sum: " << sum << std::endl;

    // Concatenate strings:
    std::string sentence = std::accumulate(words.begin(), words.end(),
                                           std::string(),
                                           [](const std::string& acc, const std::string& w) {
                                               return acc + w;
                                           });
    std::cout << "  Concatenated: \"" << sentence << "\"" << std::endl;

    // CUSTOM FOLD: build a complex result
    struct Stats {
        int count = 0;
        int sum = 0;
        int max = 0;
        int min = 1000000;
    };

    Stats stats = std::accumulate(numbers.begin(), numbers.end(),
                                  Stats{},
                                  [](Stats acc, int n) {
                                      acc.count++;
                                      acc.sum += n;
                                      acc.max = std::max(acc.max, n);
                                      acc.min = std::min(acc.min, n);
                                      return acc;
                                  });

    std::cout << "  Stats: count=" << stats.count
        << " sum=" << stats.sum
        << " max=" << stats.max
        << " min=" << stats.min << std::endl;
}