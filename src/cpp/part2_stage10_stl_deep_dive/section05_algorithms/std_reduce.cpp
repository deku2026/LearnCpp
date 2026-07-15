// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_reduce
// Topic id : part2/stage10/section05_algorithms/std_reduce
//
// Covers: std::reduce possibly unordered/parallel fold

#include "learn/topic_registry.hpp"

#include <cassert>
#include <numeric>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    assert(std::reduce(v.begin(), v.end()) == 10);
    assert(std::reduce(v.begin(), v.end(), 0) == 10);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    assert(std::reduce(v.begin(), v.end(), 1, std::multiplies<>()) == 24);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    // transform_reduce: map then fold
    auto r = std::transform_reduce(v.begin(), v.end(), 0, std::plus<>(), [](int x) { return x * x; });
    assert(r == 30);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_reduce", run>;

}  // namespace
