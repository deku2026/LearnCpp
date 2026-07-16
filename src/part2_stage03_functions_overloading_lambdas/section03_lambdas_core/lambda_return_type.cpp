// Lambda return types may be deduced, explicit, or decltype(auto).
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : lambda_return_type
// Topic id : part2/stage03/section03/lambda_return_type
//
// Plain deduction requires one deduced type and returns by value.  An explicit
// result can unify branches; decltype(auto) can intentionally preserve a reference.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/lambda_return_type"};

    const auto deduced = [](bool first) {
        if (first) {
            return 1;
        }
        return 2;
    };
    const auto explicit_double = [](bool first) -> double {
        if (first) {
            return 1;
        }
        return 2.5;
    };

    int value = 7;
    auto copied = [&value] { return (value); };
    auto referenced = [&value]() -> decltype(auto) { return (value); };
    static_assert(std::is_same_v<decltype(deduced(true)), int>);
    static_assert(std::is_same_v<decltype(explicit_double(true)), double>);
    static_assert(std::is_same_v<decltype(copied()), int>);
    static_assert(std::is_same_v<decltype(referenced()), int&>);

    LEARN_EXPECT_EQ(checks, deduced(false), 2);
    LEARN_EXPECT_EQ(checks, explicit_double(false), 2.5);
    referenced() = 11;
    LEARN_EXPECT_EQ(checks, value, 11);
    LEARN_EXPECT_EQ(checks, copied(), 11);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/lambda_return_type", run>;

}  // namespace
