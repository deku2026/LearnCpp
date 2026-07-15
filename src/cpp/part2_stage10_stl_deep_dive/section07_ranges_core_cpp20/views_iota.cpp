// Topic     : views::iota —— 递增序列(可无限)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.3 / 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : views_iota
// Topic id  : part2/stage10/section07/views_iota
// Refs      : https://en.cppreference.com/w/cpp/ranges/iota_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_iota] 入门：有界 iota [start, bound) ===\n";
    {
        auto five = std::views::iota(1, 6);  // [1,6)
        std::vector<int> v(five.begin(), five.end());
        assert((v == std::vector<int>{1, 2, 3, 4, 5}));
        assert(std::ranges::size(five) == 5);
        std::cout << "bounded iota OK\n";
    }

    std::cout << "=== 进阶：无限 + take + transform 平方 ===\n";
    {
        std::vector<int> sq;
        for (int x : std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; })) sq.push_back(x);
        assert((sq == std::vector<int>{1, 4, 9, 16, 25}));

        [[maybe_unused]] int sum = 0;
        for (int x : std::views::iota(1) | std::views::take(100)) sum += x;
        assert(sum == 5050);  // 1..100
        std::cout << "infinite|take + squares OK\n";
    }

    std::cout << "=== 专家：vs 算法 iota + borrowed_range + sentinel 模型 ===\n";
    {
        // 算法版 std::iota：写入已有缓冲
        std::vector<int> buf(4);
        std::iota(buf.begin(), buf.end(), 10);
        assert((buf == std::vector<int>{10, 11, 12, 13}));

        // views::iota 不分配容器，惰性生成
        // 有界 iota 是 borrowed_range（迭代器不依赖 view 对象存活语义宽松）
        static_assert(std::ranges::borrowed_range<decltype(std::views::iota(0, 5))>);

        // iterator-sentinel：无限 iota 的 end 是 unreachable_sentinel 类模型
        auto inf = std::views::iota(0);
        auto it = inf.begin();
        assert(*it == 0);
        ++it;
        assert(*it == 1);
        // 无 bound 时不能 size

        [[maybe_unused]] auto it3 = std::ranges::find(std::views::iota(0, 5), 3);
        assert(*it3 == 3);
        std::cout << "algo vs view + borrowed + sentinel OK\n";
    }

    std::cout << "[views_iota] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_iota", run>;

}  // namespace
