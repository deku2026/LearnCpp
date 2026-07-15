// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_fold_overview_cpp23
// Topic id : part3/section02/ranges_fold_overview_cpp23
//
// Covers: std::ranges::fold_left / fold_right C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    int sum = std::accumulate(v.begin(), v.end(), 0);
    LEARN_CHECK(sum == 10);
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3, 4};
    auto sum = std::ranges::fold_left(v, 0, std::plus<>{});
    LEARN_CHECK(sum == 10);
    auto prod = std::ranges::fold_left(v, 1, std::multiplies<>{});
    LEARN_CHECK(prod == 24);
#else
    std::vector<int> v{1, 2, 3, 4};
    int prod = 1;
    for (int x : v) {
        prod *= x;
    }
    LEARN_CHECK(prod == 24);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    std::vector<int> v{1, 2, 3};
    auto r = std::ranges::fold_left_first(v, std::plus<>{});
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 6);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_fold_overview_cpp23", run>;

}  // namespace
