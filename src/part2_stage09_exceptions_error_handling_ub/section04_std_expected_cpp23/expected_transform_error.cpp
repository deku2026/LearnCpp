// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform_error
// Topic id : part2/stage09/section04/expected_transform_error
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/expected_transform_error";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
enum class ParseError { empty, malformed };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::expected<int, ParseError> failure{std::unexpected{ParseError::malformed}};
    const auto described = failure.transform_error(
        [](ParseError error) { return error == ParseError::empty ? std::string{"empty"} : std::string{"malformed"}; });
    LEARN_EXPECT_EQ(checks, described.error(), std::string{"malformed"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "expected::transform_error");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_transform_error", run>;

}  // namespace
