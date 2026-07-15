// Topic     : std::byteswap —— 字节序翻转 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.5
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_byteswap_cpp23
// Topic id  : part2/stage10/section05/std_byteswap_cpp23
// Refs      : https://en.cppreference.com/w/cpp/numeric/byteswap
//             ISO [bit.byteswap]

#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <iostream>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_byteswap_cpp23] 入门：u16/u32/u64 翻转 ===\n";
    {
        constexpr auto u32 = std::byteswap(std::uint32_t{0x12345678});
        static_assert(u32 == 0x78563412u);
        assert(u32 == 0x78563412u);

        constexpr auto u16 = std::byteswap(std::uint16_t{0xABCDu});
        static_assert(u16 == 0xCDABu);
        assert(u16 == 0xCDABu);

        constexpr auto u64 = std::byteswap(std::uint64_t{0x0123456789ABCDEFull});
        static_assert(u64 == 0xEFCDAB8967452301ull);
        assert(u64 == 0xEFCDAB8967452301ull);

        // 单字节：无变化
        assert(std::byteswap(std::uint8_t{0x5A}) == 0x5A);
        std::cout << "0x12345678 -> 0x" << std::hex << u32 << std::dec << '\n';
    }

    std::cout << "=== 进阶：有符号 + 往返幂等 ===\n";
    {
        [[maybe_unused]] const auto s = std::byteswap(std::int32_t{0x10203040});
        assert(static_cast<std::uint32_t>(s) == 0x40302010u);

        [[maybe_unused]] const std::uint32_t x = 0xDEADBEEFu;
        assert(std::byteswap(std::byteswap(x)) == x);

        // 全 0 / 全 1 自反
        assert(std::byteswap(std::uint32_t{0}) == 0);
        assert(std::byteswap(std::uint32_t{0xFFFFFFFFu}) == 0xFFFFFFFFu);
        std::cout << "signed + involution OK\n";
    }

    std::cout << "=== 专家：网络/文件 endian 场景 + 与 endian 探测 ===\n";
    {
        // 教学：从大端 buffer 读 u32（网络序常为 big-endian）
        const std::uint32_t be_on_wire = 0x12345678u;  // 假设已是「内存里的大端布局整型」
        // 若本机小端，要把「按大端解释的整型」翻成主机序：
        if constexpr (std::endian::native == std::endian::little) {
            [[maybe_unused]] const auto host = std::byteswap(be_on_wire);
            assert(host == 0x78563412u);
            std::cout << "native=little: byteswap converts big-endian wire value\n";
        } else if constexpr (std::endian::native == std::endian::big) {
            // 本机大端：wire 值无需翻转
            assert(be_on_wire == 0x12345678u);
            std::cout << "native=big: no swap needed for big-endian wire\n";
        }

#if defined(__cpp_lib_byteswap)
        std::cout << "__cpp_lib_byteswap=" << __cpp_lib_byteswap << '\n';
#endif
        // 与手动移位等价（u16）
        const std::uint16_t w = 0xA1B2u;
        [[maybe_unused]] const auto manual = static_cast<std::uint16_t>(((w & 0xFFu) << 8) | (w >> 8));
        assert(std::byteswap(w) == manual);
        std::cout << "endian-aware teaching path OK\n";
    }

    std::cout << "[std_byteswap_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_byteswap_cpp23", run>;

}  // namespace
