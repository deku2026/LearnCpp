// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_sample
// Topic id : part2/stage10/section05/std_sample
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <iterator>
#include <random>
#include <set>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_sample";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> population{1, 2, 3, 4, 5, 6};
    std::vector<int> sample;
    std::mt19937 engine{12345U};
    std::sample(population.begin(), population.end(), std::back_inserter(sample), 3, engine);
    LEARN_EXPECT_EQ(checks, sample.size(), 3U);
    const std::set<int> population_set(population.begin(), population.end());
    LEARN_EXPECT(checks, std::ranges::all_of(sample, [&](int value) { return population_set.contains(value); }));
    LEARN_EXPECT_EQ(checks, std::set<int>(sample.begin(), sample.end()).size(), sample.size());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_sample", run>;

}  // namespace
