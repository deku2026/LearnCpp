// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : expected_overview_cpp23
// Topic id : part3/section03/expected_overview_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <charconv>
#include <concepts>
#include <string_view>
#if __has_include(<expected>)
#include <expected>
#endif
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section03/expected_overview_cpp23";

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
enum class ParseError { empty, malformed };

std::expected<int, ParseError> parse(std::string_view text) {
    if (text.empty()) return std::unexpected{ParseError::empty};
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size()) return std::unexpected{ParseError::malformed};
    return value;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, parse("23").value(), 23);
    LEARN_EXPECT_EQ(checks, parse("").error(), ParseError::empty);
    LEARN_EXPECT_EQ(checks, parse("2x").error(), ParseError::malformed);
    LEARN_EXPECT_EQ(checks, parse("bad").value_or(7), 7);
    LEARN_EXPECT_THROWS(checks, std::bad_expected_access<ParseError>, parse("bad").value());
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::expected");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/expected_overview_cpp23", run>;

}  // namespace
