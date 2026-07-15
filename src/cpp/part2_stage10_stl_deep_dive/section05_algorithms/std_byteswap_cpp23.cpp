// Topic     : std::byteswap —— 字节序翻转 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/numeric/byteswap
//
// 要点: <bit>; 对整型逐字节反转; 常用于网络/文件 endian 转换。

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_byteswap_cpp23] ===\n";

    constexpr auto u32 = std::byteswap(std::uint32_t{0x12345678});
    static_assert(u32 == 0x78563412u);
    assert(u32 == 0x78563412u);

    constexpr auto u16 = std::byteswap(std::uint16_t{0xABCDu});
    static_assert(u16 == 0xCDABu);
    assert(u16 == 0xCDABu);

    constexpr auto u64 = std::byteswap(std::uint64_t{0x0123456789ABCDEFull});
    static_assert(u64 == 0xEFCDAB8967452301ull);
    assert(u64 == 0xEFCDAB8967452301ull);

    // 单字节: 无变化
    assert(std::byteswap(std::uint8_t{0x5A}) == 0x5A);

    // 有符号: 按底层对象表示反转
    const auto s = std::byteswap(std::int32_t{0x10203040});
    assert(static_cast<std::uint32_t>(s) == 0x40302010u);

    // 往返
    const std::uint32_t x = 0xDEADBEEFu;
    assert(std::byteswap(std::byteswap(x)) == x);

    std::cout << "[byteswap] 0x12345678 -> 0x" << std::hex << u32 << std::dec << '\n';
    std::cout << "std_byteswap_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_byteswap_cpp23", run>;

}  // namespace
