// Topic     : std::reduce / transform_reduce —— 可并行折叠
// Doc       : 第2部分-阶段10 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/algorithm/reduce
//
// 要点: reduce 不保证顺序 → op 需可结合/可交换; 与 accumulate 对比。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_reduce] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

    // 无初值: 用 value-initialized T{}
    const int sum1 = std::reduce(v.begin(), v.end());
    assert(sum1 == 15);

    // 带初值
    const int sum2 = std::reduce(v.begin(), v.end(), 100);
    assert(sum2 == 115);

    // 自定义 op
    const int prod = std::reduce(v.begin(), v.end(), 1, std::multiplies<>{});
    assert(prod == 120);

    // 顺序策略(与 accumulate 行为一致, 仍不强制结合律校验)
    const int sum_seq = std::reduce(std::execution::seq, v.begin(), v.end(), 0, std::plus<>{});
    assert(sum_seq == 15);

    // transform_reduce: map + reduce
    const int sum_sq = std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](int x) { return x * x; });
    assert(sum_sq == 55);

    // 二元 transform_reduce: 点积
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    const int dot = std::transform_reduce(a.begin(), a.end(), b.begin(), 0);
    assert(dot == 32);

    // 与 accumulate: 对 +/* 结果相同; 对非交换 op 可能不同
    // reduce 允许重排, 故不可用减法等非结合操作并行 reduce
    const int acc = std::accumulate(v.begin(), v.end(), 0);
    assert(acc == sum1);

    std::cout << "[reduce] sum=" << sum1 << " prod=" << prod << " sum_sq=" << sum_sq << " dot=" << dot << '\n';
    std::cout << "std_reduce: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_reduce", run>;

}  // namespace
