// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : ub_overview_and_optimizer_implications
// Topic id : part6/g/section03/ub_overview_and_optimizer_implications
//
// Covers: UB lets optimizers assume impossible cases never happen

#include "learn/topic_registry.hpp"

#include <cassert>
#include <climits>
#include <optional>

namespace {

std::optional<int> safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return std::nullopt;
    return a + b;
}

void demo_basics() {
    auto r = safe_add(2, 3);
    assert(r && *r == 5);
}

void demo_intermediate() {
    assert(!safe_add(INT_MAX, 1).has_value());
}

void demo_expert() {
    assert(safe_add(INT_MIN, -1) == std::nullopt);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/ub_overview_and_optimizer_implications", run>;

}  // namespace
