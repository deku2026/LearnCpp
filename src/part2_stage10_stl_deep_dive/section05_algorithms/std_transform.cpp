// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_transform
// Topic id : part2/stage10/section05/std_transform
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <iterator>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_transform";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3};
    std::vector<int> squares;
    std::ranges::transform(values, std::back_inserter(squares), [](int value) { return value * value; });
    LEARN_EXPECT_EQ(checks, squares, std::vector<int>({1, 4, 9}));

    std::vector<int> sums(values.size());
    std::transform(values.begin(), values.end(), squares.begin(), sums.begin(), std::plus<>{});
    LEARN_EXPECT_EQ(checks, sums, std::vector<int>({2, 6, 12}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_transform", run>;

}  // namespace
