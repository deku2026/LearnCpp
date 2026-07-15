// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : out_of_bounds_deep
// Topic id : part6/g/section03/out_of_bounds_deep
//
// Covers: out-of-bounds access is UB; use size checks / at()

#include "learn/topic_registry.hpp"

#include <array>
#include <vector>

namespace {

void demo_basics() {
    std::array<int, 3> a{1, 2, 3};
    LEARN_CHECK(a[0] == 1);
    LEARN_CHECK(a.at(2) == 3);
}

void demo_intermediate() {
    std::vector<int> v{1, 2};
    LEARN_CHECK(v.size() == 2);
    LEARN_CHECK(v.at(1) == 2);
}

void demo_expert() {
    // Not executed: v[v.size()] or pointer past end dereference.
    int a[2] = {1, 2};
    int* end = a + 2;  // past-the-end pointer OK
    LEARN_CHECK(end - a == 2);
    LEARN_CHECK(a[1] == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/out_of_bounds_deep", run>;

}  // namespace
