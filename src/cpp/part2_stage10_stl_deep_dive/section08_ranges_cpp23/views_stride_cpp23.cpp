// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_stride_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_stride_cpp23
//
// Covers: views::stride C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_stride) && __cpp_lib_ranges_stride >= 202207L
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    auto s = v | std::views::stride(2);
    std::vector<int> out(s.begin(), s.end());
    assert((out == std::vector<int>{0, 2, 4}));
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_stride) && __cpp_lib_ranges_stride >= 202207L
    auto s = std::views::iota(0, 10) | std::views::stride(3);
    std::vector<int> out(s.begin(), s.end());
    assert((out == std::vector<int>{0, 3, 6, 9}));
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_stride) && __cpp_lib_ranges_stride >= 202207L
    std::vector<int> v{1, 2, 3, 4, 5};
    auto s = v | std::views::stride(1);
    assert(std::ranges::distance(s) == 5);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_stride_cpp23", run>;

}  // namespace
