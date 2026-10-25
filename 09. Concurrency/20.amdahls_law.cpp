/*
 * ============================================================================
 * AMDAHL'S LAW - THE LIMITS OF PARALLELIZATION
 * ============================================================================
 *
 * WHAT IS AMDAHL'S LAW?
 * =====================
 * A formula that predicts the maximum speedup from parallelization:
 *
 *   Speedup = 1 / ((1 - P) + (P / N))
 *
 * Where:
 *   P = fraction of code that can be parallelized (0 to 1)
 *   N = number of processors
 *
 * THE INSIGHT:
 * ============
 * The SEQUENTIAL part of your code limits maximum speedup,
 * no matter how many processors you have!
 *
 * EXAMPLE:
 * ========
 * If 90% of your code is parallelizable (P = 0.9):
 *   With 1 processor:  Speedup = 1
 *   With 10 processors: Speedup = 1 / (0.1 + 0.09) = 5.26x
 *   With 100 processors: Speedup = 1 / (0.1 + 0.009) = 9.17x
 *   With ∞ processors: Speedup = 1 / 0.1 = 10x (MAXIMUM!)
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <iomanip>
#include "concurrency_header.h"

void AmdahlsLawExplained() {
    std::cout << "=== Amdahl's Law: The Limits of Speedup ===" << std::endl;

    /*
     * THE FORMULA:
     * ================================================================
     */
    auto calculate_speedup = [](double parallel_fraction, int num_processors) {
        // Amdahl's Law:
        // Speedup = 1 / ((1 - P) + (P / N))
        double sequential_fraction = 1.0 - parallel_fraction;
        double denominator = sequential_fraction + (parallel_fraction / num_processors);
        return 1.0 / denominator;
        };

    /*
     * TABLE: Maximum speedup for different parallel fractions
     * ================================================================
     */
    std::cout << "\n  Maximum speedup by parallelizable fraction:" << std::endl;
    std::cout << "  " << std::setw(10) << "P (parallel)"
        << " | " << std::setw(8) << "4 cores"
        << " | " << std::setw(8) << "16 cores"
        << " | " << std::setw(8) << "∞ cores" << std::endl;
    std::cout << "  " << std::string(50, '-') << std::endl;

    for(double p : {0.5, 0.7, 0.9, 0.95, 0.99, 1.0}) {
        double sp_4 = calculate_speedup(p, 4);
        double sp_16 = calculate_speedup(p, 16);
        double sp_inf = 1.0 / (1.0 - p);  // Limit as N→∞

        std::cout << "  " << std::setw(10) << p
            << " | " << std::setw(8) << std::fixed << std::setprecision(2) << sp_4 << "x"
            << " | " << std::setw(8) << sp_16 << "x"
            << " | " << std::setw(8) << sp_inf << "x" << std::endl;
    }

    std::cout << R"(
    THE DEVASTATING IMPLICATION:
    ┌─────────────────────────────────────────────────────┐
    │ If 95% of your code is parallelizable:               │
    │   Maximum speedup (even with ∞ cores) = 20x        │
    │                                                     │
    │ If 90% is parallelizable:                           │
    │   Maximum speedup = 10x                            │
    │                                                     │
    │ If 50% is parallelizable:                           │
    │   Maximum speedup = 2x                             │
    │                                                     │
    │ THE SEQUENTIAL 5% DOMINATES!                       │
    │                                                     │
    │ This is why:                                        │
    │ • Serial parts of algorithms matter enormously     │
    │ • I/O, synchronization, setup time add up          │
    │ • Sometimes buying more cores barely helps         │
    └─────────────────────────────────────────────────────┘

    PRACTICAL IMPLICATIONS:
    ┌─────────────────────────────────────────────────────┐
    │ 1. PROFILE FIRST: Find the sequential bottleneck    │
    │ 2. REDUCE SEQUENTIAL PART: That gives best ROI     │
    │ 3. REALISTIC EXPECTATIONS: 4 cores ≈ 3-3.5x       │
    │ 4. MORE CORES ≠ LINEAR SPEEDUP                    │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}

void GustafsonsLaw() {
    std::cout << "\n=== Gustafson's Law (The Optimist's View) ===" << std::endl;

    /*
     * AMDAHL vs GUSTAFSON:
     * ================================================================
     * Amdahl: Fixed problem size, more processors → limited speedup
     * Gustafson: Scale the problem with processors → better outlook
     */

    std::cout << R"(
    TWO PERSPECTIVES:
    ┌─────────────────────────────────────────────────────┐
    │ AMDAHL'S LAW (Pessimistic):                        │
    │   "Fixed work, more processors"                     │
    │   Speedup limited by sequential fraction            │
    │   Good for: Analyzing existing programs            │
    │                                                     │
    │ GUSTAFSON'S LAW (Optimistic):                      │
    │   "Scale problem size with processors"              │
    │   If you have 16x compute, solve 16x bigger problems│
    │   Good for: Scientific computing, simulations      │
    │                                                     │
    │ EXAMPLE:                                            │
    │   Weather simulation:                               │
    │   • Amdahl: "Same forecast, 10x faster" (limited)   │
    │   • Gustafson: "Same time, 10x more detailed model" │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}