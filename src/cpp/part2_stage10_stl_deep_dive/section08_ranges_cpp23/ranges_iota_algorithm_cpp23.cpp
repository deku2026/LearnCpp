// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_iota_algorithm_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_iota_algorithm_cpp23
//
// Covers: ranges::iota algorithm C++23

#include "learn/topic_registry.hpp"

#include <numeric>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
    std::vector<int> v(5);
    std::ranges::iota(v, 10);
    LEARN_CHECK((v == std::vector<int>{10, 11, 12, 13, 14}));
#else
    std::vector<int> v(5);
    std::iota(v.begin(), v.end(), 10);
    LEARN_CHECK((v == std::vector<int>{10, 11, 12, 13, 14}));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
    std::vector<int> v(3);
    auto result = std::ranges::iota(v, 0);
    LEARN_CHECK(result.out == v.end());
    LEARN_CHECK(result.value == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
    std::vector<int> v(4);
    std::ranges::iota(v.begin() + 1, v.end(), 100);
    LEARN_CHECK(v[1] == 100 && v[3] == 102);
#else
    std::vector<int> v(4);
    std::iota(v.begin() + 1, v.end(), 100);
    LEARN_CHECK(v[1] == 100);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_iota_algorithm_cpp23", run>;

}  // namespace
