// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_byte_cpp17
// Topic id : part2/stage10/section06_modern_utility_types/std_byte_cpp17
//
// Covers: std::byte type-safe byte C++17

#include "learn/topic_registry.hpp"

#include <cstddef>

namespace {

void demo_basics() {
    std::byte b{std::byte{0x0F}};
    LEARN_CHECK(std::to_integer<int>(b) == 15);
}

void demo_intermediate() {
    std::byte b{std::byte{0b0001}};
    b <<= 3;
    LEARN_CHECK(std::to_integer<int>(b) == 0b1000);
    b |= std::byte{0b0111};
    LEARN_CHECK(std::to_integer<int>(b) == 0b1111);
}

void demo_expert() {
    std::byte a{std::byte{0xF0}};
    std::byte b{std::byte{0x0F}};
    LEARN_CHECK(std::to_integer<int>(a & b) == 0);
    LEARN_CHECK(std::to_integer<int>(a | b) == 0xFF);
    LEARN_CHECK(std::to_integer<int>(a ^ b) == 0xFF);
    LEARN_CHECK(std::to_integer<unsigned>(~std::byte{0}) == 0xFFu);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_byte_cpp17", run>;

}  // namespace
