import std;

/*
 * ============================================================================
 * MUTABLE LAMBDAS - MODIFYING CAPTURED COPIES
 * ============================================================================
 *
 * WHY DOES 'mutable' EXIST?
 * =========================
 * By default, by-value captures are CONST inside the lambda.
 * This is a safety feature - it prevents accidentally modifying
 * what you might think is the original variable.
 *
 * [counter]() { ++counter; }  // ERROR: counter is a const copy
 *
 * The 'mutable' keyword LIFTS this const restriction:
 *
 * [counter]() mutable { ++counter; }  // OK: can now modify the COPY
 *
 * CRITICAL UNDERSTANDING:
 * =======================
 * 'mutable' lets you modify THE LAMBDA'S COPY.
 * The ORIGINAL outside variable is NEVER affected.
 * The modification PERSISTS between calls to the lambda
 * (because the copy is a member of the lambda object).
 *
 * THINK OF IT AS THE LAMBDA HAVING PRIVATE STATE:
 * ==============================================
 * A mutable lambda is like a little object with internal state
 * that changes each time you call it. It's a STATEFUL function.
 *
 * USE CASES:
 * ==========
 * - Counters (counting how many times the lambda was called)
 * - Caches (memoizing expensive computations)
 * - State machines (tracking current state)
 * - Unique ID generators
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include "functional_header.h"

void MutableBasics() {
    std::cout << "=== Mutable Lambda Basics ===" << std::endl;

    int initial = 100;

    // WITHOUT mutable: the captured copy is CONST - can't modify
    auto const_lambda = [initial]() {
        // ++initial;  // ERROR: initial is const in here
        return initial;
        };

    // WITH mutable: the captured copy can be MODIFIED
    // This modifies the LAMBDA'S private copy, not the outside variable
    auto mutable_lambda = [initial]() mutable {
        ++initial;  // OK: modifies the lambda's internal copy
        return initial;
        };

    std::cout << "  Outside 'initial' before: " << initial << std::endl;
    std::cout << "  const_lambda() = " << const_lambda() << std::endl;
    std::cout << "  mutable_lambda() = " << mutable_lambda() << " (101)" << std::endl;
    std::cout << "  mutable_lambda() = " << mutable_lambda() << " (102)" << std::endl;
    std::cout << "  mutable_lambda() = " << mutable_lambda() << " (103)" << std::endl;
    std::cout << "  Outside 'initial' after: " << initial
        << " (still 100!)" << std::endl;

    /*
     * WHAT'S HAPPENING:
     * =================
     * The 'mutable_lambda' object has an internal 'initial' member.
     * Each call to operator() can modify that member.
     * The member persists across calls because the lambda OBJECT persists.
     *
     * Generated class looks like:
     *   class LambdaType {
     *       int initial;  // mutable member
     *   public:
     *       // Note: NOT const because of 'mutable'
     *       int operator()() { return ++initial; }
     *   };
     */
}

void StatefulCounter() {
    std::cout << "\n=== Pattern: Stateful Counter ===" << std::endl;

    /*
     * THE CLASSIC MUTABLE LAMBDA: A COUNTER
     * =====================================
     * This demonstrates the "closure with state" concept.
     * The lambda remembers how many times it's been called.
     */

     // Create a counter starting at 0
    auto counter = [count = 0]() mutable {   // C++14: init-capture
        return ++count;                       // Increment and return
        };

    // Each call increments the internal state
    std::cout << "  Call 1: " << counter() << std::endl;
    std::cout << "  Call 2: " << counter() << std::endl;
    std::cout << "  Call 3: " << counter() << std::endl;
    std::cout << "  Call 4: " << counter() << std::endl;

    // THE LAMBDA IS AN OBJECT - copying it copies its state!
    auto copy_of_counter = counter;  // Copies current state (4)
    std::cout << "  Copy call: " << copy_of_counter() << " (5 - state was copied!)" << std::endl;
    std::cout << "  Original: " << counter() << " (5 - independent now)" << std::endl;

    /*
     * IMPORTANT: Copying a mutable lambda copies its STATE.
     * The copy and the original are now independent.
     * This is standard object copy semantics.
     */
}

void UniqueIdGenerator() {
    std::cout << "\n=== Pattern: Unique ID Generator ===" << std::endl;

    // A practical use: generating sequential IDs
    // Each call gives you the next ID
    auto next_id = [id = 1000]() mutable {
        return id++;  // Post-increment: return current, then increment
        };

    std::cout << "  IDs: ";
    for(int i = 0; i < 5; ++i) {
        std::cout << next_id() << " ";
    }
    std::cout << std::endl;

    // Another example: A limited-use function
    // Only works N times, then refuses
    auto limited_call = [uses_left = 3]() mutable -> std::string {
        if(uses_left > 0) {
            --uses_left;
            return "Success! (" + std::to_string(uses_left) + " uses left)";
        }
        return "DENIED - no uses remaining";
        };

    std::cout << "  Call 1: " << limited_call() << std::endl;
    std::cout << "  Call 2: " << limited_call() << std::endl;
    std::cout << "  Call 3: " << limited_call() << std::endl;
    std::cout << "  Call 4: " << limited_call() << std::endl;
}

void CachePattern() {
    std::cout << "\n=== Pattern: Memoization/Cache ===" << std::endl;

    /*
     * MUTABLE FOR CACHING (THE "mutable" IN CONST CLASSES CONNECTION)
     * =================================================================
     * This is why 'mutable' exists in the language.
     * The concept: "logically const but physically changing"
     *
     * The lambda's BEHAVIOR (what it returns for given input)
     * doesn't change, but its internal state (cache) does.
     */

    int call_count = 0;  // Count how many times the EXPENSIVE work runs

    // A "cached" expensive function
    // First call: computes and caches. Later calls: return cache.
    auto cached_compute = [&call_count, cached = 0, is_valid = false]()
        mutable -> int {

        if(is_valid) {
            std::cout << "    [CACHE HIT - no computation needed]" << std::endl;
            return cached;
        }

        // "Expensive" computation (simulated)
        ++call_count;
        std::cout << "    [COMPUTING... call #" << call_count << "]" << std::endl;
        cached = call_count * 100;  // Fake "expensive" result
        is_valid = true;            // Mark cache as valid
        return cached;
        };

    std::cout << "  First call (computes):" << std::endl;
    std::cout << "    Result: " << cached_compute() << std::endl;

    std::cout << "  Second call (cached):" << std::endl;
    std::cout << "    Result: " << cached_compute() << std::endl;

    std::cout << "  Third call (cached):" << std::endl;
    std::cout << "    Result: " << cached_compute() << std::endl;

    std::cout << "  Total expensive computations: " << call_count
        << " (should be 1)" << std::endl;
}

void MutableGotchas() {
    std::cout << "\n=== Mutable Gotchas ===" << std::endl;

    /*
     * GOTCHA 1: mutable doesn't affect the OUTSIDE variable
     * ================================================================
     */
    int outside = 5;
    auto lambda = [outside]() mutable {
        outside = 100;  // Modifies lambda's COPY
        return outside;
        };
    lambda();
    std::cout << "  Outside after mutable modification: " << outside
        << " (still 5 - only the copy changed)" << std::endl;

    /*
     * GOTCHA 2: Copies of mutable lambdas are independent
     * ================================================================
     * Once you copy a mutable lambda, the copy's state diverges.
     */
    auto stateful = [n = 0]() mutable { return ++n; };
    stateful();
    stateful();        // Original is at 2

    auto stateful_copy = stateful;  // Copy captures state at 2
    std::cout << "  Copy: " << stateful_copy() << " (3 - continued from 2)" << std::endl;
    std::cout << "  Original: " << stateful() << " (3 - independent path)" << std::endl;

    /*
     * GOTCHA 3: Mutable in multi-threaded code needs care
     * ================================================================
     * If multiple threads call the same mutable lambda, they're
     * sharing and racing on its internal state. Use synchronization.
     */
}