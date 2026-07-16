// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_guarantee
// Topic id : part2/stage09/section03/no_guarantee
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section03/no_guarantee";

void unsafe_batch(std::vector<int>& values) {
    values.push_back(10);
    values.front() = -1;
    throw std::runtime_error{"second step failed"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2};
    LEARN_EXPECT_THROWS(checks, std::runtime_error, unsafe_batch(values));
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({-1, 2, 10}));
    // The object remains destructible here, but the operation promised no useful postcondition.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section03/no_guarantee", run>;

}  // namespace
