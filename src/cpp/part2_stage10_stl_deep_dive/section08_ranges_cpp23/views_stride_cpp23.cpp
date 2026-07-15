// Topic     : views::stride —— 每隔 n 取一个 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/stride_view
//
// 要点: 跳步采样; stride(1) 恒等。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_stride_cpp23] ===\n";

    std::vector<int> got;
    for (int x : std::views::iota(0, 10) | std::views::stride(3)) got.push_back(x);
    assert((got == std::vector<int>{0, 3, 6, 9}));

    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::vector<int> every2;
    for (int x : v | std::views::stride(2)) every2.push_back(x);
    assert((every2 == std::vector<int>{1, 3, 5}));

    assert(std::ranges::equal(v | std::views::stride(1), v));

    std::cout << "[stride] every-n sampling OK\n";
    std::cout << "views_stride_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_stride_cpp23", run>;

}  // namespace
