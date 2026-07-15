// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : signed_overflow_deep
// Topic id : part6/g/section03/signed_overflow_deep
//
// Covers: signed overflow is UB; use checks or unsigned carefully

#include "learn/topic_registry.hpp"

#include <climits>
#include <cstdint>

namespace {

bool would_add_overflow(int a, int b) {
    if (b > 0) return a > INT_MAX - b;
    if (b < 0) return a < INT_MIN - b;
    return false;
}

void demo_basics() {
    LEARN_CHECK(!would_add_overflow(1, 2));
    LEARN_CHECK(1 + 2 == 3);
}

void demo_intermediate() {
    LEARN_CHECK(would_add_overflow(INT_MAX, 1));
    LEARN_CHECK(would_add_overflow(INT_MIN, -1));
}

void demo_expert() {
    // Unsigned wrap is defined mod 2^n; still may be a logic bug.
    std::uint32_t u = 0xFFFFFFFFu;
    LEARN_CHECK(u + 1u == 0u);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/signed_overflow_deep", run>;

}  // namespace
