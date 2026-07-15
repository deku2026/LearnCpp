// Topic    : <bit> 位操作 — popcount / rotl / bit_width / byteswap...
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_operations_cpp20
// Topic id : part2/stage10/section11/std_bit_operations_cpp20
// Refs     : https://en.cppreference.com/w/cpp/header/bit
//            https://en.cppreference.com/w/cpp/numeric/popcount
//            https://en.cppreference.com/w/cpp/numeric/byteswap

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_bit_operations_cpp20] <bit> toolkit ===\n";

    // ① popcount: 置位数
    {
        assert(std::popcount(0u) == 0);
        assert(std::popcount(0b1011u) == 3);
        assert(std::popcount(0xFFu) == 8);
        std::cout << "  popcount(0b1011)=" << std::popcount(0b1011u) << '\n';
    }

    // ② countl_zero / countr_zero
    {
        const std::uint8_t x = 0b0001'1000;
        assert(std::countl_zero(x) == 3);  // 前导 0
        assert(std::countr_zero(x) == 3);  // 尾随 0
        std::cout << "  countl/r_zero on 0b00011000\n";
    }

    // ③ bit_width / bit_floor / bit_ceil / has_single_bit
    {
        assert(std::bit_width(0u) == 0);
        assert(std::bit_width(1u) == 1);
        assert(std::bit_width(5u) == 3);  // 101 → 宽度 3
        assert(std::has_single_bit(8u));
        assert(!std::has_single_bit(12u));
        assert(std::bit_floor(5u) == 4u);
        assert(std::bit_ceil(5u) == 8u);
        std::cout << "  bit_width/floor/ceil/has_single_bit OK\n";
    }

    // ④ rotl / rotr
    {
        const std::uint8_t x = 0b1000'0001;
        assert(std::rotl(x, 1) == 0b0000'0011);
        assert(std::rotr(x, 1) == 0b1100'0000);
        std::cout << "  rotl/rotr OK\n";
    }

    // ⑤ C++23 byteswap
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    {
        assert(std::byteswap(std::uint16_t{0x1234}) == 0x3412);
        assert(std::byteswap(std::uint32_t{0x01020304}) == 0x04030201u);
        std::cout << "  byteswap(C++23) OK\n";
    }
#else
    {
        const auto bswap16 = [](std::uint16_t v) -> std::uint16_t {
            return static_cast<std::uint16_t>((v << 8) | (v >> 8));
        };
        assert(bswap16(0x1234) == 0x3412);
        std::cout << "  (no std::byteswap) manual 16-bit swap OK\n";
    }
#endif

    // ⑥ endian
    {
        if constexpr (std::endian::native == std::endian::little) {
            std::cout << "  endian: little\n";
        } else if constexpr (std::endian::native == std::endian::big) {
            std::cout << "  endian: big\n";
        } else {
            std::cout << "  endian: mixed/other\n";
        }
    }

    std::cout << "[std_bit_operations_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/std_bit_operations_cpp20", run>;

}  // namespace
