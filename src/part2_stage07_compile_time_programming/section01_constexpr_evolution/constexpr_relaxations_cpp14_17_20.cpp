// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_relaxations_cpp14_17_20
// Topic id : part2/stage07/section01/constexpr_relaxations_cpp14_17_20
// References: N4950 [dcl.constexpr], [expr.const]; N3652, P0170R1, P0784R7.

#include "learn/example_support.hpp"

namespace {

// C++14: local variables, mutation, loops, and multiple statements.
constexpr unsigned factorial_cpp14(unsigned value) noexcept {
    unsigned result = 1;
    for (unsigned factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

// C++17: lambda call operators can participate in constant evaluation.
inline constexpr auto square_cpp17 = [](int value) constexpr noexcept { return value * value; };

// C++20: transient dynamic allocation is allowed when storage is released
// before constant evaluation finishes.
constexpr int allocated_sum_cpp20() {
    auto* values = new int[4]{1, 2, 3, 4};
    int result{};
    for (int index = 0; index < 4; ++index) {
        result += values[index];
    }
    delete[] values;
    return result;
}

struct ScoreSource {
    virtual constexpr int score() const noexcept = 0;
};

struct FixedScore final : ScoreSource {
    int value;
    constexpr explicit FixedScore(int score_value) noexcept : value(score_value) {}
    constexpr int score() const noexcept override { return value; }
};

// C++20 also permits virtual dispatch during constant evaluation when the
// dynamic object is known to the evaluator.
constexpr int polymorphic_score_cpp20(const ScoreSource& source) noexcept {
    return source.score();
}

static_assert(factorial_cpp14(6) == 720);
static_assert(square_cpp17(9) == 81);
static_assert(allocated_sum_cpp20() == 10);
static_assert(polymorphic_score_cpp20(FixedScore{42}) == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_relaxations_cpp14_17_20"};

    LEARN_EXPECT_EQ(checks, factorial_cpp14(5), 120U);
    LEARN_EXPECT_EQ(checks, square_cpp17(-7), 49);
    LEARN_EXPECT_EQ(checks, allocated_sum_cpp20(), 10);

    const FixedScore source{7};
    LEARN_EXPECT_EQ(checks, polymorphic_score_cpp20(source), 7);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_relaxations_cpp14_17_20", run>;

}  // namespace
