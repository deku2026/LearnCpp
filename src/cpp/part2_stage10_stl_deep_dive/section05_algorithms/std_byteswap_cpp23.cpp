// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_byteswap_cpp23
// Topic id : part2/stage10/section05_algorithms/std_byteswap_cpp23
//
// Covers: std::byteswap C++23 byte order reverse

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <version>

#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
#include <bit>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    assert(std::byteswap(std::uint16_t{0x1234}) == std::uint16_t{0x3412});
#else
    auto bs16 = [](std::uint16_t x) -> std::uint16_t { return static_cast<std::uint16_t>((x << 8) | (x >> 8)); };
    assert(bs16(0x1234) == 0x3412);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    assert(std::byteswap(std::uint32_t{0x12345678}) == std::uint32_t{0x78563412});
#else
    auto bs32 = [](std::uint32_t x) -> std::uint32_t {
        return ((x & 0x000000FFu) << 24) | ((x & 0x0000FF00u) << 8) | ((x & 0x00FF0000u) >> 8) |
               ((x & 0xFF000000u) >> 24);
    };
    assert(bs32(0x12345678u) == 0x78563412u);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    const std::uint64_t x = 0x0123456789ABCDEFull;
    const auto y = std::byteswap(x);
    assert(std::byteswap(y) == x);
#else
    assert(true);
#endif
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_byteswap_cpp23", run>;

}  // namespace
