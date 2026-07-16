// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : numeric_overview
// Topic id : part2/stage10/section05/numeric_overview
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <functional>
#include <numeric>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/numeric_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values(4);
    std::iota(values.begin(), values.end(), 1);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 2, 3, 4}));
    LEARN_EXPECT_EQ(checks, std::inner_product(values.begin(), values.end(), values.begin(), 0), 30);
    std::vector<int> prefix(values.size());
    std::partial_sum(values.begin(), values.end(), prefix.begin());
    LEARN_EXPECT_EQ(checks, prefix, std::vector<int>({1, 3, 6, 10}));
    LEARN_EXPECT_EQ(checks, std::adjacent_difference(prefix.begin(), prefix.end(), values.begin()), values.end());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/numeric_overview", run>;

}  // namespace
