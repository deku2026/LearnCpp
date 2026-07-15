// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_versus_alternatives_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/mdspan_versus_alternatives_cpp23
//
// Covers: mdspan vs nested vector / raw pointer+strides

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <vector>
#include <version>

#if defined(__has_include)
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#endif
namespace {

void demo_basics() {
    // Nested vector: flexible but non-contiguous rows, double indirection
    std::vector<std::vector<int>> nested{{1, 2}, {3, 4}};
    LEARN_CHECK(nested[1][0] == 3);
    // Flat buffer + manual index: contiguous but error-prone
    std::vector<int> flat{1, 2, 3, 4};
    auto at = [&](int i, int j) -> int& { return flat[static_cast<std::size_t>(i * 2 + j)]; };
    LEARN_CHECK(at(1, 0) == 3);
}

void demo_intermediate() {
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    std::vector<int> flat{1, 2, 3, 4, 5, 6};
    std::mdspan m{flat.data(), std::extents<std::size_t, 2, 3>{}};
    LEARN_CHECK((m[1, 2] == 6));
    // mdspan: zero-copy view, multi-index, layout policy — no ownership
#else
    std::vector<int> flat{1, 2, 3, 4, 5, 6};
    LEARN_CHECK(flat[5] == 6);
#endif
}

void demo_expert() {
    // Choose: owning nested only if rows truly vary; else flat+mdspan/span
    std::vector<int> buf(12, 0);
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L && __has_include(<mdspan>)
    std::mdspan<int, std::dextents<std::size_t, 2>> m{buf.data(), 3, 4};
    m[2, 3] = 99;
    LEARN_CHECK(buf[11] == 99);
#else
    buf[11] = 99;
    LEARN_CHECK(buf[11] == 99);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/mdspan_versus_alternatives_cpp23", run>;

}  // namespace
