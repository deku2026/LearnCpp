// Topic     : std::gcd / std::lcm —— 最大公约 / 最小公倍
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.5
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_gcd_lcm
// Topic id  : part2/stage10/section05/std_gcd_lcm
// Refs      : https://en.cppreference.com/w/cpp/numeric/gcd
//             https://en.cppreference.com/w/cpp/numeric/lcm

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_gcd_lcm] 入门：gcd / lcm 基本 ===\n";
    {
        assert(std::gcd(12, 18) == 6);
        assert(std::gcd(17, 13) == 1);
        assert(std::gcd(0, 5) == 5);
        assert(std::gcd(0, 0) == 0);

        assert(std::lcm(4, 6) == 12);
        assert(std::lcm(3, 5) == 15);
        assert(std::lcm(0, 7) == 0);
        // 关系：gcd(a,b) * lcm(a,b) == |a*b|（无溢出时）
        assert(std::gcd(12, 18) * std::lcm(12, 18) == 12 * 18);
        std::cout << "basic gcd/lcm OK\n";
    }

    std::cout << "=== 进阶：负数绝对值 + 约分 + 多宽度 ===\n";
    {
        assert(std::gcd(-12, 18) == 6);
        assert(std::gcd(-12, -18) == 6);
        assert(std::lcm(-4, 6) == 12);

        int num = 42, den = 56;
        const int g = std::gcd(num, den);
        num /= g;
        den /= g;
        assert(num == 3 && den == 4);

        const auto g2 = std::gcd(std::int64_t{100}, 30);
        assert(g2 == 10);
        std::cout << "signed + reduce fraction OK\n";
    }

    std::cout << "=== 专家：多参数折叠 lcm + 溢出注意 ===\n";
    {
        // 多周期：折叠 lcm
        const int period = std::lcm(std::lcm(2, 3), 4);
        assert(period == 12);

        std::vector<int> periods{2, 3, 4, 6};
        int fold = 1;
        for (int p : periods) fold = std::lcm(fold, p);
        assert(fold == 12);

        // ⚠️ lcm 可能溢出中间 |a/gcd*b|；大数场景用宽类型
        const auto big = std::lcm(std::int64_t{1000003}, std::int64_t{1000033});
        assert(big > 0);

        std::cout << "fold lcm + wide type OK\n";
    }

    std::cout << "[std_gcd_lcm] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_gcd_lcm", run>;

}  // namespace
