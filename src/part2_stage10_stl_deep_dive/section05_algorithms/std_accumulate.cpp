// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_accumulate
// Topic id : part2/stage10/section05/std_accumulate
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <numeric>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_accumulate";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3, 4};
    LEARN_EXPECT_EQ(checks, std::accumulate(values.begin(), values.end(), 0), 10);
    const std::string joined =
        std::accumulate(values.begin(), values.end(), std::string{},
                        [](std::string result, int value) { return std::move(result) + std::to_string(value); });
    LEARN_EXPECT_EQ(checks, joined, std::string{"1234"});
    const long long wide = std::accumulate(values.begin(), values.end(), 0LL);
    LEARN_EXPECT_EQ(checks, wide, 10LL);  // The initial value controls the accumulator type.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_accumulate", run>;

}  // namespace
