// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : byte_versus_char
// Topic id : part3/section01/byte_versus_char
// Reference: C++ working draft [cstddef.syn], [basic.fundamental], [span.objectrep].

#include "learn/example_support.hpp"

#if __has_include(<cstddef>)
#include <cstddef>
#define LEARN_HAS_BYTE 1
#else
#define LEARN_HAS_BYTE 0
#endif

#include <array>
#include <bit>
#include <cstdint>
#include <span>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/byte_versus_char";

#if LEARN_HAS_BYTE
    learn::ExampleChecks checks{topic};
    static_assert(sizeof(char) == 1);
    static_assert(std::is_enum_v<std::byte>);
    static_assert(!std::is_convertible_v<std::byte, int>);

    const std::byte permissions = std::byte{0b1010'0001};
    const std::byte low_nibble = permissions & std::byte{0x0F};
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>(low_nibble), 1U);
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>(permissions >> 4), 10U);

    const std::uint32_t value = 0x1234ABCDU;
    const auto representation = std::bit_cast<std::array<std::byte, sizeof(value)>>(value);
    LEARN_EXPECT_EQ(checks, std::bit_cast<std::uint32_t>(representation), value);

    std::array<char, 3> text{'A', 'B', 'C'};
    const std::span text_view{text};
    const auto bytes = std::as_bytes(text_view);
    LEARN_EXPECT_EQ(checks, bytes.size(), text_view.size_bytes());
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned char>(bytes.front()), static_cast<unsigned char>('A'));

    // char participates in text APIs and integer promotion; std::byte expresses raw object representation.
    // Byte order in `representation` is deliberately not asserted: it follows the native endianness.
#if 0
    const auto ill_formed = std::byte{1} + std::byte{2};  // std::byte has bitwise, not arithmetic, operations.
#endif
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "standard <cstddef> std::byte support");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/byte_versus_char", run>;

}  // namespace
