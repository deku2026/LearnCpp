// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_reduce
// Topic id : part2/stage10/section05/std_reduce
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <functional>
#include <numeric>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_reduce";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3, 4};
    LEARN_EXPECT_EQ(checks, std::reduce(values.begin(), values.end(), 0), 10);
    LEARN_EXPECT_EQ(checks, std::reduce(values.begin(), values.end(), 1, std::multiplies<>{}), 24);
    // reduce may regroup operations; use associative operations when order independence matters.
    const int left_fold = std::accumulate(values.begin(), values.end(), 0, std::minus<>{});
    LEARN_EXPECT_EQ(checks, left_fold, -10);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_reduce", run>;

}  // namespace
