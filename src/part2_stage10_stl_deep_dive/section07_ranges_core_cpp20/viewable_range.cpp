// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : viewable_range
// Topic id : part2/stage10/section07/viewable_range
//

#include "learn/example_support.hpp"

#include <initializer_list>
#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/viewable_range"};
    static_assert(std::ranges::viewable_range<std::vector<int>&>);
    static_assert(std::ranges::viewable_range<const std::vector<int>&>);
    static_assert(std::ranges::viewable_range<std::vector<int>>);
    static_assert(!std::ranges::viewable_range<const std::vector<int>>);
    static_assert(!std::ranges::viewable_range<std::initializer_list<int>>);

    std::vector source{1, 2, 3};
    auto referring_view = std::views::all(source);
    auto owning_view = std::views::all(std::vector{4, 5, 6});
    *referring_view.begin() = 10;

    int owned_sum = 0;
    for (const int value : owning_view) {
        owned_sum += value;
    }

    LEARN_EXPECT_EQ(checks, source.front(), 10);
    LEARN_EXPECT_EQ(checks, owned_sum, 15);

#if 0
    // A prvalue initializer_list does not own its backing array for a saved pipeline.
    auto unsafe = std::initializer_list<int>{1, 2, 3} | std::views::all;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/viewable_range", run>;

}  // namespace
