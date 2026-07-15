// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : link_time_optimization
// Topic id : part6/g/section02/link_time_optimization
//
// Covers: LTO conceptual benefits; correctness still required

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

static int mul(int a, int b) {
    return a * b;
}

int api_square(int x) {
    return mul(x, x);
}

void demo_basics() {
    assert(api_square(4) == 16);
}

void demo_intermediate() {
    assert(api_square(0) == 0);
}

void demo_expert() {
    assert(api_square(3) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/link_time_optimization", run>;

}  // namespace
