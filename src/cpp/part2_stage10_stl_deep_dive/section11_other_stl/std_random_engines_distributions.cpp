// Topic    : <random> 引擎 + 分布 — 别用 rand()
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_random_engines_distributions
// Topic id : part2/stage10/section11/std_random_engines_distributions
// Refs     : https://en.cppreference.com/w/cpp/numeric/random
//            https://en.cppreference.com/w/cpp/numeric/random/mt19937
//            https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_random_engines_distributions] engines+dist ===\n";

    // ① 确定性引擎: 固定种子 → 可复现序列(测试友好)
    {
        std::mt19937 eng{42};
        std::uniform_int_distribution<int> dist{1, 6};  // 骰子
        std::vector<int> rolls;
        for (int i = 0; i < 10; ++i) {
            rolls.push_back(dist(eng));
        }
        for (int r : rolls) {
            (void)r;
            assert(r >= 1 && r <= 6);
        }
        // 同种子应复现
        std::mt19937 eng2{42};
        std::uniform_int_distribution<int> dist2{1, 6};
        for (int i = 0; i < 10; ++i) {
            assert(dist2(eng2) == rolls[static_cast<std::size_t>(i)]);
        }
        std::cout << "  mt19937 seed=42 reproducible dice:";
        for (int r : rolls) {
            std::cout << ' ' << r;
        }
        std::cout << '\n';
    }

    // ② 真实熵种子(非确定性)
    {
        std::random_device rd;
        std::mt19937 eng{rd()};
        std::uniform_real_distribution<double> unit{0.0, 1.0};
        const double x = unit(eng);
        assert(x >= 0.0 && x < 1.0);
        std::cout << "  random_device sample u01=" << x << '\n';
    }

    // ③ 正态分佈: 均值/方差粗检
    {
        std::mt19937 eng{7};
        std::normal_distribution<double> normal{0.0, 1.0};
        std::vector<double> samples;
        samples.reserve(2000);
        for (int i = 0; i < 2000; ++i) {
            samples.push_back(normal(eng));
        }
        const double mean = std::accumulate(samples.begin(), samples.end(), 0.0) / static_cast<double>(samples.size());
        assert(std::abs(mean) < 0.15);  // 大样本均值接近 0
        std::cout << "  normal(0,1) sample mean≈" << mean << '\n';
    }

    // ④ bernoulli / 洗牌
    {
        std::mt19937 eng{99};
        std::bernoulli_distribution coin{0.5};
        int heads = 0;
        for (int i = 0; i < 200; ++i) {
            heads += coin(eng) ? 1 : 0;
        }
        assert(heads > 40 && heads < 160);
        std::cout << "  bernoulli heads/200=" << heads << '\n';

        std::vector<int> v{1, 2, 3, 4, 5};
        std::shuffle(v.begin(), v.end(), eng);
        assert(v.size() == 5);
        assert(std::is_permutation(v.begin(), v.end(), std::vector<int>{1, 2, 3, 4, 5}.begin()));
        std::cout << "  shuffle OK\n";
    }

    // 教学: C rand() 质量差、全局状态、分布有偏 → 用引擎+分布
    std::cout << "[std_random_engines_distributions] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/std_random_engines_distributions", run>;

}  // namespace
