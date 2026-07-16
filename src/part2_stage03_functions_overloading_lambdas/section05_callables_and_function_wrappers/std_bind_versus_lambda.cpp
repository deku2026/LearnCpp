// A lambda usually expresses argument binding more directly than std::bind.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_versus_lambda
// Topic id : part2/stage03/section05/std_bind_versus_lambda
//
// The bind example intentionally demonstrates placeholder reordering; the
// equivalent lambda makes the resulting argument order explicit.

#include "learn/example_support.hpp"

#include <functional>
#include <memory>
#include <type_traits>

namespace {

int digits(int hundreds, int tens, int ones) {
    return hundreds * 100 + tens * 10 + ones;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section05/std_bind_versus_lambda"};

    using namespace std::placeholders;
    auto bound = std::bind(digits, _2, 9, _1);
    auto clear_lambda = [](int first, int second) { return digits(second, 9, first); };
    LEARN_EXPECT_EQ(checks, bound(3, 4), 493);
    LEARN_EXPECT_EQ(checks, clear_lambda(3, 4), 493);

    auto owns_resource = [value = std::make_unique<int>(7)] { return *value; };
    static_assert(!std::is_copy_constructible_v<decltype(owns_resource)>);
    LEARN_EXPECT_EQ(checks, owns_resource(), 7);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_versus_lambda", run>;

}  // namespace
