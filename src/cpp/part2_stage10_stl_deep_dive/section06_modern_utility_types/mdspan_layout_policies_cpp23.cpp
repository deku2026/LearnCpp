// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_layout_policies_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/mdspan_layout_policies_cpp23
//
// Covers: mdspan layout_right/left/stride policies C++23

#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <version>

#if defined(__has_include)
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    int data[6] = {0, 1, 2, 3, 4, 5};
    // layout_right = row-major (default)
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_right> mr{data};
    LEARN_CHECK((mr[0, 0] == 0));
    LEARN_CHECK((mr[0, 1] == 1));
    LEARN_CHECK((mr[1, 0] == 3));
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    int data[6] = {0, 1, 2, 3, 4, 5};
    // layout_left = column-major
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_left> ml{data};
    LEARN_CHECK((ml[0, 0] == 0));
    LEARN_CHECK((ml[1, 0] == 1));
    LEARN_CHECK((ml[0, 1] == 2));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    int data[12] = {};
    for (int i = 0; i < 12; ++i) {
        data[i] = i;
    }
    std::array<std::size_t, 2> strides{2, 6};  // custom stride example via layout_stride
    std::layout_stride::mapping<std::extents<std::size_t, 2, 2>> map{std::extents<std::size_t, 2, 2>{}, strides};
    std::mdspan<int, std::extents<std::size_t, 2, 2>, std::layout_stride> ms{data, map};
    LEARN_CHECK((ms[0, 0] == 0));
    LEARN_CHECK(ms.stride(0) == 2);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/mdspan_layout_policies_cpp23", run>;

}  // namespace
