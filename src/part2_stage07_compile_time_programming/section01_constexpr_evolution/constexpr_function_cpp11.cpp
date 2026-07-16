// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_cpp11
// Topic id : part2/stage07/section01/constexpr_function_cpp11
// References: N3337/N4950 [dcl.constexpr], [expr.const].

#include "learn/example_support.hpp"

#include <array>

namespace {

// This single-return recursive style obeys the original C++11 constexpr-body
// restrictions. The same function is still an ordinary callable at runtime.
constexpr unsigned factorial_cpp11(unsigned value) noexcept {
    return value < 2U ? 1U : value * factorial_cpp11(value - 1U);
}

constexpr int absolute_cpp11(int value) noexcept {
    return value < 0 ? -value : value;
}

static_assert(factorial_cpp11(0) == 1);
static_assert(factorial_cpp11(5) == 120);
static_assert(absolute_cpp11(-42) == 42);

using SixElements = std::array<int, factorial_cpp11(3)>;
static_assert(SixElements{}.size() == 6);

int run(int argc, char** argv) {
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_function_cpp11"};

    unsigned runtime_argument = 6U;
    runtime_argument += static_cast<unsigned>(argc - argc);
    LEARN_EXPECT_EQ(checks, factorial_cpp11(runtime_argument), 720U);
    LEARN_EXPECT_EQ(checks, absolute_cpp11(-7), 7);

    constexpr auto compile_time_result = factorial_cpp11(5);
    LEARN_EXPECT_EQ(checks, compile_time_result, 120U);
    LEARN_EXPECT_EQ(checks, SixElements{}.size(), 6uz);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_function_cpp11", run>;

}  // namespace
