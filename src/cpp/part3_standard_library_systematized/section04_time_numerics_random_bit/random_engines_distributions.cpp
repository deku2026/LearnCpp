// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.2 <random>
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : random_engines_distributions
// Topic id : part3/section04/random_engines_distributions
//
// Refs:
//   https://en.cppreference.com/w/cpp/numeric/random
//
// 要点: 引擎+分布两段式; random_device 播种; 复用引擎; 可复现用固定种子。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <random>
#include <set>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/random_engines_distributions ===\n";

    // 可复现: 固定种子
    std::mt19937 eng{42};
    std::uniform_int_distribution<int> die(1, 6);
    const int first = die(eng);
    assert(first >= 1 && first <= 6);

    // 同一引擎继续产生序列
    std::set<int> seen;
    for (int i = 0; i < 60; ++i) {
        seen.insert(die(eng));
    }
    assert(seen.size() >= 4);  // 高概率覆盖多个面

    std::cout << "[intro] mt19937 + uniform_int_distribution first=" << first << '\n';

    // 真实播种路径(教学质量平台相关)
    std::random_device rd;
    std::mt19937 eng2{rd()};
    std::uniform_real_distribution<double> unit(0.0, 1.0);
    const double u = unit(eng2);
    assert(u >= 0.0 && u < 1.0);

    std::normal_distribution<double> gauss(0.0, 1.0);
    const double g = gauss(eng2);
    (void)g;
    std::cout << "[advanced] normal sample=" << g << " uniform=" << u << '\n';

    // 伯努利 / 伯努利试验
    std::bernoulli_distribution coin(0.5);
    [[maybe_unused]] int heads = 0;
    for (int i = 0; i < 100; ++i) {
        heads += coin(eng2) ? 1 : 0;
    }
    assert(heads >= 0 && heads <= 100);

    // 固定种子可复现
    std::mt19937 a{7}, b{7};
    std::uniform_int_distribution<int> d(0, 1000);
    assert(d(a) == d(b));

    std::cout << "[expert] never re-seed every iteration; avoid std::rand\n";
    std::cout << "random_engines_distributions: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/random_engines_distributions", run>;

}  // namespace
