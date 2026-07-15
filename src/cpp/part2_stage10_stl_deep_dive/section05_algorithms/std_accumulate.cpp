// Topic     : std::accumulate —— 顺序左折叠
// Doc       : 第2部分-阶段10 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/algorithm/accumulate
//
// 要点: 保证从左到右; 初值类型决定结果类型; 可自定义二元 op。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_accumulate] ===\n";

    std::vector<int> v{3, 1, 4, 1, 5};

    // 求和 / 求积
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    const int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});
    assert(sum == 14);
    assert(product == 3 * 1 * 4 * 1 * 5);
    std::cout << "[basic] sum=" << sum << " product=" << product << '\n';

    // ⚠️ 初值类型: 若用 int 累加 double 会截断
    std::vector<double> d{0.1, 0.2, 0.3};
    const double ok = std::accumulate(d.begin(), d.end(), 0.0);
    const double bad = std::accumulate(d.begin(), d.end(), 0);  // int 初值!
    assert(ok > 0.59 && ok < 0.61);
    assert(bad == 0);  // 每次 (int)0.x 截断 → 始终 0
    std::cout << "[pitfall] double with 0.0 => " << ok << "; with int 0 => " << bad << " (truncated!)\n";

    // 自定义 op: 求最大值
    const int mx = std::accumulate(v.begin(), v.end(), v.front(), [](int a, int b) { return a > b ? a : b; });
    assert(mx == 5);

    // 拼接
    std::vector<std::string> parts{"Hello", ", ", "STL"};
    const auto s = std::accumulate(parts.begin(), parts.end(), std::string{});
    assert(s == "Hello, STL");

    // 空范围返回初值
    std::vector<int> empty;
    assert(std::accumulate(empty.begin(), empty.end(), 42) == 42);

    std::cout << "std_accumulate: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_accumulate", run>;

}  // namespace
