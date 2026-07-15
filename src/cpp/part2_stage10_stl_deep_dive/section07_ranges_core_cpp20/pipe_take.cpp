// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_take
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_take
//
// Covers: views::take pipe

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto t = v | std::views::take(3);
    LEARN_CHECK(std::ranges::distance(t) == 3);
    LEARN_CHECK(*t.begin() == 1);
}

void demo_intermediate() {
    auto t = std::views::iota(0) | std::views::take(5);
    LEARN_CHECK(std::ranges::distance(t) == 5);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3};
    auto t = v | std::views::take(10);  // take more than size
    LEARN_CHECK(std::ranges::distance(t) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_take", run>;

}  // namespace
