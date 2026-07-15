// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : catch2_intro
// Topic id : part4/section04/catch2_intro
//
// Covers: Catch2 TEST_CASE/SECTION conceptual

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // TEST_CASE("name") { REQUIRE(1+1 == 2); }
    assert(1 + 1 == 2);
}

void demo_intermediate() {
    // SECTION for BDD-like nested scenarios
    assert(true);
}

void demo_expert() {
    // Header-only option simplifies small projects
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/catch2_intro", run>;

}  // namespace
