// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_zip_transform_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_zip_transform_cpp23
//
// Covers: views::zip_transform C++23

#include "learn/topic_registry.hpp"

#include <functional>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};
    auto z = std::views::zip_transform(std::plus<>{}, a, b);
    std::vector<int> out(z.begin(), z.end());
    LEARN_CHECK((out == std::vector<int>{11, 22, 33}));
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    std::vector<int> a{1, 2};
    std::vector<int> b{3, 4};
    auto z = std::views::zip_transform([](int x, int y) { return x * y; }, a, b);
    LEARN_CHECK(*z.begin() == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    auto z =
        std::views::zip_transform([](int x, int y) { return x - y; }, std::views::iota(5, 8), std::views::iota(1, 4));
    std::vector<int> out(z.begin(), z.end());
    LEARN_CHECK((out == std::vector<int>{4, 4, 4}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_zip_transform_cpp23", run>;

}  // namespace
