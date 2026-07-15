// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : random_engines_distributions
// Topic id : part3/section04/random_engines_distributions
//
// Covers: random engines and distributions

#include "learn/topic_registry.hpp"

#include <cassert>
#include <random>

namespace {

void demo_basics() {
    std::mt19937 eng{42};
    std::uniform_int_distribution<int> dist{1, 6};
    int x = dist(eng);
    assert(x >= 1 && x <= 6);
}

void demo_intermediate() {
    std::mt19937 eng{1};
    std::uniform_real_distribution<double> dist{0.0, 1.0};
    double x = dist(eng);
    assert(x >= 0.0 && x < 1.0);
}

void demo_expert() {
    std::seed_seq seed{1, 2, 3, 4};
    std::mt19937 eng{seed};
    std::normal_distribution<double> nd{0.0, 1.0};
    double z = nd(eng);
    assert(z == z);  // not NaN
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/random_engines_distributions", run>;

}  // namespace
