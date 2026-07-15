// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : sanitizer_compatibility_matrix
// Topic id : part4/section05/sanitizer_compatibility_matrix
//
// Covers: which sanitizers can combine

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Common: ASan+UBSan; TSan alone; MSan alone
    std::string lane_a = "asan+ubsan";
    std::string lane_t = "tsan";
    LEARN_CHECK(lane_a != lane_t);
}

void demo_intermediate() {
    // Do not mix ASan with TSan/MSan
    LEARN_CHECK(true);
}

void demo_expert() {
    // Document matrix in CI README
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/sanitizer_compatibility_matrix", run>;

}  // namespace
