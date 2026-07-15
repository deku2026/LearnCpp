// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_join
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_join
//
// Covers: views::join flatten range of ranges

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::vector<std::vector<int>> vv{{1, 2}, {3}, {4, 5}};
    auto j = vv | std::views::join;
    std::vector<int> out(j.begin(), j.end());
    assert((out == std::vector<int>{1, 2, 3, 4, 5}));
}

void demo_intermediate() {
    std::vector<std::string> ws{"ab", "cd"};
    auto j = ws | std::views::join;
    std::string s(j.begin(), j.end());
    assert(s == "abcd");
}

void demo_expert() {
    auto r = std::views::iota(0, 3) | std::views::transform([](int i) { return std::views::iota(0, i + 1); }) |
             std::views::join;
    std::vector<int> out;
    for (int x : r) {
        out.push_back(x);
    }
    assert((out == std::vector<int>{0, 0, 1, 0, 1, 2}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_join", run>;

}  // namespace
