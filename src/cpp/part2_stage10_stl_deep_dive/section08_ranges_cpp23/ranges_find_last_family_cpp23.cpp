// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_find_last_family_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_find_last_family_cpp23
//
// Covers: ranges::find_last family C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
    std::vector<int> v{1, 2, 3, 2, 1};
    auto [it, end] = std::ranges::find_last(v, 2);
    LEARN_CHECK(it != end && *it == 2);
    LEARN_CHECK(it == v.begin() + 3);
#else
    std::vector<int> v{1, 2, 3, 2, 1};
    auto it = std::find(v.rbegin(), v.rend(), 2);
    LEARN_CHECK(it != v.rend() && *it == 2);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
    std::vector<int> v{1, 2, 3, 4, 5};
    auto [it, end] = std::ranges::find_last_if(v, [](int x) { return x % 2 == 0; });
    LEARN_CHECK(it != end && *it == 4);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
    std::vector<int> v{1, 3, 5};
    auto [it, end] = std::ranges::find_last_if_not(v, [](int x) { return x % 2 == 0; });
    LEARN_CHECK(it != end && *it == 5);
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_find_last_family_cpp23", run>;

}  // namespace
