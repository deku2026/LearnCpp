// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_operations_cpp20
// Topic id : part3/section04/bit_operations_cpp20
//
// Covers: std::bit_width, popcount, rotl, rotr

#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>

namespace {

void demo_basics() {
    LEARN_CHECK(std::popcount(0b1011u) == 3);
    LEARN_CHECK(std::has_single_bit(8u));
    LEARN_CHECK(!std::has_single_bit(6u));
}

void demo_intermediate() {
    LEARN_CHECK(std::bit_width(5u) == 3);
    LEARN_CHECK(std::countl_zero(0b0001'0000u) >= 0);
    LEARN_CHECK(std::countr_zero(0b1000u) == 3);
}

void demo_expert() {
    std::uint32_t x = 0x12345678u;
    auto y = std::rotl(x, 8);
    auto z = std::rotr(y, 8);
    LEARN_CHECK(z == x);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/bit_operations_cpp20", run>;

}  // namespace
