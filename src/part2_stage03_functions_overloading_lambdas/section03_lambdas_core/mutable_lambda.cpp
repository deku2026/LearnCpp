// mutable removes const from a lambda closure's function call operator.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : mutable_lambda
// Topic id : part2/stage03/section03/mutable_lambda
//
// It permits mutation of captured copies.  Reference captures can modify the
// referent without mutable because the referent is not a closure data member.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/mutable_lambda"};

    int seed = 0;
    auto counter = [seed]() mutable { return ++seed; };
    static_assert(std::is_invocable_r_v<int, decltype(counter)&>);
    static_assert(!std::is_invocable_v<const decltype(counter)&>);

    LEARN_EXPECT_EQ(checks, counter(), 1);
    LEARN_EXPECT_EQ(checks, counter(), 2);
    LEARN_EXPECT_EQ(checks, seed, 0);

    auto increment_external = [&seed] { return ++seed; };
    static_assert(std::is_invocable_r_v<int, const decltype(increment_external)&>);
    LEARN_EXPECT_EQ(checks, increment_external(), 1);
    LEARN_EXPECT_EQ(checks, seed, 1);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/mutable_lambda", run>;

}  // namespace
