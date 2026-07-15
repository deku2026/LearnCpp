// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : parameterized_and_fixture
// Topic id : part4/section04/parameterized_and_fixture
//
// Covers: parameterized tests and fixtures pattern

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Fixture: shared SetUp/TearDown for suite
    struct Fixture {
        int value = 0;
        void SetUp() { value = 1; }
    };
    Fixture f;
    f.SetUp();
    assert(f.value == 1);
}

void demo_intermediate() {
    // Parameterized: same test over value table
    int cases[] = {1, 2, 3};
    for (int c : cases) {
        assert(c > 0);
    }
}

void demo_expert() {
    // Keep fixtures lean; avoid hidden global state
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/parameterized_and_fixture", run>;

}  // namespace
