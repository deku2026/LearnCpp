// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : uninitialized_read_deep
// Topic id : part6/g/section03/uninitialized_read_deep
//
// Covers: reading indeterminate values is UB for many types

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>

namespace {

void demo_basics() {
    int x = 0;  // initialize
    assert(x == 0);
}

void demo_intermediate() {
    std::optional<int> o;
    assert(!o.has_value());
    o = 5;
    assert(*o == 5);
}

void demo_expert() {
    // Not executed: int x; int y = x;
    int x{};
    assert(x == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/uninitialized_read_deep", run>;

}  // namespace
