// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_value_and_error
// Topic id : part2/stage09/section04/expected_value_and_error
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_value_and_error";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::expected<int, std::string> value{8};
    const std::expected<int, std::string> failure{std::unexpected{"offline"}};
    LEARN_EXPECT_EQ(checks, value.value(), 8);
    LEARN_EXPECT_EQ(checks, failure.error(), std::string{"offline"});
    LEARN_EXPECT_EQ(checks, failure.value_or(5), 5);
    LEARN_EXPECT_THROWS(checks, std::bad_expected_access<std::string>, failure.value());
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "std::expected observers");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_value_and_error", run>;

}  // namespace
