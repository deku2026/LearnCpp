// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform
// Topic id : part2/stage09/section04/expected_transform
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_transform";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::expected<int, std::string> value{6};
    const std::expected<int, std::string> failure{std::unexpected{"missing"}};
    const auto doubled = value.transform([](int number) { return number * 2; });
    const auto untouched = failure.transform([](int number) { return number * 2; });
    LEARN_EXPECT_EQ(checks, doubled.value(), 12);
    LEARN_EXPECT_EQ(checks, untouched.error(), std::string{"missing"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "expected::transform");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_transform", run>;

}  // namespace
