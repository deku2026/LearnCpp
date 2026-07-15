// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_chunk_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_chunk_cpp23
//
// Covers: views::chunk C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    std::vector<int> v{1, 2, 3, 4, 5};
    auto c = v | std::views::chunk(2);
    assert(std::ranges::distance(c) == 3);
    assert(std::ranges::distance(*c.begin()) == 2);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    std::vector<int> v{1, 2, 3, 4, 5};
    auto c = v | std::views::chunk(2);
    auto it = c.begin();
    ++it;
    ++it;
    assert(std::ranges::distance(*it) == 1);  // last chunk remainder
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    auto c = std::views::iota(0, 6) | std::views::chunk(3);
    int n = 0;
    for (auto chunk : c) {
        n += static_cast<int>(std::ranges::distance(chunk));
    }
    assert(n == 6);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_chunk_cpp23", run>;

}  // namespace
