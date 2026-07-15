// Topic     : views::cartesian_product —— 笛卡尔积 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/cartesian_product_view
//
// 要点: 所有组合; 网格坐标生成; 大小为各长度之积。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_cartesian_product_cpp23] ===\n";

    std::vector<int> xs{1, 2};
    std::vector<char> ys{'a', 'b'};

    std::vector<std::string> pairs;
    for (auto [x, y] : std::views::cartesian_product(xs, ys)) {
        pairs.push_back(std::to_string(x) + y);
    }
    assert((pairs == std::vector<std::string>{"1a", "1b", "2a", "2b"}));

    // 坐标网格
    std::vector<std::pair<int, int>> grid;
    for (auto [i, j] : std::views::cartesian_product(std::views::iota(0, 2), std::views::iota(0, 3)))
        grid.emplace_back(i, j);
    assert(grid.size() == 6);
    // assert 是宏：pair 字面量中的逗号需额外括号
    assert((grid.front() == std::pair{0, 0}));
    assert((grid.back() == std::pair{1, 2}));

    // 空因子 → 空积
    std::vector<int> empty;
    assert(std::ranges::empty(std::views::cartesian_product(xs, empty)));

    std::cout << "[cartesian_product] pairs + grid OK\n";
    std::cout << "views_cartesian_product_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_cartesian_product_cpp23", run>;

}  // namespace
