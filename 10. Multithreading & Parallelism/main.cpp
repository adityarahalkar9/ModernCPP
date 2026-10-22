import std;
#include "parallelism_header.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " ModernCPP - 10. Parallelism Complete" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // --- Parallel Algorithms (01) ---
    // ParallelSortDemo();
    // ParallelTransformDemo();
    // ParallelReduceDemo();

    // --- Execution Policies (02) ---
    // AllPoliciesDemo();
    // PolicySafetyRules();

    // --- Thread Pool (03) ---
    // ThreadPoolDemo();

    // --- Fork-Join (04) ---
    // ForkJoinBasics();
    // ParallelMergeSort();

    // --- Parallel Map (05) ---
    // ParallelMapPatterns();

    // --- Parallel Reduce (06) ---
    // ParallelReduceDemo();
    // TreeReduction();

    // --- Parallel Sort (07) ---
    // SortingComparison();

    // --- Data Parallelism (08) ---
    // DataParallelExamples();

    // --- Task Parallelism (09) ---
    // TaskParallelismDemo();
    // PipelineOfTasks();

    // --- Load Balancing (10) ---
    // StaticVsDynamic();

    // --- False Sharing (11) ---
    // FalseSharingDemonstration();  // Run this to see the impact!

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "  End of Parallelism Examples" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}