// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_recursion
// Topic id : part6/e/section04/compile_time_recursion
//
// Covers: compile-time recursion for factorial / sum

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

template <int N>
struct Fact {
    static constexpr int value = N * Fact<N - 1>::value;
};
template <>
struct Fact<0> {
    static constexpr int value = 1;
};

constexpr int sum_to(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i) s += i;
    return s;
}

void demo_basics() {
    static_assert(Fact<5>::value == 120);
    assert(Fact<4>::value == 24);
}

void demo_intermediate() {
    static_assert(sum_to(5) == 15);
}

void demo_expert() {
    static_assert(Fact<0>::value == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_recursion", run>;

}  // namespace
