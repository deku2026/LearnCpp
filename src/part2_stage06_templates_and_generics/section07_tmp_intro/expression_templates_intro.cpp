// An expression template stores an operation and evaluates elements lazily.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : expression_templates_intro
// Topic id : part2/stage06/section07/expression_templates_intro
//
// AddExpression references two live vectors; evaluation creates one final result and no intermediate vector.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

template <std::size_t Size>
struct Vector {
    std::array<double, Size> values{};

    double& operator[](std::size_t index) { return values[index]; }
    double operator[](std::size_t index) const { return values[index]; }
};

template <std::size_t Size>
struct AddExpression {
    const Vector<Size>& left;
    const Vector<Size>& right;

    double operator[](std::size_t index) const { return left[index] + right[index]; }
};

template <std::size_t Size>
AddExpression<Size> add(const Vector<Size>& left, const Vector<Size>& right) {
    return {left, right};
}

template <std::size_t Size>
Vector<Size> evaluate(const AddExpression<Size>& expression) {
    Vector<Size> result;
    for (std::size_t index = 0; index < Size; ++index) {
        result[index] = expression[index];
    }
    return result;
}

static_assert(!std::is_same_v<AddExpression<3>, Vector<3>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section07/expression_templates_intro"};
    Vector<3> left{{1.0, 2.0, 3.0}};
    const Vector<3> right{{10.0, 20.0, 30.0}};
    const auto expression = add(left, right);
    LEARN_EXPECT_EQ(checks, expression[0], 11.0);

    left[0] = 5.0;
    LEARN_EXPECT_EQ(checks, expression[0], 15.0);
    const auto result = evaluate(expression);
    LEARN_EXPECT_EQ(checks, result.values, (std::array<double, 3>{15.0, 22.0, 33.0}));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/expression_templates_intro", run>;

}  // namespace
