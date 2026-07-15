// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : doctest_intro
// Topic id : part4/section04/doctest_intro
//
// Covers: doctest lightweight framework

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // CHECK(1+1 == 2); fast compile times
    assert(1 + 1 == 2);
}

void demo_intermediate() {
    // Similar to Catch2, smaller
    assert(true);
}

void demo_expert() {
    // Good for embedded/header-heavy codebases
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/doctest_intro", run>;

}  // namespace
