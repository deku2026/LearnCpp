// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : std_is_constant_evaluated_cpp20
// Topic id : part2/stage07/section04/std_is_constant_evaluated_cpp20
//
// Covers: std::is_constant_evaluated() for dual-path algorithms

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

constexpr int smart_abs(int x) {
    if (std::is_constant_evaluated()) {
        return x < 0 ? -x : x;  // pure path
    }
    // Runtime could use platform abs; keep pure for determinism.
    return x < 0 ? -x : x;
}

constexpr int path_id() {
    return std::is_constant_evaluated() ? 1 : 2;
}

void demo_basics() {
    constexpr int a = smart_abs(-5);
    static_assert(a == 5);
    LEARN_CHECK(smart_abs(-7) == 7);
}

void demo_intermediate() {
    constexpr int p = path_id();
    static_assert(p == 1);
    LEARN_CHECK(path_id() == 2);
}

void demo_expert() {
    // Common pattern: pick bit-ops at compile time, library at runtime.
    auto popcount_like = [](unsigned x) constexpr {
        if (std::is_constant_evaluated()) {
            int c = 0;
            while (x) {
                c += static_cast<int>(x & 1u);
                x >>= 1;
            }
            return c;
        }
        int c = 0;
        while (x) {
            c += static_cast<int>(x & 1u);
            x >>= 1;
        }
        return c;
    };
    constexpr int c = popcount_like(0b1011u);
    static_assert(c == 3);
    LEARN_CHECK(popcount_like(0b1000u) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/std_is_constant_evaluated_cpp20", run>;

}  // namespace
