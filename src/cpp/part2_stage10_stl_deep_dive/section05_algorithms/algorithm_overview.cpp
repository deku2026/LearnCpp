// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : algorithm_overview
// Topic id : part2/stage10/section05_algorithms/algorithm_overview
//
// Covers: algorithm library overview: find, sort, transform, erase-remove

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 4, 1, 5};
    auto it = std::find(v.begin(), v.end(), 4);
    assert(it != v.end() && *it == 4);
    assert(std::count(v.begin(), v.end(), 1) == 2);
    assert(std::any_of(v.begin(), v.end(), [](int x) { return x > 4; }));
}

void demo_intermediate() {
    std::vector<int> v{5, 2, 8, 1};
    std::sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    assert(std::binary_search(v.begin(), v.end(), 2));
    auto lb = std::lower_bound(v.begin(), v.end(), 5);
    assert(*lb == 5);
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
    assert((v == std::vector<int>{1, 3, 5}));
    std::vector<int> w{1, 2, 2, 3, 3, 3};
    w.erase(std::unique(w.begin(), w.end()), w.end());
    assert((w == std::vector<int>{1, 2, 3}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/algorithm_overview", run>;

}  // namespace
