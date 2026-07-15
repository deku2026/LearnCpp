// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : googletest_basics
// Topic id : part4/section04/googletest_basics
//
// Covers: GoogleTest TEST/ASSERT/EXPECT conceptual

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // TEST(Suite, Name) { EXPECT_EQ(1+1, 2); }
    auto add = [](int a, int b) { return a + b; };
    assert(add(1, 1) == 2);
}

void demo_intermediate() {
    // ASSERT_* aborts test; EXPECT_* continues
    assert(true);
}

void demo_expert() {
    // gtest_discover_tests for CTest integration
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/googletest_basics", run>;

}  // namespace
