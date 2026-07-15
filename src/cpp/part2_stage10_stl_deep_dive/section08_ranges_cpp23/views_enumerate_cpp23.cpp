// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_enumerate_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_enumerate_cpp23
//
// Covers: views::enumerate C++23

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_enumerate) && __cpp_lib_ranges_enumerate >= 202302L
    std::vector<char> v{'a', 'b', 'c'};
    auto e = v | std::views::enumerate;
    auto it = e.begin();
    LEARN_CHECK(std::get<0>(*it) == 0);
    LEARN_CHECK(std::get<1>(*it) == 'a');
#else
    std::vector<char> v{'a', 'b', 'c'};
    for (std::size_t i = 0; i < v.size(); ++i) {
        LEARN_CHECK(v[i] == static_cast<char>('a' + static_cast<int>(i)));
    }
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_enumerate) && __cpp_lib_ranges_enumerate >= 202302L
    std::vector<int> v{10, 20, 30};
    int sum_idx = 0;
    int sum_val = 0;
    for (auto [i, x] : v | std::views::enumerate) {
        sum_idx += static_cast<int>(i);
        sum_val += x;
    }
    LEARN_CHECK(sum_idx == 3);
    LEARN_CHECK(sum_val == 60);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_enumerate) && __cpp_lib_ranges_enumerate >= 202302L
    auto e = std::views::iota(1, 4) | std::views::enumerate;
    LEARN_CHECK(std::ranges::distance(e) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_enumerate_cpp23", run>;

}  // namespace
