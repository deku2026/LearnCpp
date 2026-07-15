// Topic    : std::bit_cast — 安全类型双关(C++20)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2 / 阶段 9 回响
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_cast_cpp20
// Topic id : part2/stage10/section11/std_bit_cast_cpp20
// Refs     : https://en.cppreference.com/w/cpp/numeric/bit_cast
//            https://en.cppreference.com/w/cpp/header/bit

#include "learn/topic_registry.hpp"

#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_bit_cast_cpp20] safe type punning ===\n";

    // ① float ↔ uint32_t 位型重解释(同大小、可平凡复制)
    {
        const float f = 1.0f;
        const std::uint32_t u = std::bit_cast<std::uint32_t>(f);
        // IEEE-754 binary32: 1.0 = 0x3f800000
        assert(u == 0x3f800000u);
        const float back = std::bit_cast<float>(u);
        assert(back == 1.0f);
        std::cout << "  1.0f bits=0x" << std::hex << u << std::dec << '\n';
    }

    // ② 对比 memcpy 老写法 — bit_cast 可 constexpr
    {
        constexpr std::uint32_t bits = 0x3f800000u;
        constexpr float f = std::bit_cast<float>(bits);
        static_assert(f == 1.0f);
        std::cout << "  constexpr bit_cast OK\n";
    }

    // ③ 整数端序探查(实现定义字节序)
    {
        const std::uint32_t x = 0x01020304u;
        const auto bytes = std::bit_cast<std::array<std::uint8_t, 4>>(x);
        std::cout << "  0x01020304 bytes:";
        for (auto b : bytes) {
            std::cout << ' ' << static_cast<int>(b);
        }
        std::cout << '\n';
        // 小端: 4 3 2 1; 大端: 1 2 3 4
        assert(bytes.size() == 4);
    }

    // ④ 与 UB 双关对比教学(勿写):
    // float f=1; auto u = *(uint32_t*)&f;  // 严格别名 UB
    // bit_cast / memcpy 是合规路径
    {
        const double d = -0.0;
        const auto bits = std::bit_cast<std::uint64_t>(d);
        // 符号位为 1
        assert((bits >> 63) == 1);
        std::cout << "  -0.0 sign bit set\n";
    }

    // ⑤ 尺寸必须相等
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    static_assert(std::is_trivially_copyable_v<float>);

    std::cout << "[std_bit_cast_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/std_bit_cast_cpp20", run>;

}  // namespace
