// C++17 lambdas can participate directly in constant evaluation.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : constexpr_lambda_cpp17
// Topic id : part2/stage03/section03/constexpr_lambda_cpp17
//
// A suitable call operator is implicitly constexpr; spelling constexpr makes
// that teaching intent explicit.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

constexpr auto square = [](int value) constexpr { return value * value; };

constexpr auto make_squares() {
    std::array<int, 4> result{};
    for (std::size_t index = 0; index < result.size(); ++index) {
        result[index] = square(static_cast<int>(index));
    }
    return result;
}

constexpr auto make_scaler(int factor) {
    return [factor](int value) constexpr { return factor * value; };
}

static_assert(square(5) == 25);
static_assert(make_squares()[3] == 9);
static_assert(make_scaler(3)(4) == 12);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/constexpr_lambda_cpp17"};
    constexpr auto scaled = make_scaler(3);
    static_assert(scaled(4) == 12);

    constexpr auto squares = make_squares();
    LEARN_EXPECT_EQ(checks, squares[0], 0);
    LEARN_EXPECT_EQ(checks, squares[2], 4);
    LEARN_EXPECT_EQ(checks, scaled(7), 21);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/constexpr_lambda_cpp17", run>;

}  // namespace
