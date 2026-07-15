// Topic     : std::gcd / std::lcm —— 最大公约 / 最小公倍
// Doc       : 第2部分-阶段10 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/numeric/gcd
//               https://en.cppreference.com/cpp/numeric/lcm
//
// 要点: C++17 <numeric>; 参数可为不同整型; 负数按绝对值处理。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_gcd_lcm] ===\n";

    assert(std::gcd(12, 18) == 6);
    assert(std::gcd(17, 13) == 1);
    assert(std::gcd(0, 5) == 5);
    assert(std::gcd(0, 0) == 0);

    // 负数: 结果非负
    assert(std::gcd(-12, 18) == 6);
    assert(std::gcd(-12, -18) == 6);

    assert(std::lcm(4, 6) == 12);
    assert(std::lcm(3, 5) == 15);
    assert(std::lcm(0, 7) == 0);

    // 约分分数 a/b
    int num = 42, den = 56;
    const int g = std::gcd(num, den);
    num /= g;
    den /= g;
    assert(num == 3 && den == 4);

    // 不同宽度整型
    const auto g2 = std::gcd(std::int64_t{100}, 30);
    assert(g2 == 10);

    // 应用: 找周期最小公倍数
    const int period = std::lcm(std::lcm(2, 3), 4);  // 12
    assert(period == 12);

    std::cout << "[gcd/lcm] 12,18 -> gcd=6 lcm=36; fraction 42/56 -> 3/4\n";
    std::cout << "std_gcd_lcm: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_gcd_lcm", run>;

}  // namespace
