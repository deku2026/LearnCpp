// Topic     : views::cartesian_product —— 笛卡尔积 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_cartesian_product_cpp23
// Topic id  : part2/stage10/section08/views_cartesian_product_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/cartesian_product_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_cartesian_product_cpp23] 入门：所有组合 ===\n";
    {
        std::vector<int> xs{1, 2};
        std::vector<char> ys{'a', 'b'};
        std::vector<std::string> pairs;
        for (auto [x, y] : std::views::cartesian_product(xs, ys)) {
            pairs.push_back(std::to_string(x) + y);
        }
        assert((pairs == std::vector<std::string>{"1a", "1b", "2a", "2b"}));
        std::cout << "2D product OK\n";
    }

    std::cout << "=== 进阶：坐标网格 + 大小为各长度之积 ===\n";
    {
        std::vector<std::pair<int, int>> grid;
        for (auto [i, j] : std::views::cartesian_product(std::views::iota(0, 2), std::views::iota(0, 3)))
            grid.emplace_back(i, j);
        assert(grid.size() == 6);  // 2*3
        assert((grid.front() == std::pair{0, 0}));
        assert((grid.back() == std::pair{1, 2}));

        // 三路
        int n = 0;
        for (auto [a, b, c] :
             std::views::cartesian_product(std::views::iota(0, 2), std::views::iota(0, 2), std::views::iota(0, 2))) {
            (void)a;
            (void)b;
            (void)c;
            ++n;
        }
        assert(n == 8);
        std::cout << "grid + 3-way OK\n";
    }

    std::cout << "=== 专家：空因子 → 空积 + 组合爆炸提醒 ===\n";
    {
        std::vector<int> xs{1, 2};
        std::vector<int> empty;
        assert(std::ranges::empty(std::views::cartesian_product(xs, empty)));
        assert(std::ranges::empty(std::views::cartesian_product(empty, xs)));

        // ⚠️ 大小乘积爆炸：大范围 cartesian 勿轻易 materialize
        auto big = std::views::cartesian_product(std::views::iota(0, 100), std::views::iota(0, 100));
        assert(std::ranges::size(big) == 10000);
        // 惰性：创建不分配 10000 个 tuple；迭代才产生

#if defined(__cpp_lib_ranges_cartesian_product)
        std::cout << "__cpp_lib_ranges_cartesian_product=" << __cpp_lib_ranges_cartesian_product << '\n';
#endif
        std::cout << "empty factor + lazy size OK\n";
    }

    std::cout << "[views_cartesian_product_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_cartesian_product_cpp23", run>;

}  // namespace
