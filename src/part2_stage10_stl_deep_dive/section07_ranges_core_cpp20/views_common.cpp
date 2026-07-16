// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_common
// Topic id : part2/stage10/section07/views_common
//

#include "learn/example_support.hpp"

#include <concepts>
#include <numeric>
#include <ranges>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/views_common"};
    auto non_common = std::views::iota(1) | std::views::take_while([](const int value) { return value < 5; });
    static_assert(!std::ranges::common_range<decltype(non_common)>);
    static_assert(
        !std::same_as<std::ranges::iterator_t<decltype(non_common)>, std::ranges::sentinel_t<decltype(non_common)>>);

    auto common = non_common | std::views::common;
    static_assert(std::ranges::common_range<decltype(common)>);
    static_assert(std::same_as<std::ranges::iterator_t<decltype(common)>, std::ranges::sentinel_t<decltype(common)>>);

    const int sum = std::accumulate(common.begin(), common.end(), 0);
    LEARN_EXPECT_EQ(checks, sum, 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_common", run>;

}  // namespace
