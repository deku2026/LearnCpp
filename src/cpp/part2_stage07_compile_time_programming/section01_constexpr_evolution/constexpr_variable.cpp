// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_variable
// Topic id : part2/stage07/section01/constexpr_variable
//
// Covers: constexpr variables, compile-time constants vs const

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>

namespace {

constexpr int kAnswer = 42;
constexpr double kPi = 3.141592653589793;
constexpr std::size_t kN = 4;

constexpr int fib(int n) {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

void demo_basics() {
    static_assert(kAnswer == 42);
    assert(kAnswer == 42);
    std::array<int, kN> a{};
    assert(a.size() == 4);
}

void demo_intermediate() {
    constexpr int f6 = fib(6);
    static_assert(f6 == 8);
    assert(f6 == 8);

    const int runtime_const = kAnswer;  // const, value from constexpr
    assert(runtime_const == 42);
}

void demo_expert() {
    constexpr auto twice = kAnswer * 2;
    static_assert(twice == 84);

    // constexpr object as NTTP-friendly constant.
    constexpr std::array<int, 3> primes{2, 3, 5};
    static_assert(primes[1] == 3);
    assert(primes[2] == 5);
    assert(kPi > 3.14 && kPi < 3.15);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_variable", run>;

}  // namespace
