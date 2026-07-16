// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_priority_queue
// Topic id : part2/stage10/section03/std_priority_queue
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <functional>
#include <queue>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/std_priority_queue";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::priority_queue<int> maximum_first;
    for (const int value : {2, 5, 1}) {
        maximum_first.push(value);
    }
    LEARN_EXPECT_EQ(checks, maximum_first.top(), 5);
    maximum_first.pop();
    LEARN_EXPECT_EQ(checks, maximum_first.top(), 2);

    std::priority_queue<int, std::vector<int>, std::greater<>> minimum_first;
    minimum_first.push(4);
    minimum_first.push(1);
    LEARN_EXPECT_EQ(checks, minimum_first.top(), 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/std_priority_queue", run>;

}  // namespace
