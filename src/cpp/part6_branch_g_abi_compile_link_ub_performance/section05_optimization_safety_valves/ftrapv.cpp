// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : ftrapv
// Topic id : part6/g/section05/ftrapv
//
// Covers: ftrapv traps signed overflow at runtime (debug dialect)

#include "learn/topic_registry.hpp"

#include <climits>

namespace {

bool will_overflow_add(int a, int b) {
    if (b > 0) return a > INT_MAX - b;
    if (b < 0) return a < INT_MIN - b;
    return false;
}

void demo_basics() {
    LEARN_CHECK(!will_overflow_add(10, 20));
    LEARN_CHECK(10 + 20 == 30);
}

void demo_intermediate() {
    LEARN_CHECK(will_overflow_add(INT_MAX, 1));
}

void demo_expert() {
    // ftrapv is a debugging aid; production still needs defined algorithms.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/ftrapv", run>;

}  // namespace
