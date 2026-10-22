import std;
/*
 * ============================================================================
 * FUNCTION COMPOSITION - BUILDING PIPELINES
 * ============================================================================
 *
 * WHAT IS FUNCTION COMPOSITION?
 * =============================
 * Combining two or more functions to create a NEW function.
 * The output of one becomes the input of the next.
 *
 * MATH NOTATION:
 * ==============
 *   (f ∘ g)(x) = f(g(x))
 *   "f composed with g, applied to x"
 *   First apply g, then apply f to the result.
 *
 * REAL-WORLD ANALOGY:
 * ===================
 * Think of an assembly line in a factory:
 *   Raw material → Station 1 → Station 2 → Station 3 → Finished product
 *
 * Each station does ONE thing well. The power comes from CHAINING them.
 *
 * WHY COMPOSE?
 * ============
 * 1. Build complex behavior from simple pieces
 * 2. Each piece is testable in isolation
 * 3. Easy to rearrange or add steps
 * 4. Reads like a description of what happens
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "functional_header.h"

void BasicComposition() {
    std::cout << "=== Basic Function Composition ===" << std::endl;

    /*
     * MANUAL COMPOSITION:
     * ==================
     * You've been composing functions all along, just manually:
     */

    auto add_one = [](int x) { return x + 1; };
    auto double_it = [](int x) { return x * 2; };

    // Manual composition: call one, feed result to next
    int step1 = add_one(5);      // 5 + 1 = 6
    int step2 = double_it(step1); // 6 * 2 = 12

    std::cout << "  Manual: add_one then double = " << step2 << std::endl;

    /*
     * AUTOMATIC COMPOSITION:
     * ======================
     * Write a function that does the composing FOR us:
     */

     // A compose function: takes two functions, returns their composition
    auto compose = [](auto f, auto g) {
        // Returns a new function that applies g first, then f
        return [f, g](auto value) {
            return f(g(value));  // Apply g, then apply f to result
            };
        };

    // Create a composed function:
    auto add_then_double = compose(double_it, add_one);
    // Reading: "double_it composed with add_one"
    // Means: first add_one, then double_it

    std::cout << "  Composed: " << add_then_double(5) << std::endl;  // (5+1)*2 = 12
}

void ChainedComposition() {
    std::cout << "\n=== Chaining Multiple Functions ===" << std::endl;

    /*
     * COMPOSING MULTIPLE FUNCTIONS:
     * ============================
     * We can chain compose calls to build longer pipelines.
     */

    auto compose = [](auto f, auto g) {
        return [f, g](auto value) {
            return f(g(value));
            };
        };

    // Individual building blocks:
    auto add_one = [](int x) { return x + 1; };
    auto double_it = [](int x) { return x * 2; };
    auto subtract_three = [](int x) { return x - 3; };
    auto square = [](int x) { return x * x; };

    // Build a pipeline: ((x+1)*2 - 3)^2
    // Reading right-to-left: first add_one, then double, then subtract, then square
    auto pipeline = compose(square,           // 4th: square it
                            compose(subtract_three,    // 3rd: subtract 3
                                    compose(double_it,         // 2nd: double it
                                            add_one)));        // 1st: add one

    // Apply the pipeline:
    int input = 4;
    // Step 1: 4 + 1 = 5
    // Step 2: 5 * 2 = 10
    // Step 3: 10 - 3 = 7
    // Step 4: 7^2 = 49
    std::cout << "  Pipeline(4) = " << pipeline(4) << " (expected 49)" << std::endl;

    /*
     * THE DOWNSIDE:
     * =============
     * Nested compose calls are hard to read.
     * C++20 ranges/views provide better syntax for this.
     */
}

void StringProcessingPipeline() {
    std::cout << "\n=== Practical Pipeline: String Processing ===" << std::endl;

    /*
     * A REAL-WORLD EXAMPLE: Text processing pipeline
     * ===============================================
     * Process a list of words through multiple transformations.
     */

    std::vector<std::string> input = {
        "  Hello  ", "  World  ", "  Foo  ", "  Bar  "
    };

    // Individual transformation functions:

    // Step 1: Trim whitespace
    auto trim = [](std::string s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        if(start == std::string::npos) return std::string{};
        return s.substr(start, end - start + 1);
        };

    // Step 2: Convert to lowercase
    auto to_lower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return s;
        };

    // Step 3: Add prefix
    auto add_prefix = [](std::string s) {
        return "processed_" + s;
        };

    // Step 4: Add suffix
    auto add_suffix = [](std::string s) {
        return s + "_v1";
        };

    // Apply the pipeline to each element:
    std::cout << "  Processing results:" << std::endl;
    for(const auto& str : input) {
        // The composition chain - output of each feeds into the next
        std::string result = str;
        result = trim(result);        // Step 1
        result = to_lower(result);    // Step 2
        result = add_prefix(result);  // Step 3
        result = add_suffix(result);  // Step 4

        std::cout << "    \"" << str << "\" -> \"" << result << "\"" << std::endl;
    }
}

void DataAnalysisPipeline() {
    std::cout << "\n=== Data Analysis Pipeline ===" << std::endl;

    /*
     * PIPELINE FOR DATA ANALYSIS:
     * ==========================
     * Filter → Transform → Aggregate
     * This is the classic data processing pattern.
     */

    struct Employee {
        std::string name;
        std::string department;
        double salary;
    };

    std::vector<Employee> employees = {
        {"Alice", "Engineering", 85000},
        {"Bob", "Sales", 45000},
        {"Charlie", "Engineering", 95000},
        {"Diana", "Marketing", 55000},
        {"Eve", "Engineering", 75000},
        {"Frank", "Sales", 50000},
    };

    // TASK: Average salary of Engineering department

    // Step 1: FILTER - only engineering
    std::vector<Employee> engineers;
    std::copy_if(employees.begin(), employees.end(),
                 std::back_inserter(engineers),
                 [](const Employee& e) { return e.department == "Engineering"; });

    // Step 2: TRANSFORM - extract salaries
    std::vector<double> salaries;
    std::transform(engineers.begin(), engineers.end(),
                   std::back_inserter(salaries),
                   [](const Employee& e) { return e.salary; });

    // Step 3: AGGREGATE - compute average
    double total = std::accumulate(salaries.begin(), salaries.end(), 0.0);
    double average = total / salaries.size();

    std::cout << "  Engineering employees: " << engineers.size() << std::endl;
    std::cout << "  Average salary: $" << average << std::endl;

    /*
     * IN C++20, THIS WOULD BE:
     * =======================
     * auto avg = employees
     *     | std::views::filter([](auto& e) { return e.department == "Engineering"; })
     *     | std::views::transform([](auto& e) { return e.salary; })
     *     // ... then average
     */
}