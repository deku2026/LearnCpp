// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : coverage_overview
// Topic id : part4/section05/coverage_overview
//
// Covers: code coverage llvm-cov/gcov

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -fprofile-instr-generate -fcoverage-mapping
    std::string tool = "llvm-cov";
    assert(!tool.empty());
}

void demo_intermediate() {
    // Enforce coverage thresholds carefully (quality not vanity)
    assert(true);
}

void demo_expert() {
    // Exclude generated code
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/coverage_overview", run>;

}  // namespace
