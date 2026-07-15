// Topic     : std::byte —— 类型安全的字节 (C++17)
// Doc       : 第2部分-阶段10 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/types/byte
//
// 要点: 不是数字(不能算术); 只位运算; to_integer 转回整型。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_byte_cpp17] ===\n";

    std::byte b{0x0F};
    assert(std::to_integer<int>(b) == 0x0F);

    // 位运算
    b <<= 4;
    assert(std::to_integer<int>(b) == 0xF0);
    b |= std::byte{0x0A};
    assert(std::to_integer<int>(b) == 0xFA);
    b &= std::byte{0xF0};
    assert(std::to_integer<int>(b) == 0xF0);
    b ^= std::byte{0xFF};
    assert(std::to_integer<int>(b) == 0x0F);
    b = ~b;
    assert(std::to_integer<int>(b) == 0xF0);

    // 原始缓冲区语义
    std::vector<std::byte> buf(4);
    buf[0] = std::byte{0xDE};
    buf[1] = std::byte{0xAD};
    buf[2] = std::byte{0xBE};
    buf[3] = std::byte{0xEF};
    assert(std::to_integer<unsigned>(buf[0]) == 0xDE);

    // ❌ 不能: b + b; b * 2;  —— 不是算术类型
    // ✅ 与 unsigned char 的区别: 意图是"原始内存字节"而非字符/小整数

    static_assert(sizeof(std::byte) == 1);

    std::cout << "[byte] shift/or/and/xor/not + buffer OK\n";
    std::cout << "std_byte_cpp17: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_byte_cpp17", run>;

}  // namespace
