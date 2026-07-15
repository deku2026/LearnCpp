// Topic     : std::reduce / transform_reduce —— 可并行折叠
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.3 / 5.4
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_reduce
// Topic id  : part2/stage10/section05/std_reduce
// Refs      : https://en.cppreference.com/w/cpp/algorithm/reduce
//             https://en.cppreference.com/w/cpp/algorithm/transform_reduce

#include "learn/topic_registry.hpp"

#include <cassert>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_reduce] 入门：求和 / 求积 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] const int sum1 = std::reduce(v.begin(), v.end());
        assert(sum1 == 15);
        [[maybe_unused]] const int sum2 = std::reduce(v.begin(), v.end(), 100);
        assert(sum2 == 115);
        [[maybe_unused]] const int prod = std::reduce(v.begin(), v.end(), 1, std::multiplies<>{});
        assert(prod == 120);
        std::cout << "sum/prod OK\n";
    }

    std::cout << "=== 进阶：seq 策略 + transform_reduce 点积 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] const int sum_seq = std::reduce(std::execution::seq, v.begin(), v.end(), 0, std::plus<>{});
        assert(sum_seq == 15);

        [[maybe_unused]] const int sum_sq =
            std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](int x) { return x * x; });
        assert(sum_sq == 55);

        std::vector<int> a{1, 2, 3};
        std::vector<int> b{4, 5, 6};
        [[maybe_unused]] const int dot = std::transform_reduce(a.begin(), a.end(), b.begin(), 0);
        assert(dot == 32);
        std::cout << "transform_reduce + dot OK\n";
    }

    std::cout << "=== 专家：vs accumulate + 可结合/可交换要求 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] const int acc = std::accumulate(v.begin(), v.end(), 0);
        [[maybe_unused]] const int red = std::reduce(v.begin(), v.end());
        assert(acc == red);

        // ⚠️ reduce 允许重排：并行时 op 必须可结合且通常可交换
        // 不可对减法/字符串拼接想当然地 par reduce
        // par 需链接 TBB（libstdc++）；此处用 seq 保证可移植
        [[maybe_unused]] const int par_like = std::reduce(std::execution::seq, v.begin(), v.end(), 0);
        assert(par_like == 15);

        // C++23 ranges::fold_left 保证左折叠、接管道（见 section08）
        std::cout << "associativity warning + seq portable OK\n";
    }

    std::cout << "[std_reduce] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_reduce", run>;

}  // namespace
