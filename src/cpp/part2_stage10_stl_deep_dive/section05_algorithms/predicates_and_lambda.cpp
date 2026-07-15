// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : predicates_and_lambda
// Topic id : part2/stage10/section05_algorithms/predicates_and_lambda
//
// Covers: predicates and lambdas with algorithms

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{5, 2, 8, 1, 9};
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    LEARN_CHECK(it != v.end() && *it == 8);
    int evens = static_cast<int>(std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }));
    LEARN_CHECK(evens == 2);
}

void demo_intermediate() {
    std::vector<int> v{5, 2, 8, 1, 9};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    LEARN_CHECK(v.front() == 9 && v.back() == 1);
    LEARN_CHECK(std::all_of(v.begin(), v.end(), [](int x) { return x >= 1; }));
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5};
    int threshold = 3;
    auto pred = [threshold](int x) { return x > threshold; };
    LEARN_CHECK(std::none_of(v.begin(), v.begin() + 3, pred));
    LEARN_CHECK(std::any_of(v.begin(), v.end(), pred));
    std::stable_partition(v.begin(), v.end(), pred);
    LEARN_CHECK(v[0] > 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/predicates_and_lambda", run>;

}  // namespace
