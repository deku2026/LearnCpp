// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : borrowed_range_dangling
// Topic id : part2/stage10/section07_ranges_core_cpp20/borrowed_range_dangling
//
// Covers: borrowed_range and ranges::dangling safety

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::borrowed_range<std::span<int>>);
    static_assert(!std::ranges::borrowed_range<std::vector<int>>);
    std::vector<int> v{1, 2, 3};
    auto it = std::ranges::find(v, 2);
    assert(it != v.end() && *it == 2);
}

void demo_intermediate() {
    // find on temporary vector returns dangling (not a usable iterator)
    auto d = std::ranges::find(std::vector<int>{1, 2, 3}, 2);
    static_assert(std::is_same_v<decltype(d), std::ranges::dangling>);
    (void)d;
    assert(true);
}

void demo_expert() {
    int a[] = {1, 2, 3};
    static_assert(std::ranges::borrowed_range<decltype((a))>);
    auto it = std::ranges::find(a, 3);
    assert(*it == 3);
    std::span<int> sp = a;
    auto it2 = std::ranges::find(sp, 1);
    assert(*it2 == 1);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/borrowed_range_dangling", run>;

}  // namespace
