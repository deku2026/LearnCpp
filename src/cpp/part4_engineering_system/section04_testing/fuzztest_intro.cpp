// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : fuzztest_intro
// Topic id : part4/section04/fuzztest_intro
//
// Covers: Google FuzzTest property-based fuzzing conceptual

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Property: for all inputs, invariant holds
    auto inv = [](int x) { return x + 0 == x; };
    LEARN_CHECK(inv(42));
}

void demo_intermediate() {
    // Domain combinators generate structured inputs
    LEARN_CHECK(true);
}

void demo_expert() {
    // Integrates with GoogleTest
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/fuzztest_intro", run>;

}  // namespace
