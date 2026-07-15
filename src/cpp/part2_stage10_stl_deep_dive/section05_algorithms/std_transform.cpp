// Topic     : std::transform —— 映射 / 二元变换
// Doc       : 第2部分-阶段10 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/algorithm/transform
//
// 要点: 一元/二元 overload; 可 in-place; 输出可用 back_inserter。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_transform] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

    // 一元: 映射到新容器
    std::vector<int> squared;
    std::transform(v.begin(), v.end(), std::back_inserter(squared), [](int x) { return x * x; });
    assert((squared == std::vector<int>{1, 4, 9, 16, 25}));

    // 原地变换
    std::vector<int> doubled = v;
    std::transform(doubled.begin(), doubled.end(), doubled.begin(), [](int x) { return x * 2; });
    assert((doubled == std::vector<int>{2, 4, 6, 8, 10}));

    // 二元: 对应位置相加
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};
    std::vector<int> sum(3);
    std::transform(a.begin(), a.end(), b.begin(), sum.begin(), std::plus<>{});
    assert((sum == std::vector<int>{11, 22, 33}));

    // 类型转换
    std::vector<std::string> words{"alpha", "beta", "gamma"};
    std::vector<std::size_t> lens;
    std::transform(words.begin(), words.end(), std::back_inserter(lens), [](const std::string& s) { return s.size(); });
    assert((lens == std::vector<std::size_t>{5, 4, 5}));

    // ranges::transform (C++20)
    std::vector<int> cubes;
    std::ranges::transform(v, std::back_inserter(cubes), [](int x) { return x * x * x; });
    assert(cubes[2] == 27);

    std::cout << "[transform] unary/binary/in-place/ranges OK\n";
    std::cout << "std_transform: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_transform", run>;

}  // namespace
