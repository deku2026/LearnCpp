// Topic     : ranges::fold_left —— 左折叠 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/fold_left
//
// 要点: ranges 版 accumulate; 可接管道; 带初值。

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
    std::cout << "=== [ranges_fold_left_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    const int sum = std::ranges::fold_left(v, 0, std::plus{});
    const int product = std::ranges::fold_left(v, 1, std::multiplies{});
    assert(sum == 15);
    assert(product == 120);

    // 接管道
    const int even_sum =
        std::ranges::fold_left(v | std::views::filter([](int x) { return x % 2 == 0; }), 0, std::plus{});
    assert(even_sum == 6);

    // 字符串折叠
    std::vector<std::string> parts{"a", "-", "b"};
    const auto s = std::ranges::fold_left(parts, std::string{}, std::plus<>{});
    assert(s == "a-b");

    // 空范围 → 初值
    assert(std::ranges::fold_left(std::vector<int>{}, 42, std::plus{}) == 42);

    std::cout << "[fold_left] sum=" << sum << " product=" << product << " even_sum=" << even_sum << '\n';
#else
    std::cout << "library not yet available, fallback demo (accumulate)\n";
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    const int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies{});
    assert(sum == 15);
    assert(product == 120);

    int even_sum = 0;
    for (int x : v)
        if (x % 2 == 0) even_sum += x;
    assert(even_sum == 6);

    std::vector<std::string> parts{"a", "-", "b"};
    const auto s = std::accumulate(parts.begin(), parts.end(), std::string{});
    assert(s == "a-b");

    assert(std::accumulate(std::vector<int>{}.begin(), std::vector<int>{}.end(), 42) == 42);

    std::cout << "[fold_left] fallback sum=" << sum << " product=" << product << " even_sum=" << even_sum << '\n';
#endif

    std::cout << "ranges_fold_left_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_left_cpp23", run>;

}  // namespace
