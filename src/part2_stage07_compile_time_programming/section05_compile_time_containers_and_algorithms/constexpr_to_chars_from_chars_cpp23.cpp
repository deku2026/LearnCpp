// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part2/stage07/section05/constexpr_to_chars_from_chars_cpp23
// References: N4950 [charconv]; P2291R3.

#include "learn/example_support.hpp"

#include <array>
#include <charconv>
#include <cstddef>
#include <string_view>
#include <system_error>

namespace {

#if defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L

struct ParsedInteger {
    int value;
    std::errc error;
    std::size_t consumed;
};

constexpr ParsedInteger parse_integer(std::string_view text, int base = 10) noexcept {
    int value = -1;
    const auto result = std::from_chars(text.data(), text.data() + text.size(), value, base);
    return ParsedInteger{value, result.ec, static_cast<std::size_t>(result.ptr - text.data())};
}

struct FormattedInteger {
    std::array<char, 32> buffer{};
    std::size_t size{};
    std::errc error{};
};

constexpr FormattedInteger format_integer(int value, int base = 10) noexcept {
    FormattedInteger output{};
    const auto result = std::to_chars(output.buffer.data(), output.buffer.data() + output.buffer.size(), value, base);
    output.size = static_cast<std::size_t>(result.ptr - output.buffer.data());
    output.error = result.ec;
    return output;
}

inline constexpr auto decimal = parse_integer("42ms");
inline constexpr auto hexadecimal = parse_integer("2a", 16);
inline constexpr auto invalid = parse_integer("x42");
inline constexpr auto overflow = parse_integer("999999999999999999999999");
inline constexpr auto formatted_hexadecimal = format_integer(-42, 16);

static_assert(decimal.value == 42 && decimal.consumed == 2 && decimal.error == std::errc{});
static_assert(hexadecimal.value == 42 && hexadecimal.consumed == 2);
static_assert(invalid.error == std::errc::invalid_argument && invalid.consumed == 0);
static_assert(overflow.error == std::errc::result_out_of_range);
static_assert(std::string_view{formatted_hexadecimal.buffer.data(), formatted_hexadecimal.size} == "-2a");

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L
    ::learn::ExampleChecks checks{"part2/stage07/section05/constexpr_to_chars_from_chars_cpp23"};

    LEARN_EXPECT_EQ(checks, decimal.value, 42);
    LEARN_EXPECT_EQ(checks, decimal.consumed, 2uz);
    LEARN_EXPECT_EQ(checks, hexadecimal.value, 42);
    LEARN_EXPECT_EQ(checks, invalid.error, std::errc::invalid_argument);
    LEARN_EXPECT_EQ(checks, overflow.error, std::errc::result_out_of_range);
    LEARN_EXPECT_EQ(checks, (std::string_view{formatted_hexadecimal.buffer.data(), formatted_hexadecimal.size}),
                    std::string_view{"-2a"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/constexpr_to_chars_from_chars_cpp23",
                                               "__cpp_lib_constexpr_charconv >= 202207L (integral overloads)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
