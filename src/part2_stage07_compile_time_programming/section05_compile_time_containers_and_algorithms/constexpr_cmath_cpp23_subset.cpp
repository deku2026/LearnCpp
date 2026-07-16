// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_cmath_cpp23_subset
// Topic id : part2/stage07/section05/constexpr_cmath_cpp23_subset
// References: N4950 [cmath.syn], [c.math]; P0533R9.

#include "learn/example_support.hpp"

#include <cmath>
#include <limits>

namespace {

// P0533 deliberately selected functions that close over rational values and
// do not strongly depend on the runtime rounding mode. It did not make every
// math function constexpr: sqrt, sin, cos, and exp are outside this subset.
inline constexpr double infinity = std::numeric_limits<double>::infinity();
inline constexpr double quiet_nan = std::numeric_limits<double>::quiet_NaN();

static_assert(std::isfinite(1.0));
static_assert(!std::isfinite(infinity));
static_assert(std::isinf(infinity));
static_assert(std::isnan(quiet_nan));
static_assert(std::isless(1.0, 2.0));

#if 0
// P0533 includes these, but the current MSVC UCRT overloads selected by
// clang-cl are not yet constexpr. They remain valid runtime operations.
static_assert(std::signbit(-0.0));
static_assert(std::fpclassify(0.0) == FP_ZERO);

// C++23 does not require this to be a constant expression.
static_assert(std::sqrt(81.0) == 9.0);
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section05/constexpr_cmath_cpp23_subset"};

    LEARN_EXPECT(checks, std::isfinite(42.0));
    LEARN_EXPECT(checks, std::isinf(infinity));
    LEARN_EXPECT(checks, std::isnan(quiet_nan));
    LEARN_EXPECT(checks, std::isgreater(3.0, 2.0));
    LEARN_EXPECT(checks, std::signbit(-0.0));
    LEARN_EXPECT_EQ(checks, std::fpclassify(0.0), FP_ZERO);
    LEARN_EXPECT_EQ(checks, std::sqrt(81.0), 9.0);  // Valid runtime path, not a C++23 guarantee at compile time.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_cmath_cpp23_subset", run>;

}  // namespace
