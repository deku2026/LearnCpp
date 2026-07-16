// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : algorithm_overview
// Topic id : part2/stage10/section05/algorithm_overview
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/algorithm_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{4, 1, 3, 1, 2};
    LEARN_EXPECT_EQ(checks, std::ranges::count(values, 1), 2);
    const auto found = std::ranges::find(values, 3);
    LEARN_EXPECT(checks, found != values.end());
    std::ranges::sort(values);
    LEARN_EXPECT(checks, std::ranges::is_sorted(values));
    values.erase(std::ranges::unique(values).begin(), values.end());
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 2, 3, 4}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/algorithm_overview", run>;

}  // namespace
