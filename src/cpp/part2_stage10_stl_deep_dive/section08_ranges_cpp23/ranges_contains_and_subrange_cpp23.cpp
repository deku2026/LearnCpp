// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_contains_and_subrange_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_contains_and_subrange_cpp23
//
// Covers: ranges::contains C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
    std::vector<int> v{1, 2, 3};
    assert(std::ranges::contains(v, 2));
    assert(!std::ranges::contains(v, 9));
#else
    std::vector<int> v{1, 2, 3};
    assert(std::ranges::find(v, 2) != v.end());
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
    assert(std::ranges::contains(std::views::iota(0, 5), 3));
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    assert((std::ranges::contains_subrange(v, std::vector<int>{2, 3})));
    assert((!std::ranges::contains_subrange(v, std::vector<int>{2, 4})));
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_contains_and_subrange_cpp23", run>;

}  // namespace
