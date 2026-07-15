// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : common_range
// Topic id : part2/stage10/section07_ranges_core_cpp20/common_range
//
// Covers: common_range same iterator/sentinel types

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::common_range<std::vector<int>>);
    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end());
    LEARN_CHECK(v.front() == 1);
}

void demo_intermediate() {
    auto v = std::views::iota(0, 5);
    // iota may be common depending on implementation/bounds
    auto c = v | std::views::common;
    std::vector<int> out(c.begin(), c.end());
    LEARN_CHECK(out.size() == 5);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto t = v | std::views::take(3);
    auto c = t | std::views::common;
    static_assert(std::ranges::common_range<decltype(c)>);
    LEARN_CHECK(std::distance(c.begin(), c.end()) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/common_range", run>;

}  // namespace
