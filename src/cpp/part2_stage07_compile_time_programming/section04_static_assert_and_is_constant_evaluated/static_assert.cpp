// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : static_assert
// Topic id : part2/stage07/section04/static_assert
//
// Covers: static_assert with/without message, in templates

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace {

template <typename T>
constexpr T safe_div(T a, T b) {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");
    return a / b;
}

template <std::size_t N>
struct PowerOfTwo {
    static_assert((N & (N - 1)) == 0 && N != 0, "N must be power of two");
    static constexpr std::size_t value = N;
};

void demo_basics() {
    static_assert(sizeof(int) >= 2);
    static_assert(true, "always ok");
    assert(true);
}

void demo_intermediate() {
    static_assert(safe_div(10, 2) == 5);
    assert(safe_div(9, 3) == 3);
    static_assert(PowerOfTwo<8>::value == 8);
}

void demo_expert() {
    static_assert(std::is_same_v<decltype(safe_div(1, 1)), int>);
    static_assert(PowerOfTwo<1>::value == 1);
    // C++17+: message optional; C++11 required message string.
    static_assert(1 + 1 == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/static_assert", run>;

}  // namespace
