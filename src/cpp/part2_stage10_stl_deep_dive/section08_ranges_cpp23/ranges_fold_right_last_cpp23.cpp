// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_right_last_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_fold_right_last_cpp23
//
// Covers: ranges::fold_right_last C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <optional>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3};
    auto o = std::ranges::fold_right_last(v, std::plus<>{});
    assert(o && *o == 6);
#else
    std::vector<int> v{1, 2, 3};
    std::optional<int> o;
    if (!v.empty()) {
        o = std::accumulate(v.rbegin() + 1, v.rend(), v.back());
    }
    assert(o && *o == 6);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> empty;
    auto o = std::ranges::fold_right_last(empty, std::plus<>{});
    assert(!o);
#else
    std::vector<int> empty;
    std::optional<int> o;
    assert(!o && empty.empty());
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{4};
    auto o = std::ranges::fold_right_last(v, std::multiplies<>{});
    assert(o && *o == 4);
#else
    std::vector<int> v{4};
    std::optional<int> o = v.back();
    assert(o && *o == 4);
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_fold_right_last_cpp23", run>;

}  // namespace
