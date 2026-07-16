// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_variant_as_error
// Topic id : part2/stage09/section05/std_variant_as_error
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <charconv>
#include <string_view>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section05/std_variant_as_error";

enum class ParseError { empty, malformed };
using ParseResult = std::variant<int, ParseError>;

ParseResult parse(std::string_view text) {
    if (text.empty()) {
        return ParseError::empty;
    }
    int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size()) {
        return ParseError::malformed;
    }
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const ParseResult good = parse("12");
    const ParseResult bad = parse("x");
    LEARN_EXPECT_EQ(checks, std::get<int>(good), 12);
    LEARN_EXPECT_EQ(checks, std::get<ParseError>(bad), ParseError::malformed);
    const int score = std::visit(
        [](auto value) -> int {
            using T = decltype(value);
            if constexpr (std::same_as<T, int>) {
                return value;
            } else {
                return -1;
            }
        },
        good);
    LEARN_EXPECT_EQ(checks, score, 12);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section05/std_variant_as_error", run>;

}  // namespace
