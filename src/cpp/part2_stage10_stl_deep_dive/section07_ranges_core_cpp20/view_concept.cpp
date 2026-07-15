// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : view_concept
// Topic id : part2/stage10/section07_ranges_core_cpp20/view_concept
//
// Covers: std::ranges::view lightweight range

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::view<std::ranges::iota_view<int>>);
    auto v = std::views::iota(0, 3);
    assert(std::ranges::distance(v) == 3);
}

void demo_intermediate() {
    std::vector<int> vec{1, 2, 3, 4};
    auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
    static_assert(std::ranges::view<decltype(evens)>);
    int sum = 0;
    for (int x : evens) {
        sum += x;
    }
    assert(sum == 6);
}

void demo_expert() {
    // views are cheap to copy/move; they don't own (usually)
    auto v = std::views::iota(1, 5);
    auto v2 = v;
    assert(std::ranges::equal(v, v2));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/view_concept", run>;

}  // namespace
