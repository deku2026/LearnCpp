// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_never_constant_evaluated_cpp23
// Topic id : part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23
//
// Covers: constexpr functions that may never be constant-evaluated (still valid)

#include "learn/topic_registry.hpp"

#include <cmath>
#include <version>

namespace {

// Marked constexpr but typically only used at runtime (e.g. depends on IO-like data).
constexpr int clamp_int(int x, int lo, int hi) {
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

// Intentionally awkward for compile-time: large loop — still constexpr-capable.
constexpr int sum_n(int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) {
        s += i;
    }
    return s;
}

int runtime_only_seed() {
    // Not constexpr — feeds runtime path.
    return 7;
}

void demo_basics() {
    // Used at runtime only — still a valid constexpr function.
    int x = clamp_int(runtime_only_seed(), 0, 5);
    LEARN_CHECK(x == 5);
}

void demo_intermediate() {
    // Same function *can* be constant-evaluated when args are constant.
    constexpr int c = clamp_int(100, 0, 10);
    static_assert(c == 10);
    LEARN_CHECK(c == 10);
}

void demo_expert() {
    // "Never constant-evaluated" in practice is a usage property, not a language error.
    int n = runtime_only_seed();
    LEARN_CHECK(sum_n(n) == 21);  // 0..6

    constexpr int small = sum_n(5);
    static_assert(small == 10);
    LEARN_CHECK(small == 10);
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
    ::learn::topic<"part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23", run>;

}  // namespace
