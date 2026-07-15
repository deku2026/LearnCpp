// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_reverse
// Topic id : part2/stage10/section07_ranges_core_cpp20/views_reverse
//
// Covers: views::reverse

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    auto r = v | std::views::reverse;
    std::vector<int> out(r.begin(), r.end());
    assert((out == std::vector<int>{3, 2, 1}));
}

void demo_intermediate() {
    auto r = std::views::iota(1, 5) | std::views::reverse;
    assert(*r.begin() == 4);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    auto r = v | std::views::reverse | std::views::take(2);
    std::vector<int> out(r.begin(), r.end());
    assert((out == std::vector<int>{4, 3}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/views_reverse", run>;

}  // namespace
