// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : asan_ubsan_lane
// Topic id : part4/section06/asan_ubsan_lane
//
// Covers: dedicated ASan+UBSan CI lane

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Separate job with sanitizer flags and debug info
    std::string lane = "asan-ubsan";
    assert(lane.find("asan") != std::string::npos);
}

void demo_intermediate() {
    // Longer timeouts; more RAM
    assert(true);
}

void demo_expert() {
    // Keep main fast lane without sanitizers if needed
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/asan_ubsan_lane", run>;

}  // namespace
