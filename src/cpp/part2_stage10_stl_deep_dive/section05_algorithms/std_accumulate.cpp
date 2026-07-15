// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_accumulate
// Topic id : part2/stage10/section05_algorithms/std_accumulate
//
// Covers: std::accumulate sequential fold

#include "learn/topic_registry.hpp"

#include <functional>
#include <numeric>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(std::accumulate(v.begin(), v.end(), 0) == 10);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(std::accumulate(v.begin(), v.end(), 1, std::multiplies<>()) == 24);
    std::vector<std::string> ws{"a", "b", "c"};
    auto s = std::accumulate(ws.begin(), ws.end(), std::string{});
    LEARN_CHECK(s == "abc");
}

void demo_expert() {
    std::vector<int> v{1, 2, 3};
    // accumulate is left fold, order preserved
    auto r = std::accumulate(v.begin(), v.end(), std::string{},
                             [](std::string acc, int x) { return acc + std::to_string(x) + ","; });
    LEARN_CHECK(r == "1,2,3,");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_accumulate", run>;

}  // namespace
