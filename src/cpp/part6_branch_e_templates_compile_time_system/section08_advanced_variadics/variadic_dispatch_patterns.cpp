// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section08_advanced_variadics
// Item     : variadic_dispatch_patterns
// Topic id : part6/e/section08/variadic_dispatch_patterns
//
// Covers: variadic fold and recursive dispatch

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

template <class... Ts>
int sum_all(Ts... xs) {
    return (0 + ... + static_cast<int>(xs));
}

template <class F, class... Ts>
void for_each_arg(F f, Ts&&... ts) {
    (f(std::forward<Ts>(ts)), ...);
}

void demo_basics() {
    assert(sum_all(1, 2, 3) == 6);
}

void demo_intermediate() {
    int acc = 0;
    for_each_arg([&](int x) { acc += x; }, 1, 2, 3, 4);
    assert(acc == 10);
}

void demo_expert() {
    assert(sum_all() == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section08/variadic_dispatch_patterns", run>;

}  // namespace
