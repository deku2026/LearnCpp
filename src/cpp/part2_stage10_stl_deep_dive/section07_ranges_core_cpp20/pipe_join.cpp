// Topic     : views::join —— 展平「范围的范围」
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/join_view
//
// 要点: 一层展平; 元素须是 range; category 可能降为 input。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_join] ===\n";

    std::vector<std::vector<int>> nested{{1, 2}, {3}, {4, 5, 6}};
    std::vector<int> flat;
    for (int x : nested | std::views::join) flat.push_back(x);
    assert((flat == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // 字符串拼接感
    std::vector<std::string> words{"C++", "20", "Ranges"};
    std::string chars;
    for (char c : words | std::views::join) chars.push_back(c);
    assert(chars == "C++20Ranges");

    // join 空组
    std::vector<std::vector<int>> with_empty{{1}, {}, {2, 3}};
    std::vector<int> f2;
    for (int x : with_empty | std::views::join) f2.push_back(x);
    assert((f2 == std::vector<int>{1, 2, 3}));

    std::cout << "[join] nested vector/string flatten OK\n";
    std::cout << "pipe_join: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_join", run>;

}  // namespace
