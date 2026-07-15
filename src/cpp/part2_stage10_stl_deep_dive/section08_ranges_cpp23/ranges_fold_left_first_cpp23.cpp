// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_left_first_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_fold_left_first_cpp23
//
// Covers: ranges::fold_left_first C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
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
    auto o = std::ranges::fold_left_first(v, std::plus<>{});
    LEARN_CHECK(o && *o == 6);
#else
    std::vector<int> v{1, 2, 3};
    std::optional<int> o;
    if (!v.empty()) {
        o = std::accumulate(v.begin() + 1, v.end(), v.front());
    }
    LEARN_CHECK(o && *o == 6);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> empty;
    auto o = std::ranges::fold_left_first(empty, std::plus<>{});
    LEARN_CHECK(!o);
#else
    std::vector<int> empty;
    std::optional<int> o;
    if (!empty.empty()) {
        o = std::accumulate(empty.begin() + 1, empty.end(), empty.front());
    }
    LEARN_CHECK(!o);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{5};
    auto o = std::ranges::fold_left_first(v, std::multiplies<>{});
    LEARN_CHECK(o && *o == 5);
#else
    std::vector<int> v{5};
    std::optional<int> o = v.front();
    LEARN_CHECK(o && *o == 5);
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
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_fold_left_first_cpp23", run>;

}  // namespace
