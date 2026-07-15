// Topic     : std::sample —— 从序列随机采样 n 个
// Doc       : 第2部分-阶段10 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/algorithm/sample
//
// 要点: C++17; 输出保持相对顺序(若输入至少 Forward); 需 URBG。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <set>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_sample] ===\n";

    std::vector<int> population(20);
    std::iota(population.begin(), population.end(), 1);  // 1..20

    std::mt19937 rng{42};  // 固定种子, 可复现
    std::vector<int> out;
    std::sample(population.begin(), population.end(), std::back_inserter(out), 5, rng);

    assert(out.size() == 5);
    // 采样结果应是原集合子集, 且相对顺序非降(Forward 输入时稳定相对序)
    assert(std::is_sorted(out.begin(), out.end()));
    for ([[maybe_unused]] auto x : out) {
        assert(x >= 1 && x <= 20);
    }
    // 无重复
    assert(std::set<int>(out.begin(), out.end()).size() == out.size());

    std::cout << "[sample] 5 from 1..20:";
    for ([[maybe_unused]] auto x : out) std::cout << ' ' << x;
    std::cout << '\n';

    // n > size → 取全部
    std::vector<int> small{10, 20, 30};
    std::vector<int> all;
    std::sample(small.begin(), small.end(), std::back_inserter(all), 100, rng);
    assert(all.size() == 3);

    // n == 0 → 空
    std::vector<int> empty_out;
    std::sample(population.begin(), population.end(), std::back_inserter(empty_out), 0, rng);
    assert(empty_out.empty());

    std::cout << "std_sample: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_sample", run>;

}  // namespace
