// Topic     : std::transform —— 映射 / 二元变换
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_transform
// Topic id  : part2/stage10/section05/std_transform
// Refs      : https://en.cppreference.com/w/cpp/algorithm/transform

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_transform] 入门：一元映射到新容器 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        std::vector<int> squared;
        std::transform(v.begin(), v.end(), std::back_inserter(squared), [](int x) { return x * x; });
        assert((squared == std::vector<int>{1, 4, 9, 16, 25}));
        std::cout << "unary map OK\n";
    }

    std::cout << "=== 进阶：原地 + 二元 + 类型变换 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        std::vector<int> doubled = v;
        std::transform(doubled.begin(), doubled.end(), doubled.begin(), [](int x) { return x * 2; });
        assert((doubled == std::vector<int>{2, 4, 6, 8, 10}));

        std::vector<int> a{1, 2, 3};
        std::vector<int> b{10, 20, 30};
        std::vector<int> sum(3);
        std::transform(a.begin(), a.end(), b.begin(), sum.begin(), std::plus<>{});
        assert((sum == std::vector<int>{11, 22, 33}));

        std::vector<std::string> words{"alpha", "beta", "gamma"};
        std::vector<std::size_t> lens;
        std::transform(words.begin(), words.end(), std::back_inserter(lens),
                       [](const std::string& s) { return s.size(); });
        assert((lens == std::vector<std::size_t>{5, 4, 5}));
        std::cout << "in-place/binary/type-change OK\n";
    }

    std::cout << "=== 专家：ranges::transform + 与 views::transform 对照 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        std::vector<int> cubes;
        std::ranges::transform(v, std::back_inserter(cubes), [](int x) { return x * x * x; });
        assert(cubes[2] == 27);

        // 算法 transform 立即写输出；views::transform 惰性（见 pipe_transform）
        // 输出迭代器必须有效：预分配或 back_inserter
        std::vector<int> out(v.size());
        std::ranges::transform(v, out.begin(), [](int x) { return -x; });
        assert((out == std::vector<int>{-1, -2, -3, -4, -5}));
        std::cout << "ranges::transform + output policy OK\n";
    }

    std::cout << "[std_transform] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_transform", run>;

}  // namespace
