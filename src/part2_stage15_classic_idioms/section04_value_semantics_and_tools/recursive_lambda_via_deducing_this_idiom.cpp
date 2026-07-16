// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : recursive_lambda_via_deducing_this_idiom
// Topic id : part2/stage15/section04/recursive_lambda_via_deducing_this_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/recursive_lambda_via_deducing_this_idiom";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    const auto factorial = [](this const auto& self, int value) -> int {
        return value < 2 ? 1 : value * self(value - 1);
    };
#else
    const auto factorial = [](auto&& self, int value) -> int { return value < 2 ? 1 : value * self(self, value - 1); };
#endif
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    LEARN_EXPECT_EQ(checks, factorial(5), 120);
#else
    LEARN_EXPECT_EQ(checks, factorial(factorial, 5), 120);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage15/section04/recursive_lambda_via_deducing_this_idiom", run>;

}  // namespace
