// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_cast_cpp20
// Topic id : part2/stage10/section11_other_stl/std_bit_cast_cpp20
//
// Covers: std::bit_cast C++20

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>

namespace {

void demo_basics() {
    const float f = 1.0f;
    const auto u = std::bit_cast<std::uint32_t>(f);
    assert(u == 0x3f800000u);
    assert(std::bit_cast<float>(u) == 1.0f);
}

void demo_intermediate() {
    struct A {
        std::uint16_t a;
        std::uint16_t b;
    };
    struct B {
        std::uint32_t ab;
    };
    A x{0x1122, 0x3344};
    // bit_cast requires same size
    static_assert(sizeof(A) == sizeof(B));
    auto y = std::bit_cast<B>(x);
    (void)y;
    assert(sizeof(y) == 4);
}

void demo_expert() {
    const double d = 0.0;
    const auto bits = std::bit_cast<std::uint64_t>(d);
    assert(bits == 0);
    assert(std::bit_cast<double>(bits) == 0.0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/std_bit_cast_cpp20", run>;

}  // namespace
