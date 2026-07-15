// Topic     : views::take_while —— 取到谓词为假
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/take_while_view
//
// 要点: 惰性终止; 谓词首次假后停止(即使后面还有真)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_take_while] ===\n";

    std::vector<int> v{1, 2, 3, 4, 0, 5, 6};
    auto prefix = v | std::views::take_while([](int x) { return x != 0; });
    // 部分实现的 view 迭代器不能直接喂 vector 双迭代器构造，循环收集更稳妥
    std::vector<int> got;
    for (int x : prefix) got.push_back(x);
    assert((got == std::vector<int>{1, 2, 3, 4}));  // 0 后的 5,6 不要

    // 与 iota
    std::vector<int> small;
    for (int x : std::views::iota(1) | std::views::take_while([](int x) { return x < 5; })) small.push_back(x);
    assert((small == std::vector<int>{1, 2, 3, 4}));

    // 一开始就假 → 空
    assert(std::ranges::empty(v | std::views::take_while([](int x) { return x < 0; })));

    std::cout << "[take_while] stop-at-predicate OK\n";
    std::cout << "pipe_take_while: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take_while", run>;

}  // namespace
