// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_basics
// Topic id : part2/stage09/section04/expected_basics
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

constexpr std::string_view kTopic = "part2/stage09/section04/expected_basics";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
std::expected<int, std::string> parse_number(std::string_view text) {
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size()) {
        return std::unexpected{"not an integer"};
    }
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto good = parse_number("42");
    const auto bad = parse_number("4x");
    LEARN_EXPECT(checks, good.has_value());
    LEARN_EXPECT_EQ(checks, *good, 42);
    LEARN_EXPECT(checks, !bad.has_value());
    LEARN_EXPECT_EQ(checks, bad.error(), std::string{"not an integer"});
    return checks.result();
}
#else
int run(int, char**) {
    return ::learn::ExampleChecks::unavailable(kTopic, "std::expected");
}
#endif

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section04/expected_basics", run>;

}  // namespace
