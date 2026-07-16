// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : optional_and_then_transform_or_else_cpp23
// Topic id : part2/stage09/section05/optional_and_then_transform_or_else_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section05/optional_and_then_transform_or_else_cpp23";

#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
std::optional<int> positive(int value) {
    return value > 0 ? std::optional{value} : std::nullopt;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    bool fallback_called = false;
    const auto good = std::optional{3}.and_then(positive).transform([](int value) { return value * 2; });
    const auto recovered = std::optional<int>{}.or_else([&] {
        fallback_called = true;
        return std::optional{9};
    });
    LEARN_EXPECT_EQ(checks, good.value(), 6);
    LEARN_EXPECT_EQ(checks, recovered.value(), 9);
    LEARN_EXPECT(checks, fallback_called);
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 optional monadic operations");
}
#endif

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section05/optional_and_then_transform_or_else_cpp23", run>;

}  // namespace
