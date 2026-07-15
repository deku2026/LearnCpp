// Topic     : views::take —— 取前 n 个
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.1 / 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_take
// Topic id  : part2/stage10/section07/pipe_take
// Refs      : https://en.cppreference.com/w/cpp/ranges/take_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_take] 入门：取前 n 个（惰性）===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> got;
        for (int x : v | std::views::take(3)) got.push_back(x);
        assert((got == std::vector<int>{1, 2, 3}));
        assert(std::ranges::empty(v | std::views::take(0)));
        std::cout << "take(3)/take(0) OK\n";
    }

    std::cout << "=== 进阶：n≥size 全取 + 与 filter/transform 组合 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        assert(std::ranges::equal(v | std::views::take(100), v));

        // 文档主线：filter → transform → take（惰性：够 3 个就停）
        std::vector<int> result;
        for (int x : v | std::views::filter([](int x) { return x % 2 == 0; }) |
                         std::views::transform([](int x) { return x * x; }) | std::views::take(2)) {
            result.push_back(x);
        }
        // 偶数 2,4 → 平方 4,16；take(2) 得到这两
        assert((result == std::vector<int>{4, 16}));
        std::cout << "cap + filter|transform|take pipeline OK\n";
    }

    std::cout << "=== 专家：截断无限范围 + sized 语义 ===\n";
    {
        // take 让无限 iota 变成有限
        std::vector<int> finite;
        for (int x : std::views::iota(1) | std::views::take(5)) finite.push_back(x);
        assert((finite == std::vector<int>{1, 2, 3, 4, 5}));

        std::vector<int> v{10, 20, 30, 40};
        auto t = v | std::views::take(2);
        static_assert(std::ranges::sized_range<decltype(t)>);
        assert(std::ranges::size(t) == 2);
        assert(t[0] == 10 && t[1] == 20);

        // take 后再 drop：先取再丢
        std::vector<int> mid;
        for (int x : v | std::views::take(3) | std::views::drop(1)) mid.push_back(x);
        assert((mid == std::vector<int>{20, 30}));
        std::cout << "infinite cap + sized OK\n";
    }

    std::cout << "[pipe_take] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take", run>;

}  // namespace
