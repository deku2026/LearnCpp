// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_transform
// Topic id : part2/stage10/section05_algorithms/std_transform
//
// Covers: std::transform unary and binary

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    std::vector<int> out;
    std::transform(v.begin(), v.end(), std::back_inserter(out), [](int x) { return x * x; });
    LEARN_CHECK((out == std::vector<int>{1, 4, 9}));
}

void demo_intermediate() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};
    std::vector<int> c(3);
    std::transform(a.begin(), a.end(), b.begin(), c.begin(), std::plus<>());
    LEARN_CHECK((c == std::vector<int>{11, 22, 33}));
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    std::transform(v.begin(), v.end(), v.begin(), [](int x) { return x + 1; });
    LEARN_CHECK((v == std::vector<int>{2, 3, 4, 5}));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_transform", run>;

}  // namespace
