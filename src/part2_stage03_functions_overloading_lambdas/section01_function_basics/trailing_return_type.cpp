// Trailing return types can name parameters and clarify complex declarators.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : trailing_return_type
// Topic id : part2/stage03/section01/trailing_return_type
//
// They are especially useful when a dependent return type is expressed with
// decltype, or when the result itself is a function pointer.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <vector>

namespace {

template <class Left, class Right>
auto add(const Left& left, const Right& right) -> decltype(left + right) {
    return left + right;
}

template <class Container>
auto first(Container& values) -> decltype((values.front())) {
    return values.front();
}

int add_ints(int left, int right) {
    return left + right;
}

int subtract_ints(int left, int right) {
    return left - right;
}

using BinaryOperation = int (*)(int, int);

auto choose_operation(bool addition) -> BinaryOperation {
    return addition ? &add_ints : &subtract_ints;
}

static_assert(std::is_same_v<decltype(add(1, 2)), int>);
static_assert(std::is_same_v<decltype(add(1, 0.5)), double>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/trailing_return_type"};
    LEARN_EXPECT_EQ(checks, add(2, 3), 5);
    LEARN_EXPECT_EQ(checks, add(std::string{"modern "}, std::string{"C++"}), std::string{"modern C++"});

    std::vector<int> values{4, 5, 6};
    static_assert(std::is_same_v<decltype(first(values)), int&>);
    first(values) = 40;
    LEARN_EXPECT_EQ(checks, values.front(), 40);

    LEARN_EXPECT_EQ(checks, choose_operation(true)(9, 4), 13);
    LEARN_EXPECT_EQ(checks, choose_operation(false)(9, 4), 5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/trailing_return_type", run>;

}  // namespace
