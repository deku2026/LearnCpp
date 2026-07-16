// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : uninitialized_read
// Topic id : part2/stage09/section08/uninitialized_read
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <optional>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/uninitialized_read";

struct Reading {
    int value{};  // A default member initializer makes default construction deterministic.
    bool valid{};
};

static_assert(std::is_aggregate_v<Reading>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Reading initialized{};
    const std::optional<int> not_measured;
    LEARN_EXPECT_EQ(checks, initialized.value, 0);
    LEARN_EXPECT(checks, !initialized.valid);
    LEARN_EXPECT(checks, !not_measured);
#if 0
    int indeterminate;
    const int undefined = indeterminate;
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/uninitialized_read", run>;

}  // namespace
