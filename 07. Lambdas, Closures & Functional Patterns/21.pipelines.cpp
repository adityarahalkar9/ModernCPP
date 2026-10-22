import std;
/*
   PIPELINES - CHAINING TRANSFORMATIONS
 
   WHAT IS A PIPELINE?
   A series of processing steps where the output of each step
   becomes the input to the next.
   data → step1 → step2 → step3 → result
  
   REAL-WORLD ANALOGIES:
   - Unix pipes:  cat file | grep pattern | sort | uniq
   - Assembly lines in factories
   - Water treatment: raw water → filter → purify → bottle
  
   BENEFITS:
   1. Each step does ONE thing well
   2. Steps are independently testable
   3. Easy to add/remove/reorder steps
   4. Reads like a description of the process
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "functional_header.h"

void BasicPipeline() {
    std::cout << "=== Basic Data Pipeline ===" << std::endl;
    // A MANUAL PIPELINE: Step by step
    // Raw input data
    std::vector<std::string> raw_data = {
        "  Apple  ", " banana ", "  CHERRY", "date  ", "  ELDERBERRY  "
    };

    // STEP 1: Clean (trim whitespace)
    std::vector<std::string> cleaned;
    std::transform(raw_data.begin(), raw_data.end(),
                   std::back_inserter(cleaned),
                   [](const std::string& s) {
                       size_t start = s.find_first_not_of(" \t");
                       size_t end = s.find_last_not_of(" \t");
                       if(start == std::string::npos) return std::string{};
                       return s.substr(start, end - start + 1);
                   });

    // STEP 2: Normalize (lowercase)
    std::vector<std::string> normalized;
    std::transform(cleaned.begin(), cleaned.end(),
                   std::back_inserter(normalized),
                   [](std::string s) {
                       std::transform(s.begin(), s.end(), s.begin(),
                                      [](unsigned char c) { return std::tolower(c); });
                       return s;
                   });

    // STEP 3: Filter (only certain lengths)
    std::vector<std::string> filtered;
    std::copy_if(normalized.begin(), normalized.end(),
                 std::back_inserter(filtered),
                 [](const std::string& s) { return s.length() <= 6; });

    // STEP 4: Transform (add numbering)
    std::vector<std::string> numbered;
    std::transform(filtered.begin(), filtered.end(),
                   std::back_inserter(numbered),
                   [i = 1](const std::string& s) mutable {
                       return std::to_string(i++) + ". " + s;
                   });

    // FINAL RESULT:
    std::cout << "  Pipeline output:" << std::endl;
    for(const auto& item : numbered) {
        std::cout << "    " << item << std::endl;
    }
}

void PipelineBuilder() {
    std::cout << "\n=== Building a Pipeline Framework ===" << std::endl;

    /*
       A REUSABLE PIPELINE SYSTEM:
       Instead of writing each step manually, create a framework
       that composes steps.
    */
    // A simple pipeline class that holds and applies transformations
    template <typename T>
    class Pipeline {
        std::vector<std::function<T(T)>> steps_;

    public:
        Pipeline& then(std::function<T(T)> step) {
            steps_.push_back(std::move(step));
            return *this;  // Enable chaining
        }

        T run(T input) {
            for(auto& step : steps_) {
                input = step(input);  // Feed output to next step
            }
            return input;
        }
    };

    // Build a text-processing pipeline:
    Pipeline<std::string> text_pipeline;

    text_pipeline
        .then([](std::string s) {  // Step 1: trim
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
              })
        .then([](std::string s) {  // Step 2: lowercase
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return s;
              })
        .then([](std::string s) {  // Step 3: replace spaces with underscores
        std::replace(s.begin(), s.end(), ' ', '_');
        return s;
              })
        .then([](std::string s) {  // Step 4: add prefix
        return "processed: " + s;
              });

    // Use the pipeline:
    std::string input = "  Hello World Example  ";
    std::string result = text_pipeline.run(input);

    std::cout << "  Input:  \"" << input << "\"" << std::endl;
    std::cout << "  Output: \"" << result << "\"" << std::endl;

    // Run more data through the same pipeline:
    std::string more_data = "  ANOTHER Test  ";
    std::cout << "  More data: \"" << text_pipeline.run(more_data) << "\"" << std::endl;
}

void ErrorHandlingPipeline() {
    std::cout << "\n=== Pipeline with Error Handling ===" << std::endl;

    /*
     * PIPELINES THAT CAN FAIL:
     * ========================
     * Each step can succeed or fail.
     * Failure stops the pipeline.
     */

     // A step that can fail
    template <typename In, typename Out>
    class MaybeStep {
        std::function<std::optional<Out>(In)> func_;

    public:
        explicit MaybeStep(std::function<std::optional<Out>(In)> f)
            : func_(std::move(f)) {
        }

        std::optional<Out> operator()(In input) const {
            return func_(input);
        }
    };

    // Steps for processing a number:
    auto parse = MaybeStep<std::string, int>(
        [](const std::string& s) -> std::optional<int> {
            try { return std::stoi(s); }
            catch(...) { return std::nullopt; }
        });

    auto validate = MaybeStep<int, int>(
        [](int x) -> std::optional<int> {
            if(x < 0) return std::nullopt;  // Negative not allowed
            return x;
        });

    auto transform = MaybeStep<int, std::string>(
        [](int x) -> std::optional<std::string> {
            if(x > 1000) return std::nullopt;  // Too big
            return "Result: " + std::to_string(x * 2);
        });

    // Chain them with short-circuit on failure:
    auto process = [&parse, &validate, &transform](const std::string& input)
        -> std::optional<std::string> {

        auto parsed = parse(input);
        if(!parsed) return std::nullopt;  // Parse failed

        auto valid = validate(*parsed);
        if(!valid) return std::nullopt;  // Validation failed

        return transform(*valid);
        };

    // Test with various inputs:
    std::vector<std::string> inputs = {"42", "-5", "not_a_number", "500"};

    for(const auto& input : inputs) {
        auto result = process(input);
        if(result) {
            std::cout << "  \"" << input << "\" -> " << *result << std::endl;
        }
        else {
            std::cout << "  \"" << input << "\" -> FAILED" << std::endl;
        }
    }
}