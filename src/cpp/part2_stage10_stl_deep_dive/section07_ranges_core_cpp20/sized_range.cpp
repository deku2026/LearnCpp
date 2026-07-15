// Topic     : sized_range —— O(1) 可知 size
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.2 / 14.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : sized_range
// Topic id  : part2/stage10/section07/sized_range
// Refs      : https://en.cppreference.com/w/cpp/ranges/sized_range

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sized_range] 入门：vector/list 的 ranges::size ===\n";
    {
        static_assert(std::ranges::sized_range<std::vector<int>>);
        static_assert(std::ranges::sized_range<std::list<int>>);  // C++11 起 list::size O(1)

        std::vector<int> v{1, 2, 3, 4, 5};
        assert(std::ranges::size(v) == 5);
        std::cout << "vector/list sized OK\n";
    }

    std::cout << "=== 进阶：take/iota 保持 sized；filter 否 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        auto taken = v | std::views::take(3);
        static_assert(std::ranges::sized_range<decltype(taken)>);
        assert(std::ranges::size(taken) == 3);

        auto filtered = v | std::views::filter([](int x) { return x > 2; });
        static_assert(!std::ranges::sized_range<decltype(filtered)>);
        // 仍可 distance（可能线性扫）
        assert(std::ranges::distance(filtered) == 3);

        auto iota = std::views::iota(0, 10);
        static_assert(std::ranges::sized_range<decltype(iota)>);
        assert(std::ranges::size(iota) == 10);
        std::cout << "take/iota yes; filter no OK\n";
    }

    std::cout << "=== 专家：无限非 sized + 组合 category 提醒 ===\n";
    {
        auto inf = std::views::iota(0);
        static_assert(!std::ranges::sized_range<decltype(inf)>);

        // take 截断无限：元素个数确定，但 std::ranges::size 是否可用因实现而异
        // （MS STL 上 take_view+iota 可能 sized_range 却不便走 size CPO）——统一用 distance 验收。
        auto finite = inf | std::views::take(4);
        assert(std::ranges::distance(finite) == 4);
        std::cout << "  take(4)|iota infinite: distance=4"
                  << " sized_range=" << std::ranges::sized_range<decltype(finite)> << '\n';

        // 需要 size 的下游（如某些 reserve 优化 / to）在 filter 后可能走慢路径
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        auto pipe = v | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::take(10);
        // filter 破坏 sized 后 take 也可能非 O(1) size（实现相关）
        assert(std::ranges::distance(pipe) == 3);
        std::cout << "infinite/take + filter distance OK\n";
    }

    std::cout << "[sized_range] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/sized_range", run>;

}  // namespace
