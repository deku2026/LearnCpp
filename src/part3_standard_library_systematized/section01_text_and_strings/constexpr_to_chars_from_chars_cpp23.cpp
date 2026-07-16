// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part3/section01/constexpr_to_chars_from_chars_cpp23
// Reference: WG21 P2291R3; constexpr applies to the integral charconv overloads.

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<charconv>)
#include <charconv>
#define LEARN_HAS_CHARCONV_HEADER 1
#else
#define LEARN_HAS_CHARCONV_HEADER 0
#endif

#include <array>
#include <string_view>
#include <system_error>

namespace {

#if LEARN_HAS_CHARCONV_HEADER && defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L
struct parsed_integer {
    int value;
    std::errc error;
    std::size_t consumed;
};

constexpr parsed_integer parse(std::string_view text, int base = 10) {
    int value = -1;
    const auto result = std::from_chars(text.data(), text.data() + text.size(), value, base);
    return {value, result.ec, static_cast<std::size_t>(result.ptr - text.data())};
}

struct formatted_integer {
    std::array<char, 32> storage{};
    std::size_t size{};
    std::errc error{};
};

constexpr formatted_integer format(int value, int base = 10) {
    formatted_integer output{};
    const auto result =
        std::to_chars(output.storage.data(), output.storage.data() + output.storage.size(), value, base);
    output.size = static_cast<std::size_t>(result.ptr - output.storage.data());
    output.error = result.ec;
    return output;
}

constexpr auto parsed = parse("2a-tail", 16);
constexpr auto invalid = parse(" 42");
constexpr auto overflow = parse("999999999999999999999999999");
constexpr auto formatted = format(-42, 16);
static_assert(parsed.value == 42 && parsed.consumed == 2 && parsed.error == std::errc{});
static_assert(invalid.error == std::errc::invalid_argument && invalid.consumed == 0);
static_assert(overflow.error == std::errc::result_out_of_range);
static_assert(std::string_view{formatted.storage.data(), formatted.size} == "-2a");
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/constexpr_to_chars_from_chars_cpp23";

#if LEARN_HAS_CHARCONV_HEADER && defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L
    learn::ExampleChecks checks{topic};
    LEARN_EXPECT_EQ(checks, parsed.value, 42);
    LEARN_EXPECT_EQ(checks, parsed.consumed, std::size_t{2});
    LEARN_EXPECT_EQ(checks, invalid.error, std::errc::invalid_argument);
    LEARN_EXPECT_EQ(checks, overflow.error, std::errc::result_out_of_range);
    LEARN_EXPECT_EQ(checks, (std::string_view{formatted.storage.data(), formatted.size}), std::string_view{"-2a"});
    // charconv does not skip whitespace, allocate, consult a locale, or throw conversion exceptions.
    return checks.result();
#else
    learn::ExampleChecks checks{topic};
    int modeled = 0;
#if LEARN_HAS_CHARCONV_HEADER
    const std::string_view text{"42x"};
    const auto result = std::from_chars(text.data(), text.data() + text.size(), modeled);
    LEARN_EXPECT_EQ(checks, modeled, 42);
    LEARN_EXPECT_EQ(checks, result.ptr, text.data() + 2);
#else
    modeled = 42;
    LEARN_EXPECT_EQ(checks, modeled, 42);
#endif
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_constexpr_charconv >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
