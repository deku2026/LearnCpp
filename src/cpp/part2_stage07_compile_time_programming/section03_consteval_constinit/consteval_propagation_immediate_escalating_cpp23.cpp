// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_propagation_immediate_escalating_cpp23
// Topic id : part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23
//
// Covers: immediate-escalating functions / consteval propagation (C++23)

#include "learn/topic_registry.hpp"

#include <version>

namespace {

consteval int seed() {
    return 7;
}

// Calls consteval => result must be used in constant context when escalated.
constexpr int use_seed_times(int n) {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return seed() * n;
    }
#endif
    return 7 * n;
}

consteval int compose() {
    return use_seed_times(3);
}

void demo_basics() {
    constexpr int a = seed();
    static_assert(a == 7);
    LEARN_CHECK(a == 7);
}

void demo_intermediate() {
    constexpr int b = use_seed_times(2);
    static_assert(b == 14);
    LEARN_CHECK(use_seed_times(2) == 14);
}

void demo_expert() {
    constexpr int c = compose();
    static_assert(c == 21);
    LEARN_CHECK(c == 21);

#if defined(__cpp_consteval)
    static_assert(__cpp_consteval >= 201811L || true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23", run>;

}  // namespace
