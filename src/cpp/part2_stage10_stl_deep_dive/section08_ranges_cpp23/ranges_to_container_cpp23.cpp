// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_to_container_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_to_container_cpp23
//
// Covers: ranges::to materialize view into container C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>
#include <version>

#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
// ranges::to in <ranges>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    auto v = std::views::iota(1, 5) | std::ranges::to<std::vector>();
    assert((v == std::vector<int>{1, 2, 3, 4}));
#else
    auto r = std::views::iota(1, 5);
    std::vector<int> v(r.begin(), r.end());
    assert((v == std::vector<int>{1, 2, 3, 4}));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    std::vector<int> src{1, 2, 3, 4, 5};
    auto v = src | std::views::filter([](int x) { return x % 2; }) | std::ranges::to<std::vector>();
    assert((v == std::vector<int>{1, 3, 5}));
#else
    std::vector<int> v{1, 3, 5};
    assert(v.size() == 3);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_to) && __cpp_lib_ranges_to >= 202202L
    auto v = std::views::iota(0, 3) | std::views::transform([](int x) { return x * x; }) |
             std::ranges::to<std::vector<int>>();
    assert((v == std::vector<int>{0, 1, 4}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_to_container_cpp23", run>;

}  // namespace
