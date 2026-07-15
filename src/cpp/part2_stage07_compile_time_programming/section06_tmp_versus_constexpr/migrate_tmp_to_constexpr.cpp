// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section06_tmp_versus_constexpr
// Item     : migrate_tmp_to_constexpr
// Topic id : part2/stage07/section06/migrate_tmp_to_constexpr
//
// Covers: same computation as TMP structs vs constexpr functions

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

// Classic TMP
template <int N>
struct FactTmp {
    static constexpr int value = N * FactTmp<N - 1>::value;
};
template <>
struct FactTmp<0> {
    static constexpr int value = 1;
};

// Modern constexpr
constexpr int fact_cx(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

template <int N>
struct FibTmp {
    static constexpr int value = FibTmp<N - 1>::value + FibTmp<N - 2>::value;
};
template <>
struct FibTmp<0> {
    static constexpr int value = 0;
};
template <>
struct FibTmp<1> {
    static constexpr int value = 1;
};

constexpr int fib_cx(int n) {
    if (n <= 1) {
        return n;
    }
    int a = 0;
    int b = 1;
    for (int i = 2; i <= n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

void demo_basics() {
    static_assert(FactTmp<5>::value == 120);
    static_assert(fact_cx(5) == 120);
    LEARN_CHECK(fact_cx(5) == FactTmp<5>::value);
}

void demo_intermediate() {
    static_assert(FibTmp<10>::value == 55);
    static_assert(fib_cx(10) == 55);
    LEARN_CHECK(fib_cx(10) == FibTmp<10>::value);
}

void demo_expert() {
    // constexpr works with runtime args too; TMP is fixed at instantiation.
    int n = 6;
    LEARN_CHECK(fact_cx(n) == 720);
    static_assert(std::is_same_v<std::integral_constant<int, FactTmp<4>::value>, std::integral_constant<int, 24>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section06/migrate_tmp_to_constexpr", run>;

}  // namespace
