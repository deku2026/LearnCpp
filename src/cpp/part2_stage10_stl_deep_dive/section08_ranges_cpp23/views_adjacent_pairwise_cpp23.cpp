// Topic     : views::adjacent / adjacent_transform / pairwise (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.2
// cppreference: https://en.cppreference.com/cpp/ranges/adjacent_view
//
// 要点: 编译期窗口 N 产出 tuple; pairwise ≡ adjacent<2>。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_adjacent_pairwise_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6};

    std::vector<int> pair_sums;
    for (auto [a, b] : v | std::views::adjacent<2>) pair_sums.push_back(a + b);
    assert((pair_sums == std::vector<int>{3, 5, 7, 9, 11}));

    // pairwise 别名
    std::vector<int> same;
    for (auto [a, b] : v | std::views::pairwise) same.push_back(a + b);
    assert(same == pair_sums);

    // adjacent_transform: 相邻积
    std::vector<int> products;
    for (int p : v | std::views::adjacent_transform<2>(std::multiplies{})) products.push_back(p);
    assert((products == std::vector<int>{2, 6, 12, 20, 30}));

    // adjacent<3>
    int triple_count = 0;
    for (auto [a, b, c] : v | std::views::adjacent<3>) {
        assert(b == a + 1 && c == b + 1);
        ++triple_count;
    }
    assert(triple_count == 4);

    // 长度 < N → 空
    assert(std::ranges::empty(std::vector{1, 2} | std::views::adjacent<3>));

    std::cout << "[adjacent/pairwise] window tuples + transform OK\n";
    std::cout << "views_adjacent_pairwise_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_adjacent_pairwise_cpp23", run>;

}  // namespace
