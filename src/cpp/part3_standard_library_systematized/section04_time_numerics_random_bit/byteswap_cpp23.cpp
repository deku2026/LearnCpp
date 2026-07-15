// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.4 byteswap (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : byteswap_cpp23
// Topic id : part3/section04/byteswap_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/numeric/byteswap
//   Feature-test: __cpp_lib_byteswap >= 202110L

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

#if !defined(__cpp_lib_byteswap) || __cpp_lib_byteswap < 202110L
constexpr std::uint32_t byteswap32(std::uint32_t x) noexcept {
    return ((x & 0x000000FFu) << 24) | ((x & 0x0000FF00u) << 8) | ((x & 0x00FF0000u) >> 8) | ((x & 0xFF000000u) >> 24);
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/byteswap_cpp23 ===\n";

    const std::uint32_t n = 0x12345678u;
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    const std::uint32_t swapped = std::byteswap(n);
    assert(swapped == 0x78563412u);
    assert(std::byteswap(swapped) == n);
    assert(std::byteswap(std::uint16_t{0xABCDu}) == 0xCDABu);
    assert(std::byteswap(std::uint8_t{0xABu}) == 0xABu);
    std::cout << "[intro] byteswap(0x12345678)=0x" << std::hex << swapped << std::dec << '\n';
#else
    const std::uint32_t swapped = byteswap32(n);
    assert(swapped == 0x78563412u);
    std::cout << "[intro] std::byteswap unavailable; hand-rolled 0x" << std::hex << swapped << std::dec << '\n';
#endif

    // 网络序/主机序场景: 结合 endian
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "[advanced] on little-endian host, byteswap converts "
                     "to/from big-endian wire form\n";
    }

    std::cout << "[expert] use for portable binary protocols, not for float "
                 "bit-twiddling (prefer bit_cast)\n";
    std::cout << "byteswap_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/byteswap_cpp23", run>;

}  // namespace
