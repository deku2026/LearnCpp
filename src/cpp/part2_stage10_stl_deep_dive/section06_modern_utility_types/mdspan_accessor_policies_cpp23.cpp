// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_accessor_policies_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/mdspan_accessor_policies_cpp23
//
// Covers: mdspan default_accessor and accessor policy idea C++23

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <type_traits>
#include <version>

#if defined(__has_include)
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    int data[4] = {1, 2, 3, 4};
    std::mdspan<int, std::extents<std::size_t, 2, 2>, std::layout_right, std::default_accessor<int>> m{data};
    LEARN_CHECK((m[0, 1] == 2));
    m[1, 0] = 9;
    LEARN_CHECK(data[2] == 9);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    int data[4] = {1, 2, 3, 4};
    std::default_accessor<int> acc{};
    LEARN_CHECK(acc.access(data, 2) == 3);
    acc.access(data, 2) = 30;
    LEARN_CHECK(data[2] == 30);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    // Accessor customizes pointer→element (e.g. scaled, atomic, checked)
    const int data[3] = {7, 8, 9};
    std::mdspan<const int, std::extents<std::size_t, 3>> m{data};
    LEARN_CHECK(m[2] == 9);
    static_assert(std::is_same_v<decltype(m)::accessor_type, std::default_accessor<const int>>);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/mdspan_accessor_policies_cpp23", run>;

}  // namespace
