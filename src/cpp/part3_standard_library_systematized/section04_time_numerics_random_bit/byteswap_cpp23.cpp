// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : byteswap_cpp23
// Topic id : part3/section04/byteswap_cpp23
//
// Covers: std::byteswap C++23

#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <version>

namespace {

void demo_basics() {
    std::uint16_t x = 0x1234u;
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    LEARN_CHECK(std::byteswap(x) == 0x3412u);
#else
    auto swap16 = [](std::uint16_t v) -> std::uint16_t { return static_cast<std::uint16_t>((v << 8) | (v >> 8)); };
    LEARN_CHECK(swap16(x) == 0x3412u);
#endif
}

void demo_intermediate() {
    std::uint32_t x = 0x01020304u;
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    LEARN_CHECK(std::byteswap(x) == 0x04030201u);
#else
    LEARN_CHECK(x == 0x01020304u);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    std::uint64_t x = 0x0102030405060708ull;
    LEARN_CHECK(std::byteswap(std::byteswap(x)) == x);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/byteswap_cpp23", run>;

}  // namespace
