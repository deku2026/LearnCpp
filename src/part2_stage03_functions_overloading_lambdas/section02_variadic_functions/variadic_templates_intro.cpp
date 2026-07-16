// Variadic templates retain every argument's type and value category.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : variadic_templates_intro
// Topic id : part2/stage03/section02/variadic_templates_intro
//
// Fold expressions reduce a pack, while forwarding lets a wrapper invoke a
// callable without losing lvalue/rvalue information.

#include "learn/example_support.hpp"

#include <concepts>
#include <cstddef>
#include <functional>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class... Values>
    requires(sizeof...(Values) > 0 && (std::is_arithmetic_v<Values> && ...))
constexpr auto total(Values... values) {
    using Result = std::common_type_t<Values...>;
    return (Result{} + ... + static_cast<Result>(values));
}

template <class... Values>
std::string join_with_commas(const Values&... values) {
    std::ostringstream output;
    std::size_t index = 0;
    ((output << (index++ == 0 ? "" : ",") << values), ...);
    return output.str();
}

template <class Callable, class... Arguments>
decltype(auto) invoke_counted(std::size_t& calls, Callable&& callable, Arguments&&... arguments) {
    ++calls;
    return std::invoke(std::forward<Callable>(callable), std::forward<Arguments>(arguments)...);
}

static_assert(total(1, 2, 3, 4) == 10);
static_assert(std::is_same_v<decltype(total(1, 2.5)), double>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section02/variadic_templates_intro"};
    LEARN_EXPECT_EQ(checks, total(1, 2.5, 3U), 6.5);
    LEARN_EXPECT_EQ(checks, join_with_commas(1, "two", 3.5), std::string{"1,two,3.5"});

    std::size_t calls = 0;
    auto describe = [](std::string prefix, int value) { return prefix + std::to_string(value); };
    LEARN_EXPECT_EQ(checks, invoke_counted(calls, describe, std::string{"value="}, 7), std::string{"value=7"});
    LEARN_EXPECT_EQ(checks, calls, std::size_t{1});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/variadic_templates_intro", run>;

}  // namespace
