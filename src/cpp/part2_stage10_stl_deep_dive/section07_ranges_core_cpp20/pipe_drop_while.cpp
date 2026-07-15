// Topic     : views::drop_while —— 跳到谓词为假
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_drop_while
// Topic id  : part2/stage10/section07/pipe_drop_while
// Refs      : https://en.cppreference.com/w/cpp/ranges/drop_while_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_drop_while] 入门：跳过前缀满足谓词的元素 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 0, 6};
        std::vector<int> got;
        for (int x : v | std::views::drop_while([](int x) { return x < 4; })) got.push_back(x);
        assert((got == std::vector<int>{4, 5, 0, 6}));  // 从首个 ≥4 起保留全部
        std::cout << "drop prefix <4 OK\n";
    }

    std::cout << "=== 进阶：全丢 / 全留 + 与 take_while 对称 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        assert(std::ranges::empty(v | std::views::drop_while([](int) { return true; })));
        assert(std::ranges::equal(v | std::views::drop_while([](int) { return false; }), v));

        // 字符串：跳过空白
        std::string s = "   hi";
        std::string trimmed;
        for (char c : s | std::views::drop_while([](char c) { return c == ' '; })) trimmed.push_back(c);
        assert(trimmed == "hi");
        std::cout << "all-drop/all-keep + whitespace OK\n";
    }

    std::cout << "=== 专家：只看前缀 + 中间再满足也不回跳 ===\n";
    {
        // drop_while 找到第一个假后，后续即使再满足谓词也保留
        std::vector<int> v{2, 4, 1, 6, 8};
        std::vector<int> got;
        for (int x : v | std::views::drop_while([](int x) { return x % 2 == 0; })) got.push_back(x);
        // 跳过 2,4；从 1 起：1,6,8 全留（6,8 虽偶数）
        assert((got == std::vector<int>{1, 6, 8}));

        // 组合：drop_while 再 filter
        std::vector<int> only_big;
        for (int x : v | std::views::drop_while([](int x) { return x < 3; }) |
                         std::views::filter([](int x) { return x > 5; })) {
            only_big.push_back(x);
        }
        assert((only_big == std::vector<int>{6, 8}));
        std::cout << "prefix-only semantics OK\n";
    }

    std::cout << "[pipe_drop_while] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop_while", run>;

}  // namespace
