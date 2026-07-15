// Topic     : 定宽整型 <cstdint>
// Doc       : 第2部分-阶段2 · 步骤 1.2
// cppreference: https://en.cppreference.com/cpp/types/integer
//               https://en.cppreference.com/cpp/header/cstdint
//
// 要点: 需要“正好 N 位 / 至少 N 位 / 能装指针”时用 <cstdint>；
//       intN_t 精确宽度；int_leastN_t / int_fastN_t 至少宽度；intptr_t 装指针。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [fixed_width_integers_cstdint] ===\n";

    // -------------------------------------------------------------------------
    // 入门：精确宽度
    // -------------------------------------------------------------------------
    std::int32_t exactly32 = -1;
    std::uint64_t exactly64 = 0;
    exactly64 = static_cast<std::uint64_t>(1) << 40;

    static_assert(sizeof(std::int8_t) == 1);
    static_assert(sizeof(std::int16_t) == 2);
    static_assert(sizeof(std::int32_t) == 4);
    static_assert(sizeof(std::int64_t) == 8);
    static_assert(sizeof(std::uint32_t) == 4);
    static_assert(sizeof(std::uint64_t) == 8);

    assert(exactly32 == -1);
    assert(exactly64 == (std::uint64_t{1} << 40));
    std::cout << "[intro] int32_t=" << exactly32 << " uint64_t(1<<40)=" << exactly64 << '\n';

    // -------------------------------------------------------------------------
    // 进阶：least / fast / max / pointer-sized
    // -------------------------------------------------------------------------
    std::int_least16_t least16 = 0x7FFF;
    std::int_fast16_t fast16 = 42;  // 至少 16 位，本平台“最快”实现
    std::intmax_t biggest = std::numeric_limits<std::intmax_t>::max();
    (void)least16;
    (void)fast16;

    // 能容纳对象指针的整数类型（用于指针标记、序列化偏移等）
    int stack_obj = 7;
    [[maybe_unused]] std::intptr_t ip = reinterpret_cast<std::intptr_t>(&stack_obj);
    [[maybe_unused]] std::uintptr_t up = reinterpret_cast<std::uintptr_t>(&stack_obj);
    assert(reinterpret_cast<int*>(ip) == &stack_obj);
    assert(reinterpret_cast<int*>(up) == &stack_obj);
    assert(*reinterpret_cast<int*>(ip) == 7);

    static_assert(sizeof(std::intptr_t) >= sizeof(void*));
    static_assert(sizeof(std::uintptr_t) >= sizeof(void*));

    std::cout << "[advanced] sizeof(int_fast16_t)=" << sizeof(std::int_fast16_t)
              << " sizeof(intmax_t)=" << sizeof(std::intmax_t) << " intptr ok\n";
    std::cout << "[advanced] intmax_t max=" << biggest << '\n';

    // -------------------------------------------------------------------------
    // 专家：与 plain int 的关系、可移植策略
    // -------------------------------------------------------------------------
    // 1) int32_t 等是 typedef，可能与 int / long 同型，也可能不同——别用 is_same 假定
    // 2) 精确宽度类型在不支持的平台可不存在；主流桌面/服务器都有
    // 3) 协议/文件格式/跨语言 ABI：优先精确宽度；本地循环计数：size_t / 普通 int 亦可
    // 4) 算术溢出：有符号溢出仍是 UB；无符号按模 2^N 回绕

    [[maybe_unused]] std::uint8_t wrap = 255;
    ++wrap;  // 无符号回绕 → 0
    assert(wrap == 0);

    // 有符号：不要依赖“溢出后变成负”；用更大宽度或检查
    [[maybe_unused]] std::int32_t near_max = std::numeric_limits<std::int32_t>::max() - 1;
    near_max += 1;
    assert(near_max == std::numeric_limits<std::int32_t>::max());
    // near_max += 1; // 若再加 → UB，禁止演示执行

    // 打印底层“可能等于谁”
    std::cout << "[expert] is_same int32_t,int? " << std::boolalpha
              << std::is_same_v<std::int32_t, int> << "  int32_t,long? " << std::is_same_v<std::int32_t, long> << '\n';

    // 位操作示例：打包两个 u16 到 u32
    std::uint16_t hi = 0xABCD;
    std::uint16_t lo = 0x1234;
    std::uint32_t packed = (static_cast<std::uint32_t>(hi) << 16) | static_cast<std::uint32_t>(lo);
    assert(packed == 0xABCD1234u);
    std::cout << "[expert] packed=0x" << std::hex << packed << std::dec << '\n';

    std::cout << "=== fixed_width_integers_cstdint: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/fixed_width_integers_cstdint", run>;

}  // namespace
