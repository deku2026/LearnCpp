// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_left_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_fold_left_cpp23
//
// Covers: ranges::fold_left C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    assert(std::ranges::fold_left(v, 0, std::plus<>{}) == 10);
#else
    std::vector<int> v{1, 2, 3, 4};
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    assert(std::ranges::fold_left(v, 1, std::multiplies<>{}) == 24);
#else
    std::vector<int> v{1, 2, 3, 4};
    assert(std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{}) == 24);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    auto r = std::views::iota(1, 5);
    assert(std::ranges::fold_left(r, 0, std::plus<>{}) == 10);
#else
    int s = 0;
    for (int x : std::views::iota(1, 5)) {
        s += x;
    }
    assert(s == 10);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_fold_left_cpp23", run>;

}  // namespace
