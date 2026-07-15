// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_shift_left_right_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_shift_left_right_cpp23
//
// Covers: std::ranges::shift_left / shift_right C++23 (or std::shift C++20)

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_shift) && __cpp_lib_shift >= 201806L
    std::vector<int> v{1, 2, 3, 4, 5};
    auto it = std::shift_left(v.begin(), v.end(), 2);
    // [3,4,5, ?, ?] — elements after it are moved-from
    assert(v[0] == 3);
    assert(v[1] == 4);
    assert(v[2] == 5);
    (void)it;
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_shift) && __cpp_lib_shift >= 201806L
    std::vector<int> v{1, 2, 3, 4, 5};
    std::shift_right(v.begin(), v.end(), 2);
    assert(v[2] == 1);
    assert(v[3] == 2);
    assert(v[4] == 3);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_shift) && __cpp_lib_shift >= 201806L
    std::vector<int> v{1, 2, 3};
    auto it = std::shift_left(v.begin(), v.end(), 0);
    assert(it == v.end());
    assert((v == std::vector<int>{1, 2, 3}));
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_shift_left_right_cpp23", run>;

}  // namespace
