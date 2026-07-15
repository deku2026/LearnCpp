// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : ubsan_overview
// Topic id : part4/section05/ubsan_overview
//
// Covers: UndefinedBehaviorSanitizer

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -fsanitize=undefined
    int x = 100;
    int y = x * 2;  // safe
    LEARN_CHECK(y == 200);
}

void demo_intermediate() {
    // Catches signed overflow, bad shifts, misaligned (optional)
    LEARN_CHECK(true);
}

void demo_expert() {
    // Pair with ASan in CI lane
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/ubsan_overview", run>;

}  // namespace
