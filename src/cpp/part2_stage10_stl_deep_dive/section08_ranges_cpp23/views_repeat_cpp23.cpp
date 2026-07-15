// Topic     : views::repeat —— 重复值 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/repeat_view
//
// 要点: repeat(v,n) 有界; repeat(v) 无限; 配 take。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_repeat_cpp23] ===\n";

    std::vector<int> got;
    for (int x : std::views::repeat(7, 3)) got.push_back(x);
    assert((got == std::vector<int>{7, 7, 7}));
    assert(std::ranges::size(std::views::repeat(0, 5)) == 5);

    // 无限 + take
    std::vector<int> ones;
    for (int x : std::views::repeat(1) | std::views::take(4)) ones.push_back(x);
    assert((ones == std::vector<int>{1, 1, 1, 1}));

    // 填充初始化感
    auto zeros = std::views::repeat(0, 10);
    assert(std::ranges::all_of(zeros, [](int x) { return x == 0; }));

    std::cout << "[repeat] bounded/infinite OK\n";
    std::cout << "views_repeat_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_repeat_cpp23", run>;

}  // namespace
