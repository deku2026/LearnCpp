// A functor is an object whose operator() supplies call syntax and behavior.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : functor_basics
// Topic id : part2/stage03/section05/functor_basics
//
// Its named type can carry state and participate in compile-time callable traits.

#include "learn/example_support.hpp"

#include <algorithm>
#include <type_traits>
#include <vector>

namespace {

struct AffineTransform {
    int scale{};
    int offset{};

    constexpr int operator()(int value) const noexcept { return value * scale + offset; }
};

static_assert(AffineTransform{2, 1}(3) == 7);
static_assert(std::is_invocable_r_v<int, const AffineTransform&, int>);
static_assert(std::is_nothrow_invocable_v<const AffineTransform&, int>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section05/functor_basics"};

    const AffineTransform twice_plus_one{2, 1};
    LEARN_EXPECT_EQ(checks, twice_plus_one(5), 11);

    const std::vector<int> input{1, 2, 3};
    std::vector<int> output(input.size());
    std::transform(input.begin(), input.end(), output.begin(), twice_plus_one);
    LEARN_EXPECT_EQ(checks, output, std::vector<int>({3, 5, 7}));

    const auto equivalent_lambda = [scale = 2, offset = 1](int value) { return value * scale + offset; };
    LEARN_EXPECT_EQ(checks, equivalent_lambda(5), twice_plus_one(5));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/functor_basics", run>;

}  // namespace
