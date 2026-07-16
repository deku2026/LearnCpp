// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : text_headers
// Topic id : part3/section08/text_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <charconv>
#include <regex>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section08/text_headers";

constexpr std::array<std::string_view, 11> headers{
    "charconv", "cstring", "cuchar",      "cwchar",        "cwctype", "format",
    "regex",    "string",  "string_view", "text_encoding", "version",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string text = "item-23";
    const std::regex pattern{R"(item-([0-9]+))"};
    std::smatch match;
    LEARN_EXPECT(checks, std::regex_match(text, match, pattern));
    int value{};
    const std::string digits = match[1].str();
    const auto parsed = std::from_chars(digits.data(), digits.data() + digits.size(), value);
    LEARN_EXPECT(checks, parsed.ec == std::errc{});
    LEARN_EXPECT_EQ(checks, value, 23);
    LEARN_EXPECT_EQ(checks, headers.size(), 11U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/text_headers", run>;

}  // namespace
