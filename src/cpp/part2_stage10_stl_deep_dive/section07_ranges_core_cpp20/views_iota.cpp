// Topic     : views::iota —— 递增序列(可无限)
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/iota_view
//
// 要点: iota(start) 无限; iota(start,bound) 有界; 惰性生成无容器。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_iota] ===\n";

    auto five = std::views::iota(1, 6);  // [1,6)
    std::vector<int> v(five.begin(), five.end());
    assert((v == std::vector<int>{1, 2, 3, 4, 5}));
    assert(std::ranges::size(five) == 5);

    // 平方数
    std::vector<int> sq;
    for (int x : std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; })) sq.push_back(x);
    assert((sq == std::vector<int>{1, 4, 9, 16, 25}));

    // 无限 + take
    int sum = 0;
    for (int x : std::views::iota(1) | std::views::take(100)) sum += x;
    assert(sum == 5050);

    // 与旧 iota 算法区别: views 不写进已有缓冲
    std::vector<int> buf(4);
    std::iota(buf.begin(), buf.end(), 10);  // 算法版写入
    assert((buf == std::vector<int>{10, 11, 12, 13}));

    std::cout << "[iota] bounded/infinite/transform OK\n";
    std::cout << "views_iota: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_iota", run>;

}  // namespace
