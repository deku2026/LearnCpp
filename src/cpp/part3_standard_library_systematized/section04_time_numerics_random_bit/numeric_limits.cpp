// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_limits
// Topic id : part3/section04/numeric_limits
//
// Covers: std::numeric_limits traits

#include "learn/topic_registry.hpp"

#include <cassert>
#include <limits>

namespace {

void demo_basics() {
    assert(std::numeric_limits<int>::is_integer);
    assert(std::numeric_limits<int>::min() < 0);
    assert(std::numeric_limits<int>::max() > 0);
}

void demo_intermediate() {
    assert(std::numeric_limits<unsigned>::min() == 0u);
    assert(std::numeric_limits<float>::epsilon() > 0.0f);
    assert(std::numeric_limits<double>::digits10 >= 15);
}

void demo_expert() {
    static_assert(std::numeric_limits<bool>::digits == 1);
    assert(std::numeric_limits<int>::radix == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numeric_limits", run>;

}  // namespace
