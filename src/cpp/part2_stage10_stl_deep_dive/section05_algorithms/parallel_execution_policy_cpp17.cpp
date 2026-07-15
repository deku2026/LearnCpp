// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : parallel_execution_policy_cpp17
// Topic id : part2/stage10/section05_algorithms/parallel_execution_policy_cpp17
//
// Covers: C++17 execution policies seq/par/par_unseq (safe sequential demo)

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <numeric>
#include <vector>
#include <version>

#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
#include <execution>
#endif

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 4, 1, 5};
#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
    std::sort(std::execution::seq, v.begin(), v.end());
#else
    std::sort(v.begin(), v.end());
#endif
    LEARN_CHECK(std::is_sorted(v.begin(), v.end()));
}

void demo_intermediate() {
    std::vector<int> v(100);
    std::iota(v.begin(), v.end(), 1);
#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
    // Use seq to stay portable without TBB linkage requirements
    std::for_each(std::execution::seq, v.begin(), v.end(), [](int& x) { x *= 2; });
#else
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 2; });
#endif
    LEARN_CHECK(v.front() == 2 && v.back() == 200);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5};
#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
    auto sum = std::reduce(std::execution::seq, v.begin(), v.end(), 0);
#else
    auto sum = std::reduce(v.begin(), v.end(), 0);
#endif
    LEARN_CHECK(sum == 15);
    // par/par_unseq require no data races in predicates
    LEARN_CHECK(true);
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
    ::learn::topic<"part2/stage10/section05_algorithms/parallel_execution_policy_cpp17", run>;

}  // namespace
