// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : ubsan_overview
// Topic id : part4/section05/ubsan_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>

namespace {

#if defined(__clang__)
#if __has_feature(undefined_behavior_sanitizer)
constexpr bool undefined_sanitizer_enabled = true;
#else
constexpr bool undefined_sanitizer_enabled = false;
#endif
#elif defined(__SANITIZE_UNDEFINED__)
constexpr bool undefined_sanitizer_enabled = true;
#else
constexpr bool undefined_sanitizer_enabled = false;
#endif

[[nodiscard]] constexpr std::optional<int> checked_add(int left, int right) noexcept {
    constexpr int maximum = std::numeric_limits<int>::max();
    constexpr int minimum = std::numeric_limits<int>::min();
    if ((right > 0 && left > maximum - right) || (right < 0 && left < minimum - right)) {
        return std::nullopt;
    }
    return left + right;
}

[[nodiscard]] constexpr std::optional<std::uint32_t> checked_left_shift(std::uint32_t value,
                                                                        unsigned int count) noexcept {
    constexpr unsigned int digits = std::numeric_limits<std::uint32_t>::digits;
    if (count >= digits || value > (std::numeric_limits<std::uint32_t>::max() >> count)) {
        return std::nullopt;
    }
    return value << count;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/ubsan_overview"};

    LEARN_EXPECT_EQ(checks, checked_add(40, 2), std::optional<int>{42});
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::max(), 1).has_value());
    LEARN_EXPECT_EQ(checks, checked_left_shift(3U, 4U), std::optional<std::uint32_t>{48U});
    LEARN_EXPECT(checks, !checked_left_shift(1U, 32U).has_value());

#if 0
    // Each statement belongs in a subprocess expected to fail under UBSan.
    const int overflow = std::numeric_limits<int>::max() + 1;
    const int invalid_shift = 1 << 32;
    const int null_read = *static_cast<int*>(nullptr);
#endif

    constexpr std::string_view flags = "-fsanitize=undefined -fno-sanitize-recover=undefined -g";
    constexpr std::string_view instrumentation =
        undefined_sanitizer_enabled ? "UBSan instrumentation is active" : "UBSan instrumentation is not active";
    LEARN_EXPECT(checks, flags.contains("no-sanitize-recover"));
    LEARN_EXPECT(checks, instrumentation.starts_with("UBSan"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/ubsan_overview", run>;

}  // namespace
