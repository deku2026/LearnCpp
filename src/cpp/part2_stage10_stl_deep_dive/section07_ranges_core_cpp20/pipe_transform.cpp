// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_transform
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_transform
//
// Covers: views::transform pipe

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    auto sq = v | std::views::transform([](int x) { return x * x; });
    std::vector<int> out(sq.begin(), sq.end());
    LEARN_CHECK((out == std::vector<int>{1, 4, 9}));
}

void demo_intermediate() {
    auto r = std::views::iota(1, 5) | std::views::transform([](int x) { return x + 10; });
    LEARN_CHECK(*r.begin() == 11);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3};
    auto r =
        v | std::views::transform([](int x) { return x * 2; }) | std::views::transform([](int x) { return x + 1; });
    std::vector<int> out(r.begin(), r.end());
    LEARN_CHECK((out == std::vector<int>{3, 5, 7}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_transform", run>;

}  // namespace
