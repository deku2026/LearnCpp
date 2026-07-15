// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : viewable_range
// Topic id : part2/stage10/section07_ranges_core_cpp20/viewable_range
//
// Covers: viewable_range convertible to view

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::viewable_range<std::vector<int>&>);
    std::vector<int> v{1, 2, 3};
    auto r = std::views::all(v);
    assert(std::ranges::size(r) == 3);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    auto t = v | std::views::take(2);
    assert(std::ranges::distance(t) == 2);
}

void demo_expert() {
    static_assert(std::ranges::viewable_range<std::ranges::iota_view<int>>);
    auto v = std::views::iota(0, 10) | std::views::filter([](int x) { return x > 5; });
    assert(std::ranges::distance(v) == 4);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/viewable_range", run>;

}  // namespace
