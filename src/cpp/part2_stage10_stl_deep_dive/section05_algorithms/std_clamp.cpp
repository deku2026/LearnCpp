// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_clamp
// Topic id : part2/stage10/section05_algorithms/std_clamp
//
// Covers: std::clamp C++17

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>

namespace {

void demo_basics() {
    assert(std::clamp(5, 0, 10) == 5);
    assert(std::clamp(-1, 0, 10) == 0);
    assert(std::clamp(99, 0, 10) == 10);
}

void demo_intermediate() {
    assert(std::clamp(3.5, 1.0, 2.0) == 2.0);
    assert(std::clamp(1.5, 1.0, 2.0) == 1.5);
}

void demo_expert() {
    const int lo = 0;
    const int hi = 100;
    for (int x : {-10, 0, 50, 100, 200}) {
        const int c = std::clamp(x, lo, hi);
        assert(c >= lo && c <= hi);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_clamp", run>;

}  // namespace
