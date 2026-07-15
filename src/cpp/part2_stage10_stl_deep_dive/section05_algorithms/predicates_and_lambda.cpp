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
#include <cassert>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{5, 2, 8, 1, 9};
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    assert(it != v.end() && *it == 8);
    int evens = static_cast<int>(std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }));
    assert(evens == 2);
}

void demo_intermediate() {
    std::vector<int> v{5, 2, 8, 1, 9};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    assert(v.front() == 9 && v.back() == 1);
    assert(std::all_of(v.begin(), v.end(), [](int x) { return x >= 1; }));
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5};
    int threshold = 3;
    auto pred = [threshold](int x) { return x > threshold; };
    assert(std::none_of(v.begin(), v.begin() + 3, pred));
    assert(std::any_of(v.begin(), v.end(), pred));
    std::stable_partition(v.begin(), v.end(), pred);
    assert(v[0] > 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/predicates_and_lambda", run>;

}  // namespace
