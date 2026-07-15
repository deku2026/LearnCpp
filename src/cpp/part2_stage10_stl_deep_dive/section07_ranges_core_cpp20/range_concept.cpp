// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : range_concept
// Topic id : part2/stage10/section07_ranges_core_cpp20/range_concept
//
// Covers: std::ranges::range concept

#include "learn/topic_registry.hpp"

#include <concepts>
#include <ranges>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::ranges::range<std::vector<int>>);
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(std::ranges::begin(v) != std::ranges::end(v));
}

void demo_intermediate() {
    int a[] = {1, 2, 3};
    static_assert(std::ranges::range<decltype(a)>);
    LEARN_CHECK(std::ranges::size(a) == 3);
}

void demo_expert() {
    static_assert(std::ranges::range<std::string>);
    static_assert(!std::ranges::range<int>);
    std::vector<int> v;
    LEARN_CHECK(std::ranges::empty(v));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/range_concept", run>;

}  // namespace
