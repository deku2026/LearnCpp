// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : unit_versus_integration
// Topic id : part4/section04/unit_versus_integration
//
// Covers: unit vs integration test boundaries

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Unit: pure function, no I/O
    auto twice = [](int x) { return x * 2; };
    LEARN_CHECK(twice(3) == 6);
}

void demo_intermediate() {
    // Integration: multiple components + real filesystem/network carefully
    LEARN_CHECK(true);
}

void demo_expert() {
    // Pyramid: many unit, fewer integration, few e2e
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/unit_versus_integration", run>;

}  // namespace
