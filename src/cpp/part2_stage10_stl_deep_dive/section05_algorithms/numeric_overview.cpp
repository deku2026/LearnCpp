// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : numeric_overview
// Topic id : part2/stage10/section05_algorithms/numeric_overview
//
// Covers: numeric: accumulate, iota, partial_sum, inner_product

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <numeric>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
    std::vector<int> seq(5);
    std::iota(seq.begin(), seq.end(), 10);
    assert(seq.front() == 10 && seq.back() == 14);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    std::vector<int> ps(v.size());
    std::partial_sum(v.begin(), v.end(), ps.begin());
    assert((ps == std::vector<int>{1, 3, 6, 10}));
    int prod = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>());
    assert(prod == 24);
}

void demo_expert() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    assert(std::inner_product(a.begin(), a.end(), b.begin(), 0) == 32);
    std::vector<int> d(3);
    std::adjacent_difference(a.begin(), a.end(), d.begin());
    assert((d == std::vector<int>{1, 1, 1}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/numeric_overview", run>;

}  // namespace
