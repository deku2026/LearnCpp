// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_slide_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_slide_cpp23
//
// Covers: views::slide C++23

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_slide) && __cpp_lib_ranges_slide >= 202202L
    std::vector<int> v{1, 2, 3, 4};
    auto s = v | std::views::slide(2);
    LEARN_CHECK(std::ranges::distance(s) == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_slide) && __cpp_lib_ranges_slide >= 202202L
    std::vector<int> v{1, 2, 3, 4};
    auto s = v | std::views::slide(3);
    auto first = *s.begin();
    std::vector<int> w(first.begin(), first.end());
    LEARN_CHECK((w == std::vector<int>{1, 2, 3}));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_slide) && __cpp_lib_ranges_slide >= 202202L
    auto s = std::views::iota(0, 5) | std::views::slide(2);
    int n = 0;
    for (auto win : s) {
        LEARN_CHECK(std::ranges::distance(win) == 2);
        ++n;
    }
    LEARN_CHECK(n == 4);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_slide_cpp23", run>;

}  // namespace
