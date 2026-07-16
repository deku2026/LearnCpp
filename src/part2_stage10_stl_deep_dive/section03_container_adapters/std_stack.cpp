// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_stack
// Topic id : part2/stage10/section03/std_stack
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <stack>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/std_stack";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::stack<int, std::vector<int>> values;
    values.push(1);
    values.emplace(2);
    LEARN_EXPECT_EQ(checks, values.top(), 2);
    values.pop();
    LEARN_EXPECT_EQ(checks, values.top(), 1);
    LEARN_EXPECT_EQ(checks, values.size(), 1U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/std_stack", run>;

}  // namespace
