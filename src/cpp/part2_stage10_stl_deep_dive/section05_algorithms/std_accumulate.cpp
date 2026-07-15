// Topic     : std::accumulate —— 顺序左折叠
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_accumulate
// Topic id  : part2/stage10/section05/std_accumulate
// Refs      : https://en.cppreference.com/w/cpp/algorithm/accumulate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_accumulate] 入门：求和 / 求积 ===\n";
    {
        std::vector<int> v{3, 1, 4, 1, 5};
        const int sum = std::accumulate(v.begin(), v.end(), 0);
        const int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});
        assert(sum == 14);
        assert(product == 3 * 1 * 4 * 1 * 5);
        std::cout << "sum=" << sum << " product=" << product << '\n';
    }

    std::cout << "=== 进阶：初值类型陷阱 + 自定义 op + 拼接 ===\n";
    {
        // ⚠️ 初值类型决定累加类型：int 初值会截断 double
        std::vector<double> d{0.1, 0.2, 0.3};
        const double ok = std::accumulate(d.begin(), d.end(), 0.0);
        const double bad = std::accumulate(d.begin(), d.end(), 0);  // int 初值!
        assert(ok > 0.59 && ok < 0.61);
        assert(bad == 0);  // 每次截断 → 0
        std::cout << "double with 0.0 => " << ok << "; with int 0 => " << bad << " (truncated!)\n";

        std::vector<int> v{3, 1, 4, 1, 5};
        [[maybe_unused]] const int mx =
            std::accumulate(v.begin(), v.end(), v.front(), [](int a, int b) { return a > b ? a : b; });
        assert(mx == 5);

        std::vector<std::string> parts{"Hello", ", ", "STL"};
        const auto s = std::accumulate(parts.begin(), parts.end(), std::string{});
        assert(s == "Hello, STL");
    }

    std::cout << "=== 专家：保证顺序 vs reduce + 空范围 + ranges::fold ===\n";
    {
        // accumulate 保证从左折叠；reduce(C++17) 可不保序/可并行
        std::vector<std::string> parts{"a", "b", "c"};
        const auto cat = std::accumulate(parts.begin(), parts.end(), std::string{},
                                         [](std::string a, const std::string& b) { return a + b; });
        assert(cat == "abc");  // 顺序确定

        std::vector<int> empty;
        assert(std::accumulate(empty.begin(), empty.end(), 42) == 42);

        // C++23 更推荐 ranges::fold_left（接管道、类型更干净）——见 section08
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] const int sum = std::accumulate(v.begin(), v.end(), 0);
        assert(sum == 15);
        std::cout << "left-fold order + empty init OK\n";
    }

    std::cout << "[std_accumulate] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_accumulate", run>;

}  // namespace
