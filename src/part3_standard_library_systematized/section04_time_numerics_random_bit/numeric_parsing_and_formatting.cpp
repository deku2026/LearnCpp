// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_parsing_and_formatting
// Topic id : part3/section04/numeric_parsing_and_formatting
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <array>
#include <charconv>
#include <concepts>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section04/numeric_parsing_and_formatting";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int number{};
    constexpr std::string_view hex = "7f";
    const auto parsed = std::from_chars(hex.data(), hex.data() + hex.size(), number, 16);
    LEARN_EXPECT(checks, parsed.ec == std::errc{} && parsed.ptr == hex.data() + hex.size());
    LEARN_EXPECT_EQ(checks, number, 127);
    constexpr std::string_view partial = "12x";
    const auto partial_result = std::from_chars(partial.data(), partial.data() + partial.size(), number);
    LEARN_EXPECT(checks, partial_result.ec == std::errc{} && partial_result.ptr == partial.data() + 2);

    std::array<char, 32> buffer{};
    const auto formatted = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 255, 16);
    LEARN_EXPECT(checks, formatted.ec == std::errc{});
    LEARN_EXPECT_EQ(checks, (std::string_view{buffer.data(), formatted.ptr}), std::string_view{"ff"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/numeric_parsing_and_formatting", run>;

}  // namespace
