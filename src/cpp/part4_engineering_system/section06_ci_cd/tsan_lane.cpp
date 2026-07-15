// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : tsan_lane
// Topic id : part4/section06/tsan_lane
//
// Covers: dedicated TSan CI lane

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Run concurrency tests under TSan only
    std::string lane = "tsan";
    assert(lane == "tsan");
}

void demo_intermediate() {
    // Exclude conflicting sanitizer flags
    assert(true);
}

void demo_expert() {
    // Flaky races still need deterministic repros
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/tsan_lane", run>;

}  // namespace
