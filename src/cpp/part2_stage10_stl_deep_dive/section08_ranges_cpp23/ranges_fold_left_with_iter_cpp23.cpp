// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_left_with_iter_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_fold_left_with_iter_cpp23
//
// Covers: ranges::fold_left_with_iter C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    // in_value_result: .in is the end iterator, .value is the folded result
    auto result = std::ranges::fold_left_with_iter(v, 0, std::plus<>{});
    assert(result.value == 10);
    assert(result.in == v.end());
#else
    std::vector<int> v{1, 2, 3, 4};
    auto val = std::accumulate(v.begin(), v.end(), 0);
    auto it = v.end();
    assert(val == 10);
    assert(it == v.end());
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    auto result = std::ranges::fold_left_with_iter(v.begin(), v.begin() + 2, 0, std::plus<>{});
    assert(result.value == 3);
    assert(result.in == v.begin() + 2);
#else
    std::vector<int> v{1, 2, 3, 4};
    auto it = v.begin() + 2;
    auto val = std::accumulate(v.begin(), it, 0);
    assert(val == 3);
    assert(it == v.begin() + 2);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{2, 3, 4};
    auto result = std::ranges::fold_left_with_iter(v, 1, std::multiplies<>{});
    assert(result.value == 24);
    assert(result.in == v.end());
#else
    std::vector<int> v{2, 3, 4};
    auto val = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});
    auto it = v.end();
    assert(val == 24);
    assert(it == v.end());
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_fold_left_with_iter_cpp23", run>;

}  // namespace
