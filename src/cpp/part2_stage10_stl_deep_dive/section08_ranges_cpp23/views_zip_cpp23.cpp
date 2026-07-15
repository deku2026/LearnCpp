// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_zip_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_zip_cpp23
//
// Covers: views::zip C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <tuple>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> a{1, 2, 3};
    std::vector<char> b{'a', 'b', 'c'};
    auto z = std::views::zip(a, b);
    auto it = z.begin();
    assert(std::get<0>(*it) == 1);
    assert(std::get<1>(*it) == 'a');
#else
    std::vector<int> a{1, 2, 3};
    assert(a.size() == 3);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> a{1, 2};
    std::vector<int> b{10, 20, 30};
    auto z = std::views::zip(a, b);
    assert(std::ranges::distance(z) == 2);  // min length
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    int sum = 0;
    for (auto [x, y] : std::views::zip(a, b)) {
        sum += x + y;
    }
    assert(sum == 21);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_zip_cpp23", run>;

}  // namespace
