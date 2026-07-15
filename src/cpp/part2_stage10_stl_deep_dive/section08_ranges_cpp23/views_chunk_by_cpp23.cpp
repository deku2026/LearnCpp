// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_chunk_by_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_chunk_by_cpp23
//
// Covers: views::chunk_by C++23

#include "learn/topic_registry.hpp"

#include <functional>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    std::vector<int> v{1, 1, 2, 2, 2, 3};
    auto c = v | std::views::chunk_by(std::equal_to<>{});
    LEARN_CHECK(std::ranges::distance(c) == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    std::vector<int> v{1, 2, 3, 2, 1};
    auto c = v | std::views::chunk_by(std::less<>{});
    // increasing runs: [1,2,3] [2] wait — chunk_by keeps while pred(prev,curr)
    LEARN_CHECK(std::ranges::distance(c) >= 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    std::vector<int> v{5, 5, 5};
    auto c = v | std::views::chunk_by(std::equal_to<>{});
    LEARN_CHECK(std::ranges::distance(c) == 1);
    LEARN_CHECK(std::ranges::distance(*c.begin()) == 3);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_chunk_by_cpp23", run>;

}  // namespace
