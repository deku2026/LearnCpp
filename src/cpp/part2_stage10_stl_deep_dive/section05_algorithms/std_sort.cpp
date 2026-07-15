// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_sort
// Topic id : part2/stage10/section05_algorithms/std_sort
//
// Covers: std::sort, stable_sort, partial_sort, nth_element

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 4, 1, 5};
    std::sort(v.begin(), v.end());
    assert((v == std::vector<int>{1, 1, 3, 4, 5}));
}

void demo_intermediate() {
    std::vector<std::pair<int, char>> v{{1, 'b'}, {1, 'a'}, {2, 'c'}};
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first < b.first; });
    assert(v[0].second == 'b' && v[1].second == 'a');
    std::vector<int> w{9, 1, 8, 2, 7};
    std::partial_sort(w.begin(), w.begin() + 3, w.end());
    assert(w[0] == 1 && w[1] == 2 && w[2] == 7);
}

void demo_expert() {
    std::vector<int> v{9, 1, 8, 2, 7, 3};
    std::nth_element(v.begin(), v.begin() + 2, v.end());
    assert(v[2] == 3);
    for (std::size_t i = 0; i < 2; ++i) {
        assert(v[i] <= v[2]);
    }
    for (std::size_t i = 3; i < v.size(); ++i) {
        assert(v[i] >= v[2]);
    }
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_sort", run>;

}  // namespace
