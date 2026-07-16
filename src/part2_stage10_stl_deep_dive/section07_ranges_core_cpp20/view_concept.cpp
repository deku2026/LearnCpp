// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : view_concept
// Topic id : part2/stage10/section07/view_concept
//

#include "learn/example_support.hpp"

#include <ranges>
#include <span>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/view_concept"};
    static_assert(std::ranges::range<std::vector<int>>);
    static_assert(!std::ranges::view<std::vector<int>>);
    static_assert(std::ranges::view<std::span<int>>);
    static_assert(std::ranges::view<decltype(std::views::iota(0, 3))>);

    std::vector values{1, 2, 3};
    auto referenced = std::views::all(values);
    auto owned = std::views::all(std::vector{4, 5, 6});
    static_assert(std::ranges::view<decltype(referenced)>);
    static_assert(std::ranges::view<decltype(owned)>);

    *referenced.begin() = 10;
    int owned_sum = 0;
    for (const int value : owned) {
        owned_sum += value;
    }

    LEARN_EXPECT_EQ(checks, values.front(), 10);
    LEARN_EXPECT_EQ(checks, owned_sum, 15);
    // A view may reference external storage or own a movable range; `view` describes
    // cheap range semantics, not a universal non-owning guarantee.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/view_concept", run>;

}  // namespace
