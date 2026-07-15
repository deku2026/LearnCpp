// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_gcd_lcm
// Topic id : part2/stage10/section05_algorithms/std_gcd_lcm
//
// Covers: std::gcd and std::lcm C++17

#include "learn/topic_registry.hpp"

#include <cassert>
#include <numeric>

namespace {

void demo_basics() {
    assert(std::gcd(12, 18) == 6);
    assert(std::gcd(7, 3) == 1);
}

void demo_intermediate() {
    assert(std::lcm(4, 6) == 12);
    assert(std::lcm(3, 5) == 15);
}

void demo_expert() {
    assert(std::gcd(0, 5) == 5);
    assert(std::lcm(0, 5) == 0);
    assert(std::gcd(54, 24) * std::lcm(54, 24) == 54 * 24);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_gcd_lcm", run>;

}  // namespace
