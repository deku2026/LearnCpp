// Topic     : views::drop —— 跳过前 n 个
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/drop_view
//
// 要点: n ≥ size → 空; 与 take 对称。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_drop] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto tail = v | std::views::drop(2);
    std::vector<int> got(tail.begin(), tail.end());
    assert((got == std::vector<int>{3, 4, 5, 6}));

    assert(std::ranges::empty(v | std::views::drop(100)));
    assert(std::ranges::equal(v | std::views::drop(0), v));

    // drop + take = 切片 [2, 5)
    std::vector<int> mid;
    for (int x : v | std::views::drop(2) | std::views::take(3)) mid.push_back(x);
    assert((mid == std::vector<int>{3, 4, 5}));

    std::cout << "[drop] skip + slice OK\n";
    std::cout << "pipe_drop: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop", run>;

}  // namespace
