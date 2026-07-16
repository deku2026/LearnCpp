// C++14 function return deduction follows auto variable deduction rules.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : auto_return_type_deduction_cpp14
// Topic id : part2/stage03/section01/auto_return_type_deduction_cpp14
//
// Plain auto returns a value.  auto& or decltype(auto) is needed when a
// wrapper intentionally preserves a reference.

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>
#include <vector>

namespace {

auto copied_front(std::vector<int>& values) {
    return values.front();
}

auto& referenced_front(std::vector<int>& values) {
    return values.front();
}

decltype(auto) forwarded_front(std::vector<int>& values) {
    return (values.front());
}

auto choose_integer(bool first) {
    if (first) {
        return 10;
    }
    return 20;
}

static_assert(std::is_same_v<decltype(copied_front(std::declval<std::vector<int>&>())), int>);
static_assert(std::is_same_v<decltype(referenced_front(std::declval<std::vector<int>&>())), int&>);
static_assert(std::is_same_v<decltype(forwarded_front(std::declval<std::vector<int>&>())), int&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/auto_return_type_deduction_cpp14"};
    std::vector<int> values{7, 8};

    int copy = copied_front(values);
    copy = 70;
    LEARN_EXPECT_EQ(checks, copy, 70);
    LEARN_EXPECT_EQ(checks, values.front(), 7);

    referenced_front(values) = 71;
    LEARN_EXPECT_EQ(checks, values.front(), 71);
    forwarded_front(values) = 72;
    LEARN_EXPECT_EQ(checks, values.front(), 72);
    LEARN_EXPECT_EQ(checks, choose_integer(true) + choose_integer(false), 30);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/auto_return_type_deduction_cpp14", run>;

}  // namespace
