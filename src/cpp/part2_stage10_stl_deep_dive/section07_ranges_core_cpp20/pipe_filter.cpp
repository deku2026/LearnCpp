// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_filter
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_filter
//
// Covers: views::filter pipe

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
    std::vector<int> out;
    for (int x : evens) {
        out.push_back(x);
    }
    LEARN_CHECK((out == std::vector<int>{2, 4}));
}

void demo_intermediate() {
    auto r = std::views::iota(1, 10) | std::views::filter([](int x) { return x > 7; });
    LEARN_CHECK(std::ranges::distance(r) == 2);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    int sum = 0;
    for (int x :
         v | std::views::filter([](int x) { return x % 2; }) | std::views::transform([](int x) { return x * x; })) {
        sum += x;
    }
    LEARN_CHECK(sum == 1 + 9);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_filter", run>;

}  // namespace
