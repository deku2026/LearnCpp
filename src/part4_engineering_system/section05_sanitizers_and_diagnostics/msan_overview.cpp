// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : msan_overview
// Topic id : part4/section05/msan_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace {

#if defined(__clang__)
#if __has_feature(memory_sanitizer)
constexpr bool memory_sanitizer_enabled = true;
#else
constexpr bool memory_sanitizer_enabled = false;
#endif
#else
constexpr bool memory_sanitizer_enabled = false;
#endif

struct Header {
    std::uint16_t kind{};
    std::uint16_t length{};
};

[[nodiscard]] constexpr std::optional<Header> parse_header(std::span<const std::uint8_t> bytes) noexcept {
    if (bytes.size() < 4) {
        return std::nullopt;
    }
    const auto kind =
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(bytes[0]) << 8U | static_cast<std::uint16_t>(bytes[1]));
    const auto length =
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(bytes[2]) << 8U | static_cast<std::uint16_t>(bytes[3]));
    return Header{kind, length};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/msan_overview"};

    constexpr std::array<std::uint8_t, 4> initialized_bytes{0x00U, 0x2AU, 0x01U, 0x00U};
    const auto header = parse_header(initialized_bytes);
    LEARN_EXPECT(checks, header.has_value());
    LEARN_EXPECT_EQ(checks, header->kind, 42U);
    LEARN_EXPECT_EQ(checks, header->length, 256U);
    LEARN_EXPECT(checks, !parse_header(std::span{initialized_bytes}.first(3)).has_value());

#if 0
    // A dedicated MSan binary diagnoses the read before initialization.
    int uninitialized;
    return uninitialized == 7;
#endif

    constexpr std::string_view flags = "-fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer";
    constexpr std::string_view requirement = "instrument the executable, C++ standard library, and every dependency";
    constexpr std::string_view instrumentation =
        memory_sanitizer_enabled ? "MSan instrumentation is active" : "MSan instrumentation is not active";
    LEARN_EXPECT(checks, flags.contains("track-origins"));
    LEARN_EXPECT(checks, requirement.contains("every dependency"));
    LEARN_EXPECT(checks, instrumentation.starts_with("MSan"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/msan_overview", run>;

}  // namespace
