// C++20 explicit lambda template parameters expose and constrain their types.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : template_lambda_cpp20
// Topic id : part2/stage03/section03/template_lambda_cpp20
//
// Unlike independent auto parameters, the same named T below requires both
// arguments to have one deduced type.

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/template_lambda_cpp20"};

    const auto larger = []<std::totally_ordered T>(const T& left, const T& right) -> T {
        return left < right ? right : left;
    };
    LEARN_EXPECT_EQ(checks, larger(4, 9), 9);
    LEARN_EXPECT_EQ(checks, larger(std::string{"alpha"}, std::string{"beta"}), std::string{"beta"});

    const auto first_or = []<class T>(const std::vector<T>& values, T fallback) -> T {
        return values.empty() ? fallback : values.front();
    };
    LEARN_EXPECT_EQ(checks, first_or(std::vector<int>{5, 6}, 0), 5);
    LEARN_EXPECT_EQ(checks, first_or(std::vector<int>{}, 7), 7);

    const auto convert = []<class To, class From>(From value) -> To { return static_cast<To>(value); };
    static_assert(std::is_same_v<decltype(convert.operator()<long>(3.5)), long>);
    LEARN_EXPECT_EQ(checks, convert.operator()<long>(3.5), 3L);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/template_lambda_cpp20", run>;

}  // namespace
