// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_cpp11
// Topic id : part2/stage07/section01/constexpr_function_cpp11
//
// Covers: C++11-style constexpr functions (single return, usable at compile time)

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

constexpr int square(int x) {
    return x * x;
}

constexpr int abs_cpp11(int x) {
    return x < 0 ? -x : x;
}

constexpr int min_v(int a, int b) {
    return a < b ? a : b;
}

void demo_basics() {
    static_assert(square(5) == 25);
    constexpr int s = square(7);
    assert(s == 49);
}

void demo_intermediate() {
    static_assert(abs_cpp11(-3) == 3);
    static_assert(min_v(4, 2) == 2);
    int runtime = 6;
    assert(square(runtime) == 36);  // also callable at runtime
}

void demo_expert() {
    constexpr int table[] = {square(1), square(2), square(3), square(4)};
    static_assert(table[3] == 16);
    assert(table[2] == 9);
    assert(min_v(abs_cpp11(-10), square(3)) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_function_cpp11", run>;

}  // namespace
