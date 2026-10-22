import std;
/*
 * ============================================================================
 * IIFE - IMMEDIATELY INVOKED FUNCTION EXPRESSIONS
 * ============================================================================
 *
 * WHAT IS AN IIFE?
 * ================
 * An IIFE (pronounced "iffy") is a lambda that is defined AND called
 * in a single expression. You write the lambda and immediately
 * put () after it to invoke it right away.
 *
 * SYNTAX:
 * =======
 *   auto result = [](parameters) { body }(arguments);
 *                                    ^^^^^^^^^^
 *                                    Call it immediately!
 *
 * Or with no arguments:
 *   auto result = []{ do_something(); }();
 *
 * WHY USE IIFEs?
 * ==============
 * 1. Complex initialization - compute a value with multiple steps,
 *    temporary variables, etc., without polluting the outer scope
 *
 * 2. Scope isolation - create a private scope for intermediate variables
 *
 * 3. Static initialization - run code once at static init time
 *
 * 4. Avoiding repetition - compute something expensive once and store it
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include "functional_header.h"

void BasicIIFE() {
    std::cout << "=== Basic IIFE ===" << std::endl;

    // A lambda, immediately called with arguments
    auto result = [](int a, int b) {
        return a + b;
        }(42, 58);  // <- The () invokes the lambda RIGHT NOW

    std::cout << "  IIFE result: " << result << std::endl;

    // Without IIFE, you'd need two statements:
    auto add = [](int a, int b) { return a + b; };  // Step 1: define
    auto result2 = add(42, 58);                      // Step 2: call
    // 'add' is still in scope - might pollute the namespace

    std::cout << "  Two-step result: " << result2 << std::endl;

    // The IIFE version doesn't leave 'add' lying around.
    // It's created, used, and immediately destroyed.
}

void ComplexInitialization() {
    std::cout << "\n=== Complex Initialization ===" << std::endl;

    /*
     * THE CLASSIC USE CASE: Complex initialization
     * ============================================
     * Suppose you need to compute a value that requires
     * several intermediate steps and temporary variables.
     *
     * WITHOUT IIFE: Your outer scope gets polluted with temporaries
     */
    {
        // Without IIFE - outer scope pollution:
        std::vector<int> temp_data;              // Temporary - pollutes scope
        temp_data = {1, 2, 3, 4, 5};

        int temp_sum = 0;                        // Another temporary
        for(int v : temp_data) temp_sum += v;

        double temp_avg = static_cast<double>(temp_sum) / temp_data.size();
        // ^ Now 'temp_data', 'temp_sum' are still here even though
        //   we only needed 'temp_avg'

        std::cout << "  (Without IIFE) avg = " << temp_avg << std::endl;
        // temp_data and temp_sum are STILL alive - scope pollution!
    }

    /*
     * WITH IIFE: Everything stays inside the lambda's scope
     */
    {
        // With IIFE - clean outer scope:
        double avg = []() {                       // IIFE begins
            std::vector<int> data = {1, 2, 3, 4, 5};   // Private to lambda
            int sum = 0;                                // Private to lambda
            for(int v : data) sum += v;
            return static_cast<double>(sum) / data.size();
            }();                                       // IIFE invoked, result captured
        // 'data' and 'sum' are GONE - only 'avg' exists in outer scope

        std::cout << "  (With IIFE) avg = " << avg << std::endl;
    }
}

void StaticInitialization() {
    std::cout << "\n=== Static Initialization ===" << std::endl;

    /*
     * STATIC IIFEs: Run once at program start
     * ========================================
     * A static local initialized with an IIFE runs the lambda
     * ONCE, the first time execution reaches this line.
     * The result is cached forever.
     */

     // This could be expensive (e.g., reading config, computing tables)
    auto get_config = []() -> std::string {
        // Simulating expensive computation
        std::cout << "  [Computing config... (runs only ONCE)]" << std::endl;
        return "computed_config_value";
        };

    // Called multiple times - but lambda only runs once
    // (because we cache the result)
    static const std::string config = get_config();

    std::cout << "  Using config: " << config << std::endl;
    std::cout << "  Using again: " << config << " (no recomputation)" << std::endl;

    // Pattern: static IIFE directly
    static const auto table = []() {
        std::cout << "  [Building lookup table... (once)]" << std::endl;
        std::vector<int> result(10);
        for(int i = 0; i < 10; ++i) {
            result[i] = i * i;
        }
        return result;
        }();

    std::cout << "  Table[7] = " << table[7] << " (49)" << std::endl;
}

void ScopeIsolation() {
    std::cout << "\n=== Scope Isolation ===" << std::endl;

    /*
     * Using IIFE to create a PRIVATE SCOPE
     * ====================================
     * Sometimes you need variables temporarily - for setup,
     * computation, etc. IIFE lets them disappear when done.
     */

    int outer_var = 100;  // This belongs to the outer scope

    // IIFE creates a private scope for these computations
    auto result = [&outer_var]() {
        // Everything here is PRIVATE to this lambda invocation
        int intermediate1 = outer_var * 2;
        int intermediate2 = intermediate1 + 10;
        int intermediate3 = intermediate2 * 3;

        // Modify the outer variable if needed
        outer_var = intermediate3;

        return intermediate3;
        }();  // Execute immediately

    std::cout << "  Result: " << result << std::endl;
    std::cout << "  outer_var modified to: " << outer_var << std::endl;
    // intermediate1, 2, 3 are gone - they never polluted outer scope
}