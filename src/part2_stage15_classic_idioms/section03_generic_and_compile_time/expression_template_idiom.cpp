// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : expression_template_idiom
// Topic id : part2/stage15/section03/expression_template_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/expression_template_idiom";

template <class Derived>
struct VectorExpression {
    double operator[](std::size_t index) const { return static_cast<const Derived&>(*this)[index]; }
};

struct Vector : VectorExpression<Vector> {
    std::array<double, 3> values{};
    double operator[](std::size_t index) const { return values[index]; }
};

template <class Left, class Right>
struct Sum : VectorExpression<Sum<Left, Right>> {
    Sum(const Left& left_operand, const Right& right_operand) : left(left_operand), right(right_operand) {}
    const Left& left;
    const Right& right;
    double operator[](std::size_t index) const { return left[index] + right[index]; }
};

template <class Left, class Right>
Sum<Left, Right> operator+(const VectorExpression<Left>& left, const VectorExpression<Right>& right) {
    return {static_cast<const Left&>(left), static_cast<const Right&>(right)};
}

template <class Expression>
Vector evaluate(const VectorExpression<Expression>& expression) {
    Vector result;
    for (std::size_t index = 0; index < result.values.size(); ++index) result.values[index] = expression[index];
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Vector first{{}, {1.0, 2.0, 3.0}};
    const Vector second{{}, {4.0, 5.0, 6.0}};
    const Vector third{{}, {1.0, 1.0, 1.0}};
    const Vector result = evaluate(first + second + third);
    LEARN_EXPECT_EQ(checks, result.values, (std::array<double, 3>({6.0, 8.0, 10.0})));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/expression_template_idiom", run>;

}  // namespace
