// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_immediate_functions
// Topic id : part2/stage07/section03/consteval_immediate_functions
// References: N4950 [dcl.constexpr], [expr.const]; P1073R3.

#include "learn/example_support.hpp"

#include <array>
#include <limits>
#include <string_view>

namespace {

#if defined(__cpp_consteval) && __cpp_consteval >= 201811L

consteval unsigned parse_decimal(std::string_view text) {
    if (text.empty()) {
        throw "an integer requires at least one digit";
    }

    unsigned value{};
    for (const char character : text) {
        if (character < '0' || character > '9') {
            throw "non-decimal character";
        }
        const unsigned digit = static_cast<unsigned>(character - '0');
        if (value > (std::numeric_limits<unsigned>::max() - digit) / 10U) {
            throw "unsigned integer overflow";
        }
        value = value * 10U + digit;
    }
    return value;
}

static_assert(parse_decimal("0") == 0);
static_assert(parse_decimal("42") == 42);
static_assert(parse_decimal("12345") == 12345);

using ParsedExtent = std::array<int, parse_decimal("4")>;
static_assert(ParsedExtent{}.size() == 4);

#if 0
std::string_view runtime_text = "42";
unsigned rejected = parse_decimal(runtime_text);  // Immediate invocation is not constant.
constexpr unsigned malformed = parse_decimal("4x");
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_consteval) && __cpp_consteval >= 201811L
    ::learn::ExampleChecks checks{"part2/stage07/section03/consteval_immediate_functions"};

    constexpr auto answer = parse_decimal("42");
    LEARN_EXPECT_EQ(checks, answer, 42U);
    LEARN_EXPECT_EQ(checks, parse_decimal("007"), 7U);
    LEARN_EXPECT_EQ(checks, ParsedExtent{}.size(), 4uz);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section03/consteval_immediate_functions",
                                               "__cpp_consteval >= 201811L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/consteval_immediate_functions", run>;

}  // namespace
