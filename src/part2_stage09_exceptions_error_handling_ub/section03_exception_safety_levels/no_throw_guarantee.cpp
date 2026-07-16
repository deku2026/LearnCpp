// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_throw_guarantee
// Topic id : part2/stage09/section03/no_throw_guarantee
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section03/no_throw_guarantee";

struct Counter {
    void reset() noexcept { value = 0; }
    void swap(Counter& other) noexcept { std::swap(value, other.value); }
    int value{};
};

static_assert(std::is_nothrow_invocable_v<decltype(&Counter::reset), Counter*>);
static_assert(noexcept(std::declval<Counter&>().swap(std::declval<Counter&>())));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Counter first{3};
    Counter second{8};
    first.swap(second);
    first.reset();
    LEARN_EXPECT_EQ(checks, first.value, 0);
    LEARN_EXPECT_EQ(checks, second.value, 3);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section03/no_throw_guarantee", run>;

}  // namespace
