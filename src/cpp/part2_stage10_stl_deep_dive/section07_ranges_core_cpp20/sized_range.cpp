// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : sized_range
// Topic id : part2/stage10/section07_ranges_core_cpp20/sized_range
//
// Covers: sized_range with constant-time size

#include "learn/topic_registry.hpp"

#include <cassert>
#include <list>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::sized_range<std::vector<int>>);
    std::vector<int> v{1, 2, 3};
    assert(std::ranges::size(v) == 3);
}

void demo_intermediate() {
    auto v = std::views::iota(0, 100);
    static_assert(std::ranges::sized_range<decltype(v)>);
    assert(std::ranges::size(v) == 100);
}

void demo_expert() {
    std::list<int> L{1, 2, 3};
    static_assert(std::ranges::sized_range<std::list<int>>);
    assert(std::ranges::size(L) == 3);
    auto f = L | std::views::filter([](int x) { return x > 0; });
    // filter_view is not sized_range
    static_assert(!std::ranges::sized_range<decltype(f)>);
    assert(std::ranges::distance(f) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/sized_range", run>;

}  // namespace
