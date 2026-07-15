// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : extended_floating_types_cpp23
// Topic id : part3/section04/extended_floating_types_cpp23
//
// Covers: extended floating types / float16 notes C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <version>

namespace {

void demo_basics() {
    float f = 1.5f;
    double d = 1.5;
    long double ld = 1.5L;
    assert(f == 1.5f);
    assert(d == 1.5);
    assert(ld == 1.5L);
}

void demo_intermediate() {
    assert(std::numeric_limits<float>::max_exponent >= std::numeric_limits<float>::min_exponent);
#if defined(__STDCPP_FLOAT16_T__)
    std::float16_t h = 1.0f16;
    (void)h;
#endif
}

void demo_expert() {
    // Prefer feature macros before relying on std::float16_t / std::bfloat16_t
    assert(std::isfinite(1.0));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/extended_floating_types_cpp23", run>;

}  // namespace
