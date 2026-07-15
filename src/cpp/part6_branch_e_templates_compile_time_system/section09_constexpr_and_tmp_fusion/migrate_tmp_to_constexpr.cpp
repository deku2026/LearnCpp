// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : migrate_tmp_to_constexpr
// Topic id : part6/e/section09/migrate_tmp_to_constexpr
//
// Covers: prefer constexpr functions over pure type-level TMP when possible

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

// Old TMP style
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};
template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Modern constexpr
constexpr int factorial(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

void demo_basics() {
    static_assert(Factorial<5>::value == 120);
    static_assert(factorial(5) == 120);
}

void demo_intermediate() {
    assert(factorial(6) == 720);
}

void demo_expert() {
    assert(Factorial<4>::value == factorial(4));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/migrate_tmp_to_constexpr", run>;

}  // namespace
