// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fwrapv
// Topic id : part6/g/section05/fwrapv
//
// Covers: fwrapv defines signed wrap as two's complement (GCC/Clang dialect)

#include "learn/topic_registry.hpp"

#include <climits>
#include <optional>

namespace {

std::optional<int> checked_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return std::nullopt;
    return a + b;
}

void demo_basics() {
    LEARN_CHECK(checked_add(1, 1) == 2);
}

void demo_intermediate() {
    // Portable code should not rely on fwrapv; use checked ops or unsigned.
    LEARN_CHECK(!checked_add(INT_MAX, 1));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fwrapv", run>;

}  // namespace
