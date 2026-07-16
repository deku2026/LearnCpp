// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_variable
// Topic id : part2/stage07/section01/constexpr_variable
// References: N4950 [dcl.constexpr], [expr.const], [basic.types].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

inline constexpr std::size_t extent = 4;
inline constexpr std::array<int, extent> weights{2, 3, 5, 7};

struct Point {
    int x;
    int y;
    constexpr int manhattan() const noexcept { return (x < 0 ? -x : x) + (y < 0 ? -y : y); }
};

inline constexpr Point compile_time_point{-3, 4};

template <int Value>
struct ValueTag {
    static constexpr int value = Value;
};

using WeightSumTag = ValueTag<weights[0] + weights[1] + weights[2] + weights[3]>;

static_assert(std::is_const_v<std::remove_reference_t<decltype((extent))>>);
static_assert(weights.size() == extent);
static_assert(compile_time_point.manhattan() == 7);
static_assert(WeightSumTag::value == 17);

int run(int argc, char** argv) {
    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_variable"};

    const int runtime_read_only = argc;  // const does not imply a constant expression.
    LEARN_EXPECT_EQ(checks, runtime_read_only, argc);
    LEARN_EXPECT_EQ(checks, weights.front(), 2);
    LEARN_EXPECT_EQ(checks, weights.back(), 7);
    LEARN_EXPECT_EQ(checks, compile_time_point.manhattan(), 7);
    LEARN_EXPECT_EQ(checks, WeightSumTag::value, 17);

    (void)argv;
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_variable", run>;

}  // namespace
