// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section07_expression_templates
// Item     : expression_template_lazy_evaluation
// Topic id : part6/e/section07/expression_template_lazy_evaluation

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace {

class NumericVector {
public:
    NumericVector(std::initializer_list<double> values) : values_(values) {}
    explicit NumericVector(std::size_t size) : values_(size) {}

    [[nodiscard]] std::size_t size() const noexcept { return values_.size(); }
    [[nodiscard]] double operator[](std::size_t index) const noexcept { return values_[index]; }
    [[nodiscard]] double& operator[](std::size_t index) noexcept { return values_[index]; }

private:
    std::vector<double> values_;
};

template <class Left, class Right>
class AddExpression {
public:
    AddExpression(const Left& left, const Right& right, std::size_t& evaluations) noexcept
        : left_(left), right_(right), evaluations_(evaluations) {}

    [[nodiscard]] std::size_t size() const noexcept { return std::min(left_.size(), right_.size()); }
    [[nodiscard]] double operator[](std::size_t index) const noexcept {
        ++evaluations_;
        return left_[index] + right_[index];
    }

private:
    const Left& left_;
    const Right& right_;
    std::size_t& evaluations_;
};

template <class Expression>
[[nodiscard]] NumericVector evaluate(const Expression& expression) {
    NumericVector result(expression.size());
    for (std::size_t index = 0; index < expression.size(); ++index) {
        result[index] = expression[index];
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section07/expression_template_lazy_evaluation"};

    const NumericVector left{1.0, 2.0, 3.0};
    const NumericVector right{10.0, 20.0, 30.0};
    std::size_t element_evaluations = 0;
    const AddExpression expression{left, right, element_evaluations};
    LEARN_EXPECT_EQ(checks, element_evaluations, 0U);  // Construction is lazy.
    const auto result = evaluate(expression);          // One fused output pass.
    LEARN_EXPECT_EQ(checks, element_evaluations, 3U);
    LEARN_EXPECT_EQ(checks, result[0], 11.0);
    LEARN_EXPECT_EQ(checks, result[2], 33.0);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section07/expression_template_lazy_evaluation", run>;

}  // namespace
