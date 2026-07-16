// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : common_range
// Topic id : part2/stage10/section07/common_range
//

#include "learn/example_support.hpp"

#include <numeric>
#include <ranges>
#include <vector>

namespace {

template <std::ranges::common_range Range>
[[nodiscard]] int legacy_sum(Range&& range) {
    return std::accumulate(std::ranges::begin(range), std::ranges::end(range), 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/common_range"};
    std::vector values{1, 2, 3, 4};
    static_assert(std::ranges::common_range<decltype(values)&>);

    auto stops_at_four = std::views::iota(1) | std::views::take_while([](const int value) { return value < 4; });
    static_assert(std::ranges::range<decltype(stops_at_four)>);
    static_assert(!std::ranges::common_range<decltype(stops_at_four)>);

    std::vector<int> observed;
    for (const int value : stops_at_four) {
        observed.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, legacy_sum(values), 10);
    LEARN_EXPECT_EQ(checks, observed, (std::vector{1, 2, 3}));

#if 0
    // Classic iterator-pair algorithms cannot deduce one iterator type here.
    const int rejected = legacy_sum(stops_at_four);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/common_range", run>;

}  // namespace
