// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_relaxations_cpp14_17_20
// Topic id : part2/stage07/section01/constexpr_relaxations_cpp14_17_20
//
// Covers: constexpr loops/locals (14), lambda (17), virtual/try/more (20)

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

// C++14: multiple statements, loops, locals
constexpr int sum_to(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i) {
        s += i;
    }
    return s;
}

// C++17: constexpr lambda
constexpr auto make_adder(int x) {
    return [x](int y) constexpr { return x + y; };
}

// C++20: more can be constexpr (virtual in limited cases; here use union-free logic)
constexpr int max_arr(const int* p, int n) {
    int m = p[0];
    for (int i = 1; i < n; ++i) {
        if (p[i] > m) {
            m = p[i];
        }
    }
    return m;
}

struct Point {
    int x = 0;
    int y = 0;
    constexpr Point(int a, int b) : x(a), y(b) {}
    constexpr int manh() const { return (x < 0 ? -x : x) + (y < 0 ? -y : y); }
};

void demo_basics() {
    static_assert(sum_to(10) == 55);
    LEARN_CHECK(sum_to(10) == 55);
}

void demo_intermediate() {
    constexpr auto add5 = make_adder(5);
    static_assert(add5(7) == 12);
    LEARN_CHECK(add5(7) == 12);
}

void demo_expert() {
    constexpr int data[] = {3, 9, 1, 7};
    static_assert(max_arr(data, 4) == 9);

    constexpr Point p{3, -4};
    static_assert(p.manh() == 7);
    LEARN_CHECK(p.manh() == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_relaxations_cpp14_17_20", run>;

}  // namespace
