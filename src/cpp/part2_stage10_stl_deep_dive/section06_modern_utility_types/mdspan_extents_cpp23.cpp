// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_extents_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/mdspan_extents_cpp23
//
// Covers: mdspan extents static/dynamic/mixed C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <version>

#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
#include <mdspan>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    int data[6] = {1, 2, 3, 4, 5, 6};
    std::mdspan<int, std::extents<std::size_t, 2, 3>> m{data};
    assert(m.extent(0) == 2);
    assert(m.extent(1) == 3);
    assert(m.rank() == 2);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    int data[12] = {};
    std::mdspan<int, std::dextents<std::size_t, 2>> m{data, 3, 4};
    assert(m.extent(0) == 3);
    assert(m.extent(1) == 4);
    assert(m.size() == 12);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    int data[8] = {};
    // mixed: static 2, dynamic N
    std::mdspan<int, std::extents<std::size_t, 2, std::dynamic_extent>> m{data, 4};
    assert(m.extent(0) == 2);
    assert(m.extent(1) == 4);
    assert(m.static_extent(0) == 2);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/mdspan_extents_cpp23", run>;

}  // namespace
