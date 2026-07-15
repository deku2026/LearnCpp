// Topic     : std::sort / stable_sort / is_sorted / 自定义比较
// Doc       : 第2部分-阶段10 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/algorithm/sort
//               https://en.cppreference.com/cpp/algorithm/stable_sort
//
// 要点: sort 要求 random_access; 不稳定; stable_sort 保相对序; 平均 O(N log N)。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_sort] ===\n";

    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    std::sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    assert(v.front() == 1 && v.back() == 9);
    std::cout << "[sort] ascending:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    // 降序
    std::sort(v.begin(), v.end(), std::greater<>{});
    assert(v.front() == 9 && v.back() == 1);
    assert(std::is_sorted(v.begin(), v.end(), std::greater<>{}));

    // stable_sort: 相等键保持相对顺序
    using Pair = std::pair<int, char>;
    std::vector<Pair> items{{2, 'a'}, {1, 'x'}, {2, 'b'}, {1, 'y'}, {2, 'c'}};
    std::stable_sort(items.begin(), items.end(), [](const Pair& a, const Pair& b) { return a.first < b.first; });
    // 键 1 的相对序: x 在 y 前; 键 2: a 在 b 在 c 前
    assert(items[0] == Pair(1, 'x') && items[1] == Pair(1, 'y'));
    assert(items[2] == Pair(2, 'a') && items[3] == Pair(2, 'b') && items[4] == Pair(2, 'c'));
    std::cout << "[stable_sort] equal keys keep relative order OK\n";

    // ranges::sort (C++20): 直接传范围 + 投影
    struct Item {
        std::string name;
        int score;
    };
    std::vector<Item> board{{"Alice", 80}, {"Bob", 95}, {"Carol", 70}};
    std::ranges::sort(board, std::greater<>{}, &Item::score);
    assert(board[0].name == "Bob" && board[2].name == "Carol");
    std::cout << "[ranges::sort] projection by score OK\n";

    std::cout << "std_sort: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_sort", run>;

}  // namespace
