// A C++14 auto parameter makes the closure's operator() a function template.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : generic_lambda_cpp14
// Topic id : part2/stage03/section03/generic_lambda_cpp14
//
// Separate auto parameters are deduced independently.  A forwarding auto&&
// parameter can also preserve a referenced result.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/generic_lambda_cpp14"};

    const auto add = [](const auto& left, const auto& right) { return left + right; };
    static_assert(std::is_same_v<decltype(add(1, 2)), int>);
    static_assert(std::is_same_v<decltype(add(1, 2.5)), double>);
    LEARN_EXPECT_EQ(checks, add(2, 3), 5);
    LEARN_EXPECT_EQ(checks, add(std::string{"generic "}, std::string{"lambda"}), std::string{"generic lambda"});

    auto forward_identity = [](auto&& value) -> decltype(auto) { return std::forward<decltype(value)>(value); };
    int number = 8;
    static_assert(std::is_same_v<decltype(forward_identity(number)), int&>);
    forward_identity(number) = 9;
    LEARN_EXPECT_EQ(checks, number, 9);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/generic_lambda_cpp14", run>;

}  // namespace
