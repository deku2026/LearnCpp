// Topic     : viewable_range —— 能安全转成 view 的范围
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : viewable_range
// Topic id  : part2/stage10/section07/viewable_range
// Refs      : https://en.cppreference.com/w/cpp/ranges/viewable_range

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [viewable_range] 入门：管道入口约束 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        static_assert(std::ranges::viewable_range<std::vector<int>&>);
        static_assert(std::ranges::viewable_range<std::vector<int>>);  // rvalue → owning_view

        auto pipeline = v | std::views::transform([](int x) { return x + 1; });
        std::vector<int> out;
        for (int x : pipeline) out.push_back(x);
        assert((out == std::vector<int>{2, 3, 4}));
        std::cout << "lvalue pipe entry OK\n";
    }

    std::cout << "=== 进阶：views::all → ref_view / 已是 view ===\n";
    {
        std::vector<int> v{1, 2, 3};
        auto all = std::views::all(v);
        static_assert(std::ranges::view<decltype(all)>);
        assert(std::ranges::size(all) == 3);
        assert(std::ranges::equal(all, v));

        auto pipeline = v | std::views::transform([](int x) { return x + 1; });
        static_assert(std::ranges::viewable_range<decltype(pipeline)>);
        // 可继续 | filter
        [[maybe_unused]] auto more = pipeline | std::views::filter([](int x) { return x > 2; });
        assert(std::ranges::distance(more) == 2);
        std::cout << "views::all + re-pipe OK\n";
    }

    std::cout << "=== 专家：rvalue 容器进管道会 owning ===\n";
    {
        // rvalue vector 也可 viewable：移入 owning_view（代价是拥有一份）
        auto owned_pipe = std::vector{1, 2, 3, 4} | std::views::filter([](int x) { return x > 2; });
        std::vector<int> got;
        for (int x : owned_pipe) got.push_back(x);
        assert((got == std::vector<int>{3, 4}));

        // 教学：更清晰的是具名容器 + 非拥有 view，避免意外移动成本
        std::cout << "rvalue → owning_view path OK\n";
    }

    std::cout << "[viewable_range] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/viewable_range", run>;

}  // namespace
