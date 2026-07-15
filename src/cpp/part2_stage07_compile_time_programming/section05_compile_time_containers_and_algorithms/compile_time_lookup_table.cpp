// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_lookup_table
// Topic id : part2/stage07/section05/compile_time_lookup_table
//
// Refs:
//   https://en.cppreference.com/w/cpp/container/array
//   文档步骤 6.4：编译期生成查找表，运行期 O(1) 查

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 编译期生成平方表
// ---------------------------------------------------------------------------

constexpr auto make_square_table() {
    std::array<int, 256> table{};
    for (int i = 0; i < 256; ++i) {
        table[static_cast<std::size_t>(i)] = i * i;
    }
    return table;
}

constexpr auto kSquareTable = make_square_table();

// ---------------------------------------------------------------------------
// §进阶 — 质数标记表 / CRC 风格表 / 多维信息
// ---------------------------------------------------------------------------

constexpr bool is_prime(int n) {
    if (n < 2) {
        return false;
    }
    for (int d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

constexpr auto make_prime_flags() {
    std::array<bool, 100> flags{};
    for (int i = 0; i < 100; ++i) {
        flags[static_cast<std::size_t>(i)] = is_prime(i);
    }
    return flags;
}

constexpr auto kPrimeFlags = make_prime_flags();

// 编译期收集前 N 个质数到紧凑表
constexpr auto make_first_primes() {
    std::array<int, 25> primes{};
    int count = 0;
    for (int i = 2; count < 25; ++i) {
        if (is_prime(i)) {
            primes[static_cast<std::size_t>(count++)] = i;
        }
    }
    return primes;
}

constexpr auto kFirstPrimes = make_first_primes();

// 简单「字节 popcount」表
constexpr auto make_popcount_table() {
    std::array<std::uint8_t, 256> t{};
    for (int i = 0; i < 256; ++i) {
        int v = i;
        int c = 0;
        while (v) {
            c += v & 1;
            v >>= 1;
        }
        t[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(c);
    }
    return t;
}

constexpr auto kPop = make_popcount_table();

// ---------------------------------------------------------------------------
// §专家 — 设计要点
// ---------------------------------------------------------------------------
// 1) 表类型用 std::array / 原始数组，避免 transient allocation 泄漏问题。
// 2) 用 constexpr 变量保存表 → 数据进入只读段，运行期零计算。
// 3) godbolt 上常可见 mov eax, imm 而不是计算循环。
// 4) 表太大影响编译时间/二进制体积，需权衡。
// 5) C++20 起也可先用 vector 工作区再拷进 array（见 compile_time_sort）。

constexpr int popcount32(std::uint32_t x) {
    return kPop[x & 0xff] + kPop[(x >> 8) & 0xff] + kPop[(x >> 16) & 0xff] + kPop[(x >> 24) & 0xff];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/compile_time_lookup_table ===\n";

    static_assert(kSquareTable[0] == 0);
    static_assert(kSquareTable[15] == 225);
    static_assert(kSquareTable[255] == 65025);
    static_assert(kPrimeFlags[2]);
    static_assert(kPrimeFlags[4] == false);
    static_assert(kFirstPrimes[0] == 2);
    static_assert(kFirstPrimes[1] == 3);
    static_assert(kFirstPrimes[24] == 97);
    static_assert(kPop[7] == 3);
    static_assert(popcount32(0xF0F0F0F0u) == 16);

    // 运行期查表
    int idx = 15;
    assert(kSquareTable[static_cast<std::size_t>(idx)] == 225);
    std::cout << "[intro] kSquareTable[15]=" << kSquareTable[15] << " (table built at compile time)\n";

    std::cout << "[advanced] first primes: ";
    for (std::size_t i = 0; i < 8; ++i) {
        std::cout << kFirstPrimes[i] << (i + 1 < 8 ? "," : "");
    }
    std::cout << ",...\n";

    std::uint32_t sample = 0xABCDu;
    std::cout << "[expert] popcount32(0xABCD)=" << popcount32(sample) << " via 256-entry constexpr table\n";

    std::cout << "compile_time_lookup_table: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_lookup_table", run>;

}  // namespace
