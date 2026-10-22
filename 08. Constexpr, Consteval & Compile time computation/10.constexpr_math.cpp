/*
 * ============================================================================
 * CONSTEXPR MATH - MATHEMATICS AT COMPILE TIME
 * ============================================================================
 *
 * WHAT CAN WE COMPUTE?
 * ====================
 * Anything that doesn't need runtime input:
 * - Trigonometric tables
 * - Statistical computations
 * - Numerical methods
 * - Mathematical constants
 *
 * THE ADVANTAGE:
 * ==============
 * Expensive mathematical computations happen ONCE during compilation,
 * not every time your program runs.
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include "constexpr_header.h"

void CompileTimeTrig() {
    std::cout << "=== Compile-Time Trigonometry ===" << std::endl;

    /*
     * BUILDING A SINE TABLE AT COMPILE TIME:
     * ================================================================
     * Games and signal processing often need lookup tables.
     * Computing them at compile time = free at runtime!
     */

     // Taylor series approximation of sin(x)
    constexpr double sin_approx(double x) {
        // sin(x) ≈ x - x³/3! + x⁵/5! - x⁷/7! + ...
        double result = x;
        double term = x;
        double x_sq = x * x;

        for(int n = 1; n <= 10; ++n) {
            term *= -x_sq / ((2 * n) * (2 * n + 1));
            result += term;
        }

        return result;
    }

    // Build a lookup table:
    constexpr size_t TABLE_SIZE = 16;
    constexpr auto make_sin_table = []() {
        std::array<double, TABLE_SIZE> table{};
        for(size_t i = 0; i < TABLE_SIZE; ++i) {
            double angle = (2.0 * 3.14159265358979 * i) / TABLE_SIZE;
            table[i] = sin_approx(angle);
        }
        return table;
        };

    constexpr auto sin_table = make_sin_table();

    // Verify:
    static_assert(sin_table[0] < 0.001);        // sin(0°) ≈ 0
    static_assert(sin_table[4] > 0.999);        // sin(90°) ≈ 1
    static_assert(sin_table[8] > -0.001);       // sin(180°) ≈ 0

    std::cout << "  Sine lookup table (16 entries):" << std::endl;
    for(size_t i = 0; i < TABLE_SIZE; i += 4) {
        double angle_deg = (360.0 * i) / TABLE_SIZE;
        std::cout << "    sin(" << angle_deg << "°) = " << sin_table[i] << std::endl;
    }
    std::cout << "  (Table computed entirely at compile time!)" << std::endl;
}

void CompileTimeStatistics() {
    std::cout << "\n=== Compile-Time Statistics ===" << std::endl;

    /*
     * STATISTICAL COMPUTATIONS AT COMPILE TIME:
     * ================================================================
     */

    constexpr auto compute_stats = []() {
        std::array<double, 10> data = {
            85.5, 92.3, 78.9, 95.1, 88.7,
            91.2, 84.6, 89.8, 93.4, 87.5
        };

        // Mean:
        double sum = 0;
        for(double x : data) sum += x;
        double mean = sum / data.size();

        // Variance:
        double sq_diff_sum = 0;
        for(double x : data) {
            double diff = x - mean;
            sq_diff_sum += diff * diff;
        }
        double variance = sq_diff_sum / data.size();

        // Standard deviation:
        // (manual sqrt since std::sqrt might not be constexpr in all compilers)
        double std_dev = 0;
        if(variance > 0) {
            double guess = variance;
            for(int i = 0; i < 20; ++i) {  // Newton's method
                guess = (guess + variance / guess) / 2;
            }
            std_dev = guess;
        }

        return std::make_tuple(mean, variance, std_dev);
        };

    constexpr auto [mean, variance, std_dev] = compute_stats();

    std::cout << "  Data: exam scores" << std::endl;
    std::cout << "  Mean: " << mean << std::endl;
    std::cout << "  Variance: " << variance << std::endl;
    std::cout << "  Std Dev: " << std_dev << std::endl;
    std::cout << "  (All computed at compile time!)" << std::endl;
}

void MathematicalConstants() {
    std::cout << "\n=== Computing Mathematical Constants ===" << std::endl;

    /*
     * COMPUTING π AT COMPILE TIME:
     * ================================================================
     */

     // Leibniz formula: π/4 = 1 - 1/3 + 1/5 - 1/7 + ...
    constexpr double compute_pi(int terms) {
        double result = 0;
        for(int i = 0; i < terms; ++i) {
            if(i % 2 == 0) {
                result += 1.0 / (2 * i + 1);
            }
            else {
                result -= 1.0 / (2 * i + 1);
            }
        }
        return result * 4;
    }

    constexpr double pi_10000 = compute_pi(10000);

    std::cout << "  π (10,000 terms): " << pi_10000 << std::endl;
    std::cout << "  Reference:        3.14159265358979..." << std::endl;

    // Euler's number e = sum(1/n!)
    constexpr double compute_e(int terms) {
        double result = 1.0;
        double factorial = 1.0;
        for(int i = 1; i <= terms; ++i) {
            factorial *= i;
            result += 1.0 / factorial;
        }
        return result;
    }

    constexpr double e_20 = compute_e(20);
    std::cout << "\n  e (20 terms): " << e_20 << std::endl;
    std::cout << "  Reference:    2.71828182845905..." << std::endl;

    std::cout << R"(
    APPLICATIONS:
    ┌─────────────────────────────────────────────────────┐
    │ • Game development: physics constants, AI weights   │
    │ • Signal processing: filter coefficients           │
    │ • Graphics: precomputed lighting tables            │
    │ • Scientific computing: precomputed tables         │
    │ • Finance: precomputed interest tables             │
    │                                                     │
    │ ANY expensive computation that produces             │
    │ a CONSTANT result is a candidate!                  │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}