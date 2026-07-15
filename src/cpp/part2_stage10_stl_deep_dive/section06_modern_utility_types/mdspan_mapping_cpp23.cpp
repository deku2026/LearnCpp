// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_mapping_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/mdspan_mapping_cpp23
//
// Covers: mdspan mapping multi-index to offset C++23

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
    int data[6] = {10, 20, 30, 40, 50, 60};
    std::mdspan m{data, std::extents<std::size_t, 2, 3>{}};
    assert((m[1, 2] == 60));
    const auto& map = m.mapping();
    assert(map(1, 2) == 5);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    int data[9] = {};
    std::mdspan<int, std::extents<std::size_t, 3, 3>> m{data};
    m[1, 1] = 42;
    assert(data[4] == 42);  // row-major offset 1*3+1
    assert(m.mapping()(1, 1) == 4);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    int data[8] = {};
    std::mdspan<int, std::dextents<std::size_t, 2>> m{data, 2, 4};
    for (std::size_t i = 0; i < m.extent(0); ++i) {
        for (std::size_t j = 0; j < m.extent(1); ++j) {
            m[i, j] = static_cast<int>(i * 10 + j);
        }
    }
    assert((m[1, 3] == 13));
    assert(m.mapping().required_span_size() == 8);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/mdspan_mapping_cpp23", run>;

}  // namespace
