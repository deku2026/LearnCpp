// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_fold_right_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/ranges_fold_right_cpp23
//
// Covers: ranges::fold_right C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(std::ranges::fold_right(v, 0, std::plus<>{}) == 6);
#else
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(std::accumulate(v.rbegin(), v.rend(), 0) == 6);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<std::string> ws{"a", "b", "c"};
    auto s = std::ranges::fold_right(ws, std::string{}, [](const std::string& x, std::string acc) { return x + acc; });
    LEARN_CHECK(s == "abc");
#else
    std::vector<std::string> ws{"a", "b", "c"};
    std::string s;
    for (auto it = ws.rbegin(); it != ws.rend(); ++it) {
        s = *it + s;
    }
    LEARN_CHECK(s == "abc");
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    // right fold of subtraction is not associative — demonstrates order
    std::vector<int> v{10, 3, 2};
    auto r = std::ranges::fold_right(v, 0, std::minus<>{});
    // 10 - (3 - (2 - 0)) = 10 - (3 - 2) = 10 - 1 = 9
    LEARN_CHECK(r == 9);
#else
    // right fold of subtraction is not associative — demonstrates order
    std::vector<int> v{10, 3, 2};
    int r = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        r = *it - r;
    }
    // 10 - (3 - (2 - 0)) = 9
    LEARN_CHECK(r == 9);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/ranges_fold_right_cpp23", run>;

}  // namespace
