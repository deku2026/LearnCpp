// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : calling_conventions
// Topic id : part6/g/section01/calling_conventions
//
// Covers: calling conventions affect how args/returns are passed

#include "learn/topic_registry.hpp"

namespace {

int sum4(int a, int b, int c, int d) {
    return a + b + c + d;
}

struct Small {
    int x;
    int y;
};

Small make_small(int x, int y) {
    return Small{x, y};
}

void demo_basics() {
    LEARN_CHECK(sum4(1, 2, 3, 4) == 10);
}

void demo_intermediate() {
    Small s = make_small(2, 5);
    LEARN_CHECK(s.x + s.y == 7);
}

void demo_expert() {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/calling_conventions", run>;

}  // namespace
