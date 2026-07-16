// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : assume_unreachable_going_wrong
// Topic id : part2/stage09/section08/assume_unreachable_going_wrong
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <optional>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/assume_unreachable_going_wrong";

std::optional<int> checked_element(std::span<const int> values, std::size_t index) {
    if (index >= values.size()) {
        return std::nullopt;
    }
    return values[index];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const int values[]{4, 5};
    LEARN_EXPECT_EQ(checks, checked_element(values, 1).value(), 5);
    LEARN_EXPECT(checks, !checked_element(values, 2));
#if 0
    // A false [[assume(condition)]] or reaching std::unreachable() is undefined, not a branch hint.
    [[assume(index < values.size())]];
    return values[index];
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/assume_unreachable_going_wrong", run>;

}  // namespace
