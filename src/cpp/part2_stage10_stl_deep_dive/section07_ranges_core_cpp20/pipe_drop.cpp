// Topic     : views::drop —— 跳过前 n 个
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_drop
// Topic id  : part2/stage10/section07/pipe_drop
// Refs      : https://en.cppreference.com/w/cpp/ranges/drop_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_drop] 入门：跳过前 n 个 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        auto tail = v | std::views::drop(2);
        std::vector<int> got(tail.begin(), tail.end());
        assert((got == std::vector<int>{3, 4, 5, 6}));
        assert(std::ranges::equal(v | std::views::drop(0), v));
        std::cout << "drop(2) / drop(0) OK\n";
    }

    std::cout << "=== 进阶：n≥size 空 + drop|take 切片 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        assert(std::ranges::empty(v | std::views::drop(100)));
        assert(std::ranges::empty(v | std::views::drop(6)));

        // drop + take = 半开切片 [2, 5)
        std::vector<int> mid;
        for (int x : v | std::views::drop(2) | std::views::take(3)) mid.push_back(x);
        assert((mid == std::vector<int>{3, 4, 5}));

        // drop_while 对照在另一文件；这里组合 transform
        std::vector<int> scaled;
        for (int x : v | std::views::drop(3) | std::views::transform([](int x) { return x * 10; })) scaled.push_back(x);
        assert((scaled == std::vector<int>{40, 50, 60}));
        std::cout << "empty edges + slice pipe OK\n";
    }

    std::cout << "=== 专家：sized + random_access 保持 + 与 iota ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        auto d = v | std::views::drop(2);
        // vector 上 drop 通常保持 random_access / sized
        static_assert(std::ranges::random_access_range<decltype(d)>);
        static_assert(std::ranges::sized_range<decltype(d)>);
        assert(std::ranges::size(d) == 4);
        assert(d[0] == 3 && d[3] == 6);

        // 无限 iota 上 drop 仍可再 take
        std::vector<int> from_iota;
        for (int x : std::views::iota(0) | std::views::drop(5) | std::views::take(3)) from_iota.push_back(x);
        assert((from_iota == std::vector<int>{5, 6, 7}));
        std::cout << "category keep + infinite iota OK\n";
    }

    std::cout << "[pipe_drop] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop", run>;

}  // namespace
