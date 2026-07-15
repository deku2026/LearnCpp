// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_common
// Topic id : part2/stage10/section07_ranges_core_cpp20/views_common
//
// Covers: views::common for iterator-pair algorithms

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    auto v = std::views::iota(0, 5) | std::views::common;
    std::vector<int> out(v.begin(), v.end());
    assert(out.size() == 5);
}

void demo_intermediate() {
    std::vector<int> src{5, 1, 4, 2};
    auto t = src | std::views::take(4) | std::views::common;
    std::vector<int> v(t.begin(), t.end());
    std::sort(v.begin(), v.end());
    assert(v.front() == 1);
}

void demo_expert() {
    auto r = std::views::iota(0, 3) | std::views::filter([](int x) { return x >= 0; }) | std::views::common;
    static_assert(std::ranges::common_range<decltype(r)>);
    assert(std::distance(r.begin(), r.end()) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/views_common", run>;

}  // namespace
