// Topic     : views::reverse —— 反向视图
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/reverse_view
//
// 要点: 需 bidirectional; 不修改底层; 双 reverse 还原。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_reverse] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};
    auto rev = v | std::views::reverse;
    std::vector<int> got(rev.begin(), rev.end());
    assert((got == std::vector<int>{5, 4, 3, 2, 1}));
    assert((v == std::vector<int>{1, 2, 3, 4, 5}));  // 未改

    auto back = rev | std::views::reverse;
    assert(std::ranges::equal(back, v));

    // reverse + filter
    std::vector<int> odd_rev;
    for (int x : v | std::views::reverse | std::views::filter([](int x) { return x % 2 != 0; })) odd_rev.push_back(x);
    assert((odd_rev == std::vector<int>{5, 3, 1}));

    std::cout << "[reverse] order/identity/filter OK\n";
    std::cout << "views_reverse: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_reverse", run>;

}  // namespace
