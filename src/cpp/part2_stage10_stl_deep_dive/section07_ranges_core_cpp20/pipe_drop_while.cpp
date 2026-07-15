// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_drop_while
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_drop_while
//
// Covers: views::drop_while pipe

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{0, 0, 1, 2, 0};
    auto d = v | std::views::drop_while([](int x) { return x == 0; });
    std::vector<int> out(d.begin(), d.end());
    assert((out == std::vector<int>{1, 2, 0}));
}

void demo_intermediate() {
    auto d = std::views::iota(0, 10) | std::views::drop_while([](int x) { return x < 7; });
    assert(*d.begin() == 7);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3};
    auto d = v | std::views::drop_while([](int) { return true; });
    assert(std::ranges::empty(d));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_drop_while", run>;

}  // namespace
