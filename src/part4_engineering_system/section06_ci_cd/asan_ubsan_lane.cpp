// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : asan_ubsan_lane
// Topic id : part4/section06/asan_ubsan_lane
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section06/asan_ubsan_lane";

constexpr bool asan_enabled =
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
    true;
#else
    false;
#endif
#elif defined(__SANITIZE_ADDRESS__)
    true;
#else
    false;
#endif

int checked_sum(std::span<const int> values) {
    int sum{};
    for (const int value : values) sum += value;
    return sum;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::array values{1, 2, 3};
    LEARN_EXPECT_EQ(checks, checked_sum(values), 6);
    const std::array<std::string_view, 4> required_steps{"compile with instrumentation", "link runtime", "run tests",
                                                         "archive reports"};
    LEARN_EXPECT_EQ(checks, required_steps.size(), 4U);
    if (asan_enabled) LEARN_EXPECT(checks, asan_enabled);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/asan_ubsan_lane", run>;

}  // namespace
