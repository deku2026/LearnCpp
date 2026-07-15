// Topic     : views::take —— 取前 n 个
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/take_view
//
// 要点: n 可大于 size; 与无限范围组合; 惰性截断。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_take] ===\n";

    std::vector<int> v{10, 20, 30, 40, 50};
    auto first3 = v | std::views::take(3);
    assert(std::ranges::size(first3) == 3);
    std::vector<int> got(first3.begin(), first3.end());
    assert((got == std::vector<int>{10, 20, 30}));

    // n > size → 全部
    auto all = v | std::views::take(100);
    assert(std::ranges::size(all) == 5);

    // take(0) → 空
    assert(std::ranges::empty(v | std::views::take(0)));

    // 无限 + take
    std::vector<int> from_inf;
    for (int x : std::views::iota(1) | std::views::take(4)) from_inf.push_back(x);
    assert((from_inf == std::vector<int>{1, 2, 3, 4}));

    std::cout << "[take] finite/clamp/infinite OK\n";
    std::cout << "pipe_take: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take", run>;

}  // namespace
