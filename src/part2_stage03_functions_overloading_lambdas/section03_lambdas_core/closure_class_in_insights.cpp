// A lambda expression creates an object of a unique unnamed closure class.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : closure_class_in_insights
// Topic id : part2/stage03/section03/closure_class_in_insights
//
// The hand-written functor models the closure's state plus operator().  Two
// textually identical lambda expressions still have different closure types.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct ManualAdder {
    int base{};

    int operator()(int value) const { return base + value; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/closure_class_in_insights"};

    const ManualAdder manual{5};
    const auto generated = [base = 5](int value) { return base + value; };
    const auto another_expression = [base = 5](int value) { return base + value; };
    const auto stateless = [](int value) { return value * 2; };

    static_assert(std::is_class_v<decltype(generated)>);
    static_assert(std::is_invocable_r_v<int, const decltype(generated)&, int>);
    static_assert(!std::is_same_v<decltype(generated), decltype(another_expression)>);
    static_assert(std::is_convertible_v<decltype(stateless), int (*)(int)>);

    LEARN_EXPECT_EQ(checks, manual(7), 12);
    LEARN_EXPECT_EQ(checks, generated(7), manual(7));
    int (*function_pointer)(int) = stateless;
    LEARN_EXPECT_EQ(checks, function_pointer(6), 12);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/closure_class_in_insights", run>;

}  // namespace
