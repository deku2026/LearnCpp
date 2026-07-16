// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : to_chars_from_chars
// Topic id : part3/section01/to_chars_from_chars
// Reference: C++ working draft [charconv].

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

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/to_chars_from_chars";

#if LEARN_HAS_CHARCONV_HEADER && defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    learn::ExampleChecks checks{topic};
    std::array<char, 32> buffer{};
    const auto hexadecimal = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 255, 16);
    LEARN_EXPECT_EQ(checks, hexadecimal.ec, std::errc{});
    LEARN_EXPECT_EQ(checks, (std::string_view{buffer.data(), hexadecimal.ptr}), std::string_view{"ff"});

    constexpr std::string_view input{"42ms"};
    int value = 0;
    const auto parsed = std::from_chars(input.data(), input.data() + input.size(), value);
    LEARN_EXPECT_EQ(checks, parsed.ec, std::errc{});
    LEARN_EXPECT_EQ(checks, value, 42);
    LEARN_EXPECT_EQ(checks, parsed.ptr, input.data() + 2);

    int unchanged = 7;
    constexpr std::string_view spaced{" 42"};
    const auto whitespace = std::from_chars(spaced.data(), spaced.data() + spaced.size(), unchanged);
    LEARN_EXPECT_EQ(checks, whitespace.ec, std::errc::invalid_argument);
    LEARN_EXPECT_EQ(checks, unchanged, 7);

    std::array<char, 1> tiny{};
    const auto too_small = std::to_chars(tiny.data(), tiny.data() + tiny.size(), 123);
    LEARN_EXPECT_EQ(checks, too_small.ec, std::errc::value_too_large);

    double floating = 0.0;
    constexpr std::string_view decimal{"3.5"};
    const auto float_result = std::from_chars(decimal.data(), decimal.data() + decimal.size(), floating);
    LEARN_EXPECT_EQ(checks, float_result.ec, std::errc{});
    LEARN_EXPECT_EQ(checks, floating, 3.5);
    // No allocation, locale, or exception is involved; always inspect both ec and the returned pointer.
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_to_chars >= 201611L and <charconv>");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/to_chars_from_chars", run>;

}  // namespace
