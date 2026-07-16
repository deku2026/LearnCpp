// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : strong_guarantee
// Topic id : part2/stage09/section03/strong_guarantee
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section03/strong_guarantee";

void replace_values(std::vector<int>& target, bool fail) {
    std::vector<int> candidate{7, 8, 9};
    if (fail) {
        throw std::runtime_error{"validation failed"};
    }
    target.swap(candidate);  // Commit is non-throwing after all risky work succeeds.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2};
    LEARN_EXPECT_THROWS(checks, std::runtime_error, replace_values(values, true));
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 2}));
    replace_values(values, false);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({7, 8, 9}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section03/strong_guarantee", run>;

}  // namespace
