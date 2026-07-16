// C++17 fold expressions reduce a pack with explicit left/right association.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : fold_expressions_cpp17
// Topic id : part2/stage06/section02/fold_expressions_cpp17
//
// Subtraction makes association observable; binary folds also give empty packs an initial value.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class... Values>
constexpr auto right_subtract(Values... values) {
    return (values - ...);
}

template <class... Values>
constexpr auto left_subtract(Values... values) {
    return (... - values);
}

template <class... Values>
constexpr auto sum_with_zero(Values... values) {
    return (0 + ... + values);
}

template <class... Values>
constexpr bool all(Values... values) {
    return (values && ...);
}

static_assert(right_subtract(10, 3, 1) == 8);
static_assert(left_subtract(10, 3, 1) == 6);
static_assert(sum_with_zero() == 0);
static_assert(all());

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section02/fold_expressions_cpp17"};
    LEARN_EXPECT_EQ(checks, right_subtract(20, 5, 2), 17);
    LEARN_EXPECT_EQ(checks, left_subtract(20, 5, 2), 13);
    LEARN_EXPECT_EQ(checks, sum_with_zero(1, 2, 3, 4), 10);
    LEARN_EXPECT(checks, all(true, true, true));
    LEARN_EXPECT(checks, !all(true, false, true));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/fold_expressions_cpp17", run>;

}  // namespace
