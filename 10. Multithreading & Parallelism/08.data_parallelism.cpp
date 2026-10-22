/*
 * ============================================================================
 * DATA PARALLELISM - SAME OPERATION, DIFFERENT DATA
 * ============================================================================
 *
 * WHAT IS DATA PARALLELISM?
 * =========================
 * Applying the SAME operation to MANY pieces of data simultaneously.
 * The parallelism comes from the DATA, not the operations.
 *
 * EXAMPLE:
 * =========
 *   [Image Processing] Apply "blur" to 1M pixels:
 *     - Thread 1: pixels 0-250K
 *     - Thread 2: pixels 250K-500K
 *     - Thread 3: pixels 500K-750K
 *     - Thread 4: pixels 750K-1M
 *
 * CHARACTERISTICS:
 * ================
 * - Embarrassingly parallel (minimal coordination)
 * - Scales linearly with more cores
 * - Perfect for SIMD/vectorization
 *
 * ============================================================================
 */

import std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <chrono>
#include <cmath>
#include "parallelism_header.h"

void DataParallelExamples() {
    std::cout << "=== Data Parallelism ===" << std::endl;

    /*
     * EXAMPLE 1: Image processing (brightness adjustment)
     * ================================================================
     */

    std::vector<uint8_t> image(10'000'000);  // 10M pixels
    std::generate(image.begin(), image.end(), []() {
        return static_cast<uint8_t>(std::rand() % 256);
                  });

    // Adjust brightness (+50) for all pixels:
    std::for_each(std::execution::par, image.begin(), image.end(),
                  [](uint8_t& pixel) {
                      pixel = static_cast<uint8_t>(
                          std::min(255, static_cast<int>(pixel) + 50)
                          );
                  });

    std::cout << "  Adjusted brightness for 10M pixels" << std::endl;
    std::cout << "  First pixel: " << static_cast<int>(image[0]) << std::endl;

    /*
     * EXAMPLE 2: Physics simulation (update all particles)
     * ================================================================
     */

    struct Particle {
        float x, y, z;
        float vx, vy, vz;
    };

    std::vector<Particle> particles(1'000'000);

    float dt = 0.016f;  // 60fps timestep
    float gravity = -9.81f;

    // Update all particles in parallel:
    std::for_each(std::execution::par, particles.begin(), particles.end(),
                  [dt, gravity](Particle& p) {
                      // Apply gravity
                      p.vy += gravity * dt;

                      // Update positions
                      p.x += p.vx * dt;
                      p.y += p.vy * dt;
                      p.z += p.vz * dt;
                  });

    std::cout << "  Updated 1M particles (physics step)" << std::endl;

    std::cout << R"(
    DATA PARALLELISM PATTERNS:
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │  for_each(par, container, op)     → In-place modify │
    │  transform(par, in, out, op)     → New output      │
    │  transform_reduce(par, in, op)   → Compute+combine │
    │                                                     │
    │  REQUIREMENT: Each element independent!             │
    │  No element reads/writes another element            │
    └─────────────────────────────────────────────────────┘
)" << std::endl;
}