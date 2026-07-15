// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_take_while
// Topic id : part2/stage10/section07_ranges_core_cpp20/pipe_take_while
//
// Covers: views::take_while pipe

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto t = v | std::views::take_while([](int x) { return x < 4; });
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    auto out = t | std::ranges::to<std::vector>();
#else
    auto c = t | std::views::common;
    std::vector<int> out(std::ranges::begin(c), std::ranges::end(c));
#endif
    LEARN_CHECK((out == std::vector<int>{1, 2, 3}));
}

void demo_intermediate() {
    auto t = std::views::iota(0) | std::views::take_while([](int x) { return x < 3; });
    LEARN_CHECK(std::ranges::distance(t) == 3);
}

void demo_expert() {
    std::vector<int> v{2, 4, 6, 1, 8};
    auto t = v | std::views::take_while([](int x) { return x % 2 == 0; });
    LEARN_CHECK(std::ranges::distance(t) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/pipe_take_while", run>;

}  // namespace
