// Topic     : C++17 执行策略：seq / par / par_unseq / unseq
// Doc       : 第2部分-阶段10 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/algorithm/execution_policy_tag
//
// 要点: 一行启用并行/向量化; 谓词禁止数据竞争; libstdc++ 常需 -ltbb。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [parallel_execution_policy_cpp17] ===\n";

    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 0);

    // seq: 显式顺序(对照基线)
    auto a = v;
    std::sort(std::execution::seq, a.begin(), a.end(), std::greater<>{});
    assert(a.front() == 999 && a.back() == 0);

    // par: 允许并行(实现可回退到顺序)
    auto b = v;
    std::for_each(std::execution::par, b.begin(), b.end(), [](int& x) { x *= 2; });
    assert(b[0] == 0 && b[10] == 20 && b.back() == 1998);

    // par_unseq: 并行 + 向量化许可(谓词须可重入、无数据竞争)
    auto c = v;
    std::transform(std::execution::par_unseq, c.begin(), c.end(), c.begin(), [](int x) { return x + 1; });
    assert(c[0] == 1 && c.back() == 1000);

    // unseq (C++20): 单线程向量化许可
    auto d = v;
    std::for_each(std::execution::unseq, d.begin(), d.end(), [](int& x) { x = x * x; });
    assert(d[3] == 9 && d[10] == 100);

    // reduce + 策略
    const long long sum = std::reduce(std::execution::par, v.begin(), v.end(), 0LL);
    assert(sum == (999LL * 1000LL) / 2);

    // ⚠️ 并行谓词不能写共享可变状态(数据竞争 → UB)
    // 错误示例(勿启用): int cnt=0; for_each(par, ..., [&](int){ ++cnt; });

    std::cout << "[policy] seq/par/par_unseq/unseq demos OK, sum=" << sum << '\n';
    std::cout << "parallel_execution_policy_cpp17: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/parallel_execution_policy_cpp17", run>;

}  // namespace
