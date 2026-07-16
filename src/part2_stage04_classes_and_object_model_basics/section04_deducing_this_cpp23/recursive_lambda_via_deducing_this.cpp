// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : recursive_lambda_via_deducing_this
// Topic id : part2/stage04/section04/recursive_lambda_via_deducing_this
// References: [expr.prim.lambda], P0847R7

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/recursive_lambda_via_deducing_this";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    const auto factorial = [](this auto self, int value) -> int { return value < 2 ? 1 : value * self(value - 1); };
    LEARN_EXPECT_EQ(checks, factorial(0), 1);
    LEARN_EXPECT_EQ(checks, factorial(6), 720);
#else
    const auto factorial = [](auto&& self, int value) -> int { return value < 2 ? 1 : value * self(self, value - 1); };
    LEARN_EXPECT_EQ(checks, factorial(factorial, 0), 1);
    LEARN_EXPECT_EQ(checks, factorial(factorial, 6), 720);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section04/recursive_lambda_via_deducing_this", run>;

}  // namespace
