// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_iota
// Topic id : part2/stage10/section07_ranges_core_cpp20/views_iota
//
// Covers: views::iota sequence generator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    auto v = std::views::iota(1, 5);
    std::vector<int> out(v.begin(), v.end());
    assert((out == std::vector<int>{1, 2, 3, 4}));
}

void demo_intermediate() {
    auto v = std::views::iota(10) | std::views::take(3) | std::views::common;
    std::vector<int> out(v.begin(), v.end());
    assert((out == std::vector<int>{10, 11, 12}));
}

void demo_expert() {
    int sum = 0;
    for (int x : std::views::iota(0, 100) | std::views::filter([](int x) { return x % 10 == 0; })) {
        sum += x;
    }
    assert(sum == 450);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/views_iota", run>;

}  // namespace
