// Topic     : ranges::fold_left —— 左折叠 (C++23, 验收点)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 13.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : ranges_fold_left_cpp23
// Topic id  : part2/stage10/section08/ranges_fold_left_cpp23
// Refs      : https://en.cppreference.com/w/cpp/algorithm/ranges/fold_left
//             P2322

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_fold_left_cpp23] 入门：sum / product（验收）===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
        const int sum = std::ranges::fold_left(v, 0, std::plus{});
        const int product = std::ranges::fold_left(v, 1, std::multiplies{});
#else
        const int sum = std::accumulate(v.begin(), v.end(), 0);
        const int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies{});
#endif
        assert(sum == 15);
        assert(product == 120);
        std::cout << "sum=" << sum << " product=" << product << '\n';
    }

    std::cout << "=== 进阶：接管道 + 字符串折叠 + 空范围 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
        const int even_sum =
            std::ranges::fold_left(v | std::views::filter([](int x) { return x % 2 == 0; }), 0, std::plus{});
        assert(even_sum == 6);

        std::vector<std::string> parts{"a", "-", "b"};
        const auto s = std::ranges::fold_left(parts, std::string{}, std::plus<>{});
        assert(s == "a-b");

        assert(std::ranges::fold_left(std::vector<int>{}, 42, std::plus{}) == 42);
        std::cout << "pipe even_sum=" << even_sum << " cat=" << s << '\n';
#else
        int even_sum = 0;
        for (int x : v)
            if (x % 2 == 0) even_sum += x;
        assert(even_sum == 6);
        std::vector<std::string> parts{"a", "-", "b"};
        const auto s = std::accumulate(parts.begin(), parts.end(), std::string{});
        assert(s == "a-b");
        assert(std::accumulate(std::vector<int>{}.begin(), std::vector<int>{}.end(), 42) == 42);
        std::cout << "fallback even_sum=" << even_sum << " cat=" << s << '\n';
#endif
    }

    std::cout << "=== 专家：优于 accumulate 的点 + feature-test ===\n";
    {
        // 1) ranges 接口可直接接 view  2) 初值类型更干净  3) 有 fold_left_first 无初值变体
        std::vector<int> v{5, 1, 9, 3};
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
        [[maybe_unused]] const int mx =
            std::ranges::fold_left(v, v.front(), [](int a, int b) { return a > b ? a : b; });
        assert(mx == 9);
        // 投影：先 transform 再 fold
        [[maybe_unused]] const int sum_sq =
            std::ranges::fold_left(v | std::views::transform([](int x) { return x * x; }), 0, std::plus{});
        assert(sum_sq == 25 + 1 + 81 + 9);
        std::cout << "__cpp_lib_ranges_fold=" << __cpp_lib_ranges_fold << '\n';
#else
        const int mx = *std::max_element(v.begin(), v.end());
        assert(mx == 9);
        int sum_sq = 0;
        for (int x : v) sum_sq += x * x;
        assert(sum_sq == 116);
        std::cout << "no fold: accumulate/max_element fallback\n";
#endif
        std::cout << "max + sum-of-squares OK\n";
    }

    std::cout << "[ranges_fold_left_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_left_cpp23", run>;

}  // namespace
