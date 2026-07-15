// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_immediate_functions
// Topic id : part2/stage07/section03/consteval_immediate_functions
//
// Covers: consteval immediate functions (must produce constant)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>

namespace {

consteval int imm_square(int x) {
    return x * x;
}

consteval std::size_t strlen_c(const char* s) {
    std::size_t n = 0;
    while (s[n] != '\0') {
        ++n;
    }
    return n;
}

consteval int fib(int n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

void demo_basics() {
    constexpr int a = imm_square(9);
    static_assert(a == 81);
    assert(a == 81);
}

void demo_intermediate() {
    static_assert(strlen_c("hello") == 5);
    assert(strlen_c("hi") == 2);
}

void demo_expert() {
    static_assert(fib(10) == 55);
    // Cannot pass runtime int to consteval — only constant expressions.
    constexpr int x = 4;
    assert(imm_square(x) == 16);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/consteval_immediate_functions", run>;

}  // namespace
