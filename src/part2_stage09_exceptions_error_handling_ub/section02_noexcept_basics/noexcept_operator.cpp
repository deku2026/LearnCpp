// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_operator
// Topic id : part2/stage09/section02/noexcept_operator
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section02/noexcept_operator";

int safe_value() noexcept {
    return 7;
}
int risky_value(bool fail) {
    if (fail) {
        throw std::runtime_error{"requested"};
    }
    return 9;
}

static_assert(noexcept(safe_value()));
static_assert(!noexcept(risky_value(false)));  // The operator is unevaluated; arguments are not run.

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, safe_value(), 7);
    LEARN_EXPECT_EQ(checks, risky_value(false), 9);
    LEARN_EXPECT_THROWS(checks, std::runtime_error, risky_value(true));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section02/noexcept_operator", run>;

}  // namespace
