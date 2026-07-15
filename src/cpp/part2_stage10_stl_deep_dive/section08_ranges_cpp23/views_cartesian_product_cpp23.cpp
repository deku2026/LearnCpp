// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_cartesian_product_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_cartesian_product_cpp23
//
// Covers: views::cartesian_product C++23

#include "learn/topic_registry.hpp"

#include <ranges>
#include <tuple>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_cartesian_product) && __cpp_lib_ranges_cartesian_product >= 202207L
    auto c = std::views::cartesian_product(std::views::iota(0, 2), std::views::iota(0, 3));
    LEARN_CHECK(std::ranges::distance(c) == 6);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_cartesian_product) && __cpp_lib_ranges_cartesian_product >= 202207L
    std::vector<int> a{1, 2};
    std::vector<char> b{'x', 'y'};
    auto c = std::views::cartesian_product(a, b);
    auto it = c.begin();
    LEARN_CHECK(std::get<0>(*it) == 1);
    LEARN_CHECK(std::get<1>(*it) == 'x');
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_cartesian_product) && __cpp_lib_ranges_cartesian_product >= 202207L
    int n = 0;
    for (auto [i, j] : std::views::cartesian_product(std::views::iota(0, 2), std::views::iota(0, 2))) {
        n += i + j;
    }
    LEARN_CHECK(n == 4);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_cartesian_product_cpp23", run>;

}  // namespace
