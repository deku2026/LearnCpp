// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_random_engines_distributions
// Topic id : part2/stage10/section11_other_stl/std_random_engines_distributions
//
// Covers: random engines and distributions

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <random>
#include <vector>

namespace {

void demo_basics() {
    std::mt19937 gen{42};
    std::uniform_int_distribution<int> dist(1, 6);
    const int r = dist(gen);
    LEARN_CHECK(r >= 1 && r <= 6);
}

void demo_intermediate() {
    std::mt19937 gen{1};
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    const double x = dist(gen);
    LEARN_CHECK(x >= 0.0 && x < 1.0);
    std::normal_distribution<double> nd(0.0, 1.0);
    (void)nd(gen);
}

void demo_expert() {
    std::seed_seq seed{1, 2, 3, 4};
    std::mt19937 gen{seed};
    std::mt19937 gen2{seed};
    std::uniform_int_distribution<int> dist(0, 1000);
    LEARN_CHECK(dist(gen) == dist(gen2));  // same seed_seq -> same stream start
    std::vector<int> v{1, 2, 3, 4, 5};
    std::shuffle(v.begin(), v.end(), gen);
    LEARN_CHECK(v.size() == 5);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section11_other_stl/std_random_engines_distributions", run>;

}  // namespace
