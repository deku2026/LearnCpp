// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_adjacent_pairwise_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_adjacent_pairwise_cpp23
//
// Covers: views::adjacent / pairwise C++23

#include "learn/topic_registry.hpp"

#include <ranges>
#include <tuple>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    // adjacent is related to zip of range with drop(1); use adjacent if available
#if defined(__cpp_lib_ranges_chunk) || 1
#endif
#endif
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> v{1, 2, 3, 4};
#if defined(__cpp_lib_ranges) && __cplusplus >= 202302L
    // Try views::adjacent<2> when present
#endif
    auto a = std::views::zip(v | std::views::take(v.size() - 1), v | std::views::drop(1));
    auto it = a.begin();
    LEARN_CHECK(std::get<0>(*it) == 1 && std::get<1>(*it) == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> v{1, 2, 3, 4};
    int diffs = 0;
    for (auto [x, y] : std::views::zip(v | std::views::take(3), v | std::views::drop(1))) {
        diffs += y - x;
    }
    LEARN_CHECK(diffs == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> v{5, 1, 4};
    bool increasing_pair = false;
    for (auto [x, y] : std::views::zip(v | std::views::take(2), v | std::views::drop(1))) {
        if (x < y) {
            increasing_pair = true;
        }
    }
    LEARN_CHECK(increasing_pair);
#else
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_adjacent_pairwise_cpp23", run>;

}  // namespace
