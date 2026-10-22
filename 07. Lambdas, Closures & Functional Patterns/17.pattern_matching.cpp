import std;
/*
 * ============================================================================
 * PATTERN MATCHING WITH std::variant
 * ============================================================================
 *
 * WHAT IS PATTERN MATCHING?
 * =========================
 * A way to check a value against multiple patterns and execute
 * different code based on which pattern matches.
 *
 * THINK OF IT AS:
 * ===============
 * A super-charged switch statement that can:
 * - Match on TYPE (not just values)
 * - Extract data from the matched structure
 * - Ensure you handle ALL possible cases (compiler-enforced!)
 *
 * THE C++ TOOL: std::variant + std::visit
 * =======================================
 * std::variant is a type-safe union - it holds ONE value
 * that can be of several specified types.
 *
 * std::visit applies the right code based on which type is stored.
 *
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include "functional_header.h"

 // A variant that can hold different types of "values"
using Value = std::variant<int, double, std::string, std::vector<int>>;

void VariantBasics() {
    std::cout << "=== std::variant Basics ===" << std::endl;

    /*
     * WHAT IS A VARIANT?
     * ==================
     * A type-safe container that holds ONE value,
     * where that value can be one of several types.
     *
     * It's like a union, but:
     * - It knows which type it currently holds
     * - It calls proper constructors/destructors
     * - It's completely type-safe
     */

    Value v;

    // Store an int:
    v = 42;
    std::cout << "  Holds int: " << std::get<int>(v) << std::endl;

    // Store a string (previous int is properly destroyed):
    v = std::string("hello");
    std::cout << "  Now holds string: " << std::get<std::string>(v) << std::endl;

    // Store a vector:
    v = std::vector<int>{1, 2, 3};
    std::cout << "  Now holds vector (size: " << std::get<std::vector<int>>(v).size() << ")" << std::endl;

    // CHECKING WHAT'S INSIDE:
    v = 3.14;
    std::cout << "  Currently double: " << std::holds_alternative<double>(v) << std::endl;
    std::cout << "  Is int? " << std::holds_alternative<int>(v) << std::endl;
}

void PatternMatching() {
    std::cout << "\n=== Pattern Matching with std::visit ===" << std::endl;

    /*
     * THE PATTERN MATCHING TOOL: std::visit
     * =====================================
     * std::visit takes a "visitor" and a variant.
     * It calls the visitor with the CURRENTLY HELD type.
     *
     * If you use a generic lambda (auto), it works for all types.
     * If you provide overloads, the right one is selected.
     */

    std::vector<Value> values = {
        42,                    // int
        3.14,                  // double
        std::string("hello"),  // string
        std::vector<int>{1,2,3} // vector
    };

    // PATTERN MATCHING: Handle each type differently
    for(const auto& v : values) {
        std::visit([](const auto& value) {
            // 'value' has the ACTUAL type stored in the variant
            using T = std::decay_t<decltype(value)>;

            if constexpr(std::is_same_v<T, int>) {
                std::cout << "  int: " << value << " (half is " << value / 2 << ")" << std::endl;
            }
            else if constexpr(std::is_same_v<T, double>) {
                std::cout << "  double: " << value << " (rounded: " << std::round(value) << ")" << std::endl;
            }
            else if constexpr(std::is_same_v<T, std::string>) {
                std::cout << "  string: \"" << value << "\" (length: " << value.size() << ")" << std::endl;
            }
            else if constexpr(std::is_same_v<T, std::vector<int>>) {
                int sum = 0;
                for(int x : value) sum += x;
                std::cout << "  vector (sum: " << sum << ")" << std::endl;
            }
                   }, v);
    }

    /*
     * THE POWER:
     * ==========
     * - if constexpr chooses the RIGHT code at COMPILE time
     * - No runtime type checks
     * - Type-safe: you can't accidentally use the wrong type
     */
}

void overloadedPattern() {
    std::cout << "\n=== The Overloaded Pattern (Classic) ===" << std::endl;

    /*
     * THE FAMOUS "OVERLOADED" TRICK:
     * ==============================
     * Before if constexpr was common, this pattern was popular.
     * It creates a set of overloads that std::visit can dispatch to.
     */

     // The overloaded helper (typically defined once in a utility header)
    template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    // Create a "visitor" with specific handlers for each type:
    auto handler = overloaded{
        [](int i) { std::cout << "  int: " << i << std::endl; },
        [](double d) { std::cout << "  double: " << d << std::endl; },
        [](const std::string& s) { std::cout << "  string: \"" << s << "\"" << std::endl; },
        [](const std::vector<int>& v) {
            std::cout << "  vector with " << v.size() << " elements" << std::endl;
        }
    };

    // Apply to various values:
    std::visit(handler, Value{42});
    std::visit(handler, Value{3.14});
    std::visit(handler, Value{std::string("test")});
    std::visit(handler, Value{std::vector<int>{1,2,3,4}});
}

void StateMachinePattern() {
    std::cout << "\n=== Real Use Case: State Machine ===" << std::endl;

    /*
     * PRACTICAL EXAMPLE: Game character states
     * ========================================
     * Pattern matching is perfect for state machines where
     * different states need different data.
     */

    struct Idle { std::string animation; };
    struct Walking { double speed; std::string direction; };
    struct Attacking { int damage; int range; };
    struct Dead { int respawn_time; };

    using GameState = std::variant<Idle, Walking, Attacking, Dead>;

    // Different states with different data:
    std::vector<GameState> states = {
        Idle{"idle_anim"},
        Walking{5.5, "north"},
        Attacking{25, 100},
        Dead{30}
    };

    // Pattern match to handle each state:
    for(const auto& state : states) {
        std::visit([](const auto& s) {
            using T = std::decay_t<decltype(s)>;

            if constexpr(std::is_same_v<T, Idle>) {
                std::cout << "  [IDLE] playing: " << s.animation << std::endl;
            }
            else if constexpr(std::is_same_v<T, Walking>) {
                std::cout << "  [WALKING] " << s.speed << " m/s going " << s.direction << std::endl;
            }
            else if constexpr(std::is_same_v<T, Attacking>) {
                std::cout << "  [ATTACK] " << s.damage << " dmg at " << s.range << "m" << std::endl;
            }
            else if constexpr(std::is_same_v<T, Dead>) {
                std::cout << "  [DEAD] respawn in " << s.respawn_time << "s" << std::endl;
            }
                   }, state);
    }

    /*
     * WHY THIS IS BETTER THAN INHERITANCE:
     * ====================================
     * 1. Value semantics (can copy, compare)
     * 2. No heap allocation needed
     * 3. Exhaustive checking (compiler verifies you handle all cases)
     * 4. All states visible in one place
     */
}