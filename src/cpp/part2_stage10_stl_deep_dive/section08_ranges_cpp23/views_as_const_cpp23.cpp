// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_as_const_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_as_const_cpp23
//
// Covers: views::as_const C++23

#include "learn/topic_registry.hpp"

#include <ranges>
#include <type_traits>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_as_const) && __cpp_lib_ranges_as_const >= 202207L
    std::vector<int> v{1, 2, 3};
    auto c = v | std::views::as_const;
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*c.begin())>>);
    LEARN_CHECK(*c.begin() == 1);
#else
    const std::vector<int> v{1, 2, 3};
    LEARN_CHECK(v[0] == 1);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_as_const) && __cpp_lib_ranges_as_const >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    auto c = v | std::views::as_const | std::views::take(2);
    LEARN_CHECK(std::ranges::distance(c) == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_as_const) && __cpp_lib_ranges_as_const >= 202207L
    std::vector<int> v{5, 6};
    for (auto&& x : v | std::views::as_const) {
        LEARN_CHECK(x >= 5);
    }
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_as_const_cpp23", run>;

}  // namespace
