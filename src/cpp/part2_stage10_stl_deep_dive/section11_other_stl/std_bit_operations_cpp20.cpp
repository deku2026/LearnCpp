// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_operations_cpp20
// Topic id : part2/stage10/section11_other_stl/std_bit_operations_cpp20
//
// Covers: bit ops: countl_zero, popcount, rotl, endian C++20

#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>

namespace {

void demo_basics() {
    LEARN_CHECK(std::popcount(std::uint8_t{0b1011}) == 3);
    LEARN_CHECK(std::countl_zero(std::uint8_t{0b0001'0000}) == 3);
    LEARN_CHECK(std::countr_zero(std::uint8_t{0b0001'0000}) == 4);
}

void demo_intermediate() {
    LEARN_CHECK(std::rotl(std::uint8_t{0b1000'0001}, 1) == std::uint8_t{0b0000'0011});
    LEARN_CHECK(std::rotr(std::uint8_t{0b1000'0001}, 1) == std::uint8_t{0b1100'0000});
    LEARN_CHECK(std::has_single_bit(std::uint32_t{8}));
    LEARN_CHECK(!std::has_single_bit(std::uint32_t{12}));
}

void demo_expert() {
    LEARN_CHECK(std::bit_floor(std::uint32_t{10}) == 8);
    LEARN_CHECK(std::bit_ceil(std::uint32_t{10}) == 16);
    static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/std_bit_operations_cpp20", run>;

}  // namespace
