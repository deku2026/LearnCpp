// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : sanitizer_protection
// Topic id : part2/stage09/section08/sanitizer_protection
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <array>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/sanitizer_protection";

constexpr bool address_sanitizer_enabled =
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

int sum(std::span<const int> values) {
    int result{};
    for (const int value : values) {
        result += value;
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::array values{1, 2, 3};
    LEARN_EXPECT_EQ(checks, sum(values), 6);
    LEARN_EXPECT(checks, address_sanitizer_enabled || !address_sanitizer_enabled);
    // Sanitizers diagnose many bugs but do not define invalid operations or prove their absence.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/sanitizer_protection", run>;

}  // namespace
