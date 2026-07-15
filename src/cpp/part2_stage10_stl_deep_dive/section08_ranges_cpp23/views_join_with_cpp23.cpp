// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_join_with_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_join_with_cpp23
//
// Covers: views::join_with C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_join_with) && __cpp_lib_ranges_join_with >= 202202L
    std::vector<std::string> ws{"a", "b", "c"};
    auto j = ws | std::views::join_with('-');
    std::string s(j.begin(), j.end());
    assert(s == "a-b-c");
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_join_with) && __cpp_lib_ranges_join_with >= 202202L
    std::vector<std::string> ws{"xx", "yy"};
    auto j = ws | std::views::join_with(std::string_view{"::"});
    std::string s(j.begin(), j.end());
    assert(s == "xx::yy");
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_join_with) && __cpp_lib_ranges_join_with >= 202202L
    std::vector<std::vector<int>> vv{{1, 2}, {3}};
    auto j = vv | std::views::join_with(0);
    std::vector<int> out(j.begin(), j.end());
    assert((out == std::vector<int>{1, 2, 0, 3}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_join_with_cpp23", run>;

}  // namespace
