// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : warning_treatment_and_werror
// Topic id : part4/section03/warning_treatment_and_werror
//
// Covers: warning levels and -Werror /treats warnings as errors

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -Wall -Wextra -Wpedantic /W4
    const int warning_level = 4;
    assert(warning_level >= 3);
}

void demo_intermediate() {
    // Enable -Werror in CI after codebase is clean
    const bool werror_in_ci = true;
    assert(werror_in_ci);
}

void demo_expert() {
    // Do not blanket-disable; fix or narrow pragma
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/warning_treatment_and_werror", run>;

}  // namespace
