// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : optional_overview
// Topic id : part3/section03/optional_overview
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <optional>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section03/optional_overview";

std::optional<int> parse_digit(char value) {
    if (value < '0' || value > '9') return std::nullopt;
    return value - '0';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::optional<std::string> text{std::in_place, 3, 'a'};
    LEARN_EXPECT_EQ(checks, *text, std::string{"aaa"});
    text.reset();
    LEARN_EXPECT_EQ(checks, text.value_or("fallback"), std::string{"fallback"});
    LEARN_EXPECT_EQ(checks, parse_digit('7').value(), 7);
    LEARN_EXPECT(checks, !parse_digit('x'));
    LEARN_EXPECT_THROWS(checks, std::bad_optional_access, parse_digit('x').value());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section03/optional_overview", run>;

}  // namespace
