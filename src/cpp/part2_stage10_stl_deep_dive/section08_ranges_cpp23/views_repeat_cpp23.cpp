// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_repeat_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_repeat_cpp23
//
// Covers: views::repeat C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_repeat) && __cpp_lib_ranges_repeat >= 202207L
    auto r = std::views::repeat(7, 3);
    std::vector<int> out(r.begin(), r.end());
    assert((out == std::vector<int>{7, 7, 7}));
#else
    std::vector<int> out(3, 7);
    assert((out == std::vector<int>{7, 7, 7}));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_repeat) && __cpp_lib_ranges_repeat >= 202207L
    auto r = std::views::repeat(1) | std::views::take(5);
    assert(std::ranges::distance(r) == 5);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_repeat) && __cpp_lib_ranges_repeat >= 202207L
    int sum = 0;
    for (int x : std::views::repeat(2, 4)) {
        sum += x;
    }
    assert(sum == 8);
#else
    assert(true);
#endif
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_repeat_cpp23", run>;

}  // namespace
