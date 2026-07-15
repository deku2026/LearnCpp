// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numbers_constants
// Topic id : part3/section04/numbers_constants
//
// Covers: std::numbers mathematical constants C++20

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <numbers>

namespace {

void demo_basics() {
    assert(std::numbers::pi > 3.14 && std::numbers::pi < 3.15);
    assert(std::numbers::e > 2.71 && std::numbers::e < 2.72);
}

void demo_intermediate() {
    assert(std::numbers::sqrt2 > 1.41 && std::numbers::sqrt2 < 1.42);
    assert(std::numbers::ln2 > 0.69 && std::numbers::ln2 < 0.70);
}

void demo_expert() {
    const float pi_f = std::numbers::pi_v<float>;
    assert(pi_f > 3.14f && pi_f < 3.15f);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numbers_constants", run>;

}  // namespace
