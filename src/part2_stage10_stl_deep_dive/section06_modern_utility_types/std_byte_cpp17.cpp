// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_byte_cpp17
// Topic id : part2/stage10/section06/std_byte_cpp17

#ifdef __has_include
#if __has_include(<cstddef>)
#include <cstddef>
#define LEARN_HAS_CSTDDEF_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <array>
#include <cstdint>
#include <type_traits>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_byte_cpp17";

#if defined(LEARN_HAS_CSTDDEF_HEADER) && defined(__cpp_lib_byte) && __cpp_lib_byte >= 201603L

[[nodiscard]] constexpr std::array<std::byte, 2> encode_big_endian(std::uint16_t value) noexcept {
    return {std::byte{static_cast<unsigned char>(value >> 8U)}, std::byte{static_cast<unsigned char>(value & 0xFFU)}};
}

[[nodiscard]] constexpr std::uint16_t decode_big_endian(const std::array<std::byte, 2>& bytes) noexcept {
    const auto high = static_cast<std::uint16_t>(std::to_integer<unsigned int>(bytes[0]));
    const auto low = static_cast<std::uint16_t>(std::to_integer<unsigned int>(bytes[1]));
    return static_cast<std::uint16_t>((high << 8U) | low);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_CSTDDEF_HEADER) && defined(__cpp_lib_byte) && __cpp_lib_byte >= 201603L
    learn::ExampleChecks checks{topic_id};

    constexpr auto encoded = encode_big_endian(0xCAFEU);
    static_assert(std::to_integer<unsigned int>(encoded[0]) == 0xCAU);
    static_assert(decode_big_endian(encoded) == 0xCAFEU);
    LEARN_EXPECT_EQ(checks, decode_big_endian(encoded), 0xCAFEU);

    std::byte flags{0b0000'0011};
    flags |= std::byte{0b0000'1000};
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>(flags), 0b0000'1011U);
    flags &= std::byte{0b0000'1001};
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>(flags), 0b0000'1001U);
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>(std::byte{1} << 7U), 128U);

    static_assert(std::is_enum_v<std::byte>);
    static_assert(!std::is_arithmetic_v<std::byte>);  // bytes support bits, not numeric arithmetic.
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 __cpp_lib_byte >= 201603L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_byte_cpp17", run>;

}  // namespace
