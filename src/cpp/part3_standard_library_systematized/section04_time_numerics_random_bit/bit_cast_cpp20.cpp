// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_cast_cpp20
// Topic id : part3/section04/bit_cast_cpp20
//
// Covers: std::bit_cast type punning safely

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>

namespace {

void demo_basics() {
    float f = 1.0f;
    auto u = std::bit_cast<std::uint32_t>(f);
    auto back = std::bit_cast<float>(u);
    assert(back == 1.0f);
}

void demo_intermediate() {
    std::uint32_t bits = 0u;
    float z = std::bit_cast<float>(bits);
    assert(z == 0.0f);
}

void demo_expert() {
    struct A {
        std::uint16_t a;
        std::uint16_t b;
    };
    A x{1, 2};
    auto y = std::bit_cast<std::uint32_t>(x);
    auto z = std::bit_cast<A>(y);
    assert(z.a == 1 && z.b == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/bit_cast_cpp20", run>;

}  // namespace
