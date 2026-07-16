// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : asan_overview
// Topic id : part4/section05/asan_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

#if defined(__clang__)
#if __has_feature(address_sanitizer)
constexpr bool address_sanitizer_enabled = true;
#else
constexpr bool address_sanitizer_enabled = false;
#endif
#elif defined(__SANITIZE_ADDRESS__)
constexpr bool address_sanitizer_enabled = true;
#else
constexpr bool address_sanitizer_enabled = false;
#endif

[[nodiscard]] std::optional<int> checked_lookup(std::span<const int> values, std::size_t index) noexcept {
    if (index >= values.size()) {
        return std::nullopt;
    }
    return values[index];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/asan_overview"};

    const std::vector values{10, 20, 30};
    LEARN_EXPECT_EQ(checks, checked_lookup(values, 1), std::optional<int>{20});
    LEARN_EXPECT(checks, !checked_lookup(values, values.size()).has_value());
    LEARN_EXPECT_THROWS(checks, std::out_of_range, values.at(values.size()));

    // RAII prevents the use-after-free and double-delete families ASan diagnoses.
    auto owner = std::make_unique<int>(42);
    const int copied_before_release = *owner;
    owner.reset();
    LEARN_EXPECT_EQ(checks, copied_before_release, 42);
    LEARN_EXPECT(checks, owner == nullptr);

#if 0
    // Compile this only in a dedicated failing sanitizer test, never in the normal sweep.
    int* dangling = new int{7};
    delete dangling;
    return *dangling;  // ASan reports heap-use-after-free.
#endif

    constexpr std::string_view windows = "/fsanitize=address /Zi";
    constexpr std::string_view unix_like = "-fsanitize=address -g -O1 -fno-omit-frame-pointer";
    constexpr std::string_view instrumentation =
        address_sanitizer_enabled ? "ASan instrumentation is active" : "ASan instrumentation is not active";
    LEARN_EXPECT(checks, windows.contains("address"));
    LEARN_EXPECT(checks, unix_like.contains("frame-pointer"));
    LEARN_EXPECT(checks, instrumentation.starts_with("ASan"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/asan_overview", run>;

}  // namespace
