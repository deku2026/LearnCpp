// Topic     : views::join —— 展平「范围的范围」
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.2 / 14.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_join
// Topic id  : part2/stage10/section07/pipe_join
// Refs      : https://en.cppreference.com/w/cpp/ranges/join_view

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_join] 入门：一层展平 nested ranges ===\n";
    {
        std::vector<std::vector<int>> nested{{1, 2}, {3}, {4, 5, 6}};
        std::vector<int> flat;
        for (int x : nested | std::views::join) flat.push_back(x);
        assert((flat == std::vector<int>{1, 2, 3, 4, 5, 6}));
        std::cout << "nested vector flatten OK\n";
    }

    std::cout << "=== 进阶：字符串拼接感 + 空内层 + transform ===\n";
    {
        std::vector<std::string> words{"C++", "20", "Ranges"};
        std::string chars;
        for (char c : words | std::views::join) chars.push_back(c);
        assert(chars == "C++20Ranges");

        std::vector<std::vector<int>> with_empty{{1}, {}, {2, 3}};
        std::vector<int> f2;
        for (int x : with_empty | std::views::join) f2.push_back(x);
        assert((f2 == std::vector<int>{1, 2, 3}));

        std::vector<std::vector<int>> nested{{1, 2}, {3}};
        std::vector<int> scaled;
        for (int x : nested | std::views::join | std::views::transform([](int x) { return x * 10; }))
            scaled.push_back(x);
        assert((scaled == std::vector<int>{10, 20, 30}));
        std::cout << "string join + empty inner + transform OK\n";
    }

    std::cout << "=== 专家：只展一层 + 双 join 全平 + category ===\n";
    {
        // 三层只展一层 → 仍是 range-of-range
        std::vector<std::vector<std::vector<int>>> deep{{{1, 2}}, {{3}}};
        std::vector<std::vector<int>> mid;
        for (auto&& inner : deep | std::views::join) {
            mid.emplace_back(inner.begin(), inner.end());
        }
        assert(mid.size() == 2);
        assert((mid[0] == std::vector<int>{1, 2}));

        // 再 join 一次才全平
        std::vector<int> flat2;
        for (int x : deep | std::views::join | std::views::join) flat2.push_back(x);
        assert((flat2 == std::vector<int>{1, 2, 3}));

        std::vector<std::vector<int>> nested{{1, 2}, {3, 4}};
        auto j = nested | std::views::join;
        static_assert(std::ranges::input_range<decltype(j)>);
        // 需要分隔符时用 C++23 join_with（另文件）
        std::cout << "one-level + double join OK\n";
    }

    std::cout << "[pipe_join] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_join", run>;

}  // namespace
