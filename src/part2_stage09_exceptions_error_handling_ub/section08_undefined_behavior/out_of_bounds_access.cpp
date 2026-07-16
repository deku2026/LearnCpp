// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : out_of_bounds_access
// Topic id : part2/stage09/section08/out_of_bounds_access
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <array>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/out_of_bounds_access";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{3, 4, 5};
    LEARN_EXPECT_EQ(checks, values.at(2), 5);
    LEARN_EXPECT_THROWS(checks, std::out_of_range, values.at(3));
    const std::span view{values};
    LEARN_EXPECT_EQ(checks, view.first<2>().back(), 4);
#if 0
    const int undefined = values[values.size()];
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/out_of_bounds_access", run>;

}  // namespace
