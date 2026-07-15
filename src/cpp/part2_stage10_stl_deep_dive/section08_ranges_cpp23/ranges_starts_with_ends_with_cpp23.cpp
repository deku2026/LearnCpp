// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_starts_with_ends_with_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_starts_with_ends_with_cpp23
//
// Covers: ranges::starts_with / ends_with C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_starts_ends_with) && __cpp_lib_ranges_starts_ends_with >= 202106L
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK((std::ranges::starts_with(v, std::vector<int>{1, 2})));
    LEARN_CHECK((std::ranges::ends_with(v, std::vector<int>{3, 4})));
#else
    std::string s = "hello";
    LEARN_CHECK(s.starts_with("he"));
    LEARN_CHECK(s.ends_with("lo"));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_starts_ends_with) && __cpp_lib_ranges_starts_ends_with >= 202106L
    LEARN_CHECK(std::ranges::starts_with(std::views::iota(0, 5), std::views::iota(0, 2)));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_starts_ends_with) && __cpp_lib_ranges_starts_ends_with >= 202106L
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(!std::ranges::starts_with(v, std::vector<int>{2}));
    LEARN_CHECK(std::ranges::ends_with(v, std::vector<int>{3}));
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_starts_with_ends_with_cpp23", run>;

}  // namespace
