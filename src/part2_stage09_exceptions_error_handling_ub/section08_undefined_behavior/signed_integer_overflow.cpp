// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : signed_integer_overflow
// Topic id : part2/stage09/section08/signed_integer_overflow
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <limits>
#include <optional>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/signed_integer_overflow";

std::optional<int> checked_add(int left, int right) {
    if ((right > 0 && left > std::numeric_limits<int>::max() - right) ||
        (right < 0 && left < std::numeric_limits<int>::min() - right)) {
        return std::nullopt;
    }
    return left + right;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, checked_add(20, 22).value(), 42);
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::max(), 1));
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::min(), -1));
#if 0
    const int undefined = std::numeric_limits<int>::max() + 1;
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/signed_integer_overflow", run>;

}  // namespace
