// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : unit_versus_integration
// Topic id : part4/section04/unit_versus_integration
//
// Covers: unit vs integration test boundaries

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Unit: pure function, no I/O
    auto twice = [](int x) { return x * 2; };
    assert(twice(3) == 6);
}

void demo_intermediate() {
    // Integration: multiple components + real filesystem/network carefully
    assert(true);
}

void demo_expert() {
    // Pyramid: many unit, fewer integration, few e2e
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/unit_versus_integration", run>;

}  // namespace
