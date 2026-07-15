// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : range_v3_origin
// Topic id : part2/stage10/section09_customization_points/range_v3_origin
//
// Covers: range-v3 origin of std::ranges design

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    // std::ranges design grew from Eric Niebler's range-v3 library
    // Core ideas: views, pipe |, concepts, CPO begin/end
    std::vector<int> v{1, 2, 3, 4, 5};
    auto r = v | std::views::filter([](int x) { return x % 2; }) | std::views::transform([](int x) { return x * x; });
    int sum = 0;
    for (int x : r) {
        sum += x;
    }
    assert(sum == 1 + 9 + 25);
}

void demo_intermediate() {
    // Lazy evaluation: pipeline does not allocate intermediate containers
    auto r = std::views::iota(1, 100) | std::views::filter([](int x) { return x % 10 == 0; }) | std::views::take(3);
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    auto out = r | std::ranges::to<std::vector>();
#else
    auto c = r | std::views::common;
    std::vector<int> out(std::ranges::begin(c), std::ranges::end(c));
#endif
    assert((out == std::vector<int>{10, 20, 30}));
}

void demo_expert() {
    // range-v3 had actions; std::ranges focuses on views + algorithms
    // Materialize with iterator-pair construction or ranges::to (C++23)
    auto r = std::views::iota(0, 5);
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    auto v = r | std::ranges::to<std::vector>();
#else
    auto c = r | std::views::common;
    std::vector<int> v(std::ranges::begin(c), std::ranges::end(c));
#endif
    assert(v.size() == 5);
    assert(std::ranges::equal(v, std::views::iota(0, 5)));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section09_customization_points/range_v3_origin", run>;

}  // namespace
