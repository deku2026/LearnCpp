// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_and_then
// Topic id : part2/stage09/section04/expected_and_then
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_and_then";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
std::expected<int, std::string> nonzero(int value) {
    if (value == 0) {
        return std::unexpected{"zero"};
    }
    return value;
}

std::expected<int, std::string> reciprocal_scale(int value) {
    return 100 / value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto good = nonzero(4).and_then(reciprocal_scale);
    const auto bad = nonzero(0).and_then(reciprocal_scale);
    LEARN_EXPECT_EQ(checks, good.value(), 25);
    LEARN_EXPECT_EQ(checks, bad.error(), std::string{"zero"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "expected::and_then");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_and_then", run>;

}  // namespace
