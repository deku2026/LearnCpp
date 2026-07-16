// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_reverse
// Topic id : part2/stage10/section07/views_reverse
//

#include "learn/example_support.hpp"

#include <forward_list>
#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/views_reverse"};
    std::vector values{1, 2, 3, 4};
    auto reversed = values | std::views::reverse;
    static_assert(std::ranges::random_access_range<decltype(reversed)>);
    static_assert(std::ranges::sized_range<decltype(reversed)>);
    static_assert(!std::ranges::bidirectional_range<std::forward_list<int>&>);

    std::vector<int> observed;
    for (const int value : reversed) {
        observed.push_back(value);
    }
    *reversed.begin() = 40;

    std::vector<int> restored_order;
    for (const int value : reversed | std::views::reverse) {
        restored_order.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, observed, (std::vector{4, 3, 2, 1}));
    LEARN_EXPECT_EQ(checks, values.back(), 40);
    LEARN_EXPECT_EQ(checks, restored_order, (std::vector{1, 2, 3, 40}));

#if 0
    std::forward_list<int> only_forward{1, 2, 3};
    auto invalid = only_forward | std::views::reverse;  // Requires bidirectional_range.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_reverse", run>;

}  // namespace
