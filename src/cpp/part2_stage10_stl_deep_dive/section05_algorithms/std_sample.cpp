// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_sample
// Topic id : part2/stage10/section05_algorithms/std_sample
//
// Covers: std::sample C++17 random sampling

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> out;
    std::sample(src.begin(), src.end(), std::back_inserter(out), 3, std::mt19937{42});
    assert(out.size() == 3);
    for (int x : out) {
        assert(std::find(src.begin(), src.end(), x) != src.end());
    }
}

void demo_intermediate() {
    std::vector<int> src{10, 20, 30};
    std::vector<int> out(2);
    std::sample(src.begin(), src.end(), out.begin(), 2, std::mt19937{1});
    assert(out[0] != out[1] || src.size() < 2);
}

void demo_expert() {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> a, b;
    std::mt19937 g1{7};
    std::mt19937 g2{7};
    std::sample(src.begin(), src.end(), std::back_inserter(a), 3, g1);
    std::sample(src.begin(), src.end(), std::back_inserter(b), 3, g2);
    assert(a == b);  // same seed -> same sample
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_sample", run>;

}  // namespace
