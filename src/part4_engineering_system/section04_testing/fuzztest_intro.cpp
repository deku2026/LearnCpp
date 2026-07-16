// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : fuzztest_intro
// Topic id : part4/section04/fuzztest_intro

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr char hex_digits[] = "0123456789abcdef";

[[nodiscard]] std::string hex_encode(const std::vector<std::uint8_t>& input) {
    std::string output;
    output.reserve(input.size() * 2);
    for (const std::uint8_t byte : input) {
        output.push_back(hex_digits[byte >> 4U]);
        output.push_back(hex_digits[byte & 0x0FU]);
    }
    return output;
}

[[nodiscard]] constexpr std::optional<std::uint8_t> hex_value(char character) noexcept {
    if (character >= '0' && character <= '9') {
        return static_cast<std::uint8_t>(character - '0');
    }
    if (character >= 'a' && character <= 'f') {
        return static_cast<std::uint8_t>(character - 'a' + 10);
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<std::vector<std::uint8_t>> hex_decode(std::string_view input) {
    if (input.size() % 2 != 0) {
        return std::nullopt;
    }
    std::vector<std::uint8_t> output;
    output.reserve(input.size() / 2);
    for (std::size_t index = 0; index < input.size(); index += 2) {
        const auto high = hex_value(input[index]);
        const auto low = hex_value(input[index + 1]);
        if (!high || !low) {
            return std::nullopt;
        }
        output.push_back(static_cast<std::uint8_t>((*high << 4U) | *low));
    }
    return output;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/fuzztest_intro"};

    // FuzzTest would express this as a property over a bounded vector domain.
    for (unsigned int value = 0; value <= 0xFFU; ++value) {
        const std::vector input{static_cast<std::uint8_t>(value)};
        const auto decoded = hex_decode(hex_encode(input));
        LEARN_EXPECT(checks, decoded.has_value());
        LEARN_EXPECT_EQ(checks, *decoded, input);
    }
    const std::vector<std::uint8_t> boundary{0x00U, 0x0FU, 0x10U, 0x7FU, 0x80U, 0xFFU};
    LEARN_EXPECT_EQ(checks, *hex_decode(hex_encode(boundary)), boundary);
    LEARN_EXPECT(checks, !hex_decode("0").has_value());
    LEARN_EXPECT(checks, !hex_decode("zz").has_value());

    constexpr std::string_view property =
        "FUZZ_TEST(Hex, RoundTrips).WithDomains(fuzztest::VectorOf(...).WithMaxSize(32))";
    LEARN_EXPECT(checks, property.contains("WithMaxSize"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/fuzztest_intro", run>;

}  // namespace
