// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_lookup_table
// Topic id : part2/stage07/section05/compile_time_lookup_table
//
// Covers: constexpr lookup tables (squares, factorial)

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>

namespace {

constexpr std::array<int, 11> make_squares() {
    std::array<int, 11> a{};
    for (std::size_t i = 0; i < a.size(); ++i) {
        a[i] = static_cast<int>(i * i);
    }
    return a;
}

constexpr auto kSquares = make_squares();

constexpr int fact(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

constexpr std::array<int, 8> make_facts() {
    std::array<int, 8> a{};
    for (std::size_t i = 0; i < a.size(); ++i) {
        a[i] = fact(static_cast<int>(i));
    }
    return a;
}

void demo_basics() {
    static_assert(kSquares[0] == 0);
    static_assert(kSquares[5] == 25);
    assert(kSquares[9] == 81);
}

void demo_intermediate() {
    constexpr auto facts = make_facts();
    static_assert(facts[5] == 120);
    assert(facts[0] == 1);
    assert(facts[7] == 5040);
}

void demo_expert() {
    // Index with runtime value into compile-time table.
    int i = 4;
    assert(kSquares[static_cast<std::size_t>(i)] == 16);
    constexpr int s = kSquares[3];
    static_assert(s == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_lookup_table", run>;

}  // namespace
