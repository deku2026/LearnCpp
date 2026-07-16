// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : railway_oriented_pipeline
// Topic id : part2/stage09/section04/railway_oriented_pipeline
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <charconv>
#include <string_view>
#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section04/railway_oriented_pipeline";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
using Result = std::expected<int, std::string>;

Result parse(std::string_view text) {
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size()) {
        return std::unexpected{"parse"};
    }
    return value;
}

Result require_positive(int value) {
    return value > 0 ? Result{value} : Result{std::unexpected{"non-positive"}};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto good = parse("7").and_then(require_positive).transform([](int value) { return value * 3; });
    const auto parse_failure = parse("x").and_then(require_positive);
    const auto domain_failure = parse("-2").and_then(require_positive);
    LEARN_EXPECT_EQ(checks, good.value(), 21);
    LEARN_EXPECT_EQ(checks, parse_failure.error(), std::string{"parse"});
    LEARN_EXPECT_EQ(checks, domain_failure.error(), std::string{"non-positive"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "expected monadic pipeline");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/railway_oriented_pipeline", run>;

}  // namespace
