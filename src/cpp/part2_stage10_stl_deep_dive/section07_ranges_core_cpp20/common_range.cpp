// Topic     : common_range —— begin/end 同类型
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.2 / 10.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : common_range
// Topic id  : part2/stage10/section07/common_range
// Refs      : https://en.cppreference.com/w/cpp/ranges/common_range
//             https://en.cppreference.com/w/cpp/ranges/common_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [common_range] 入门：vector begin/end 同类型 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        static_assert(std::ranges::common_range<std::vector<int>>);
        static_assert(std::is_same_v<decltype(std::ranges::begin(v)), decltype(std::ranges::end(v))>);
        std::cout << "vector is common_range OK\n";
    }

    std::cout << "=== 进阶：有界 iota common；无限 iota 否 ===\n";
    {
        auto bounded = std::views::iota(0, 5);
        static_assert(std::ranges::common_range<decltype(bounded)>);

        auto inf = std::views::iota(0);
        static_assert(!std::ranges::common_range<decltype(inf)>);
        // end 是 unreachable_sentinel 模型，与 iterator 不同类型
        std::cout << "bounded yes / infinite no OK\n";
    }

    std::cout << "=== 专家：views::common 适配旧算法 / 迭代器对构造 ===\n";
    {
        auto taken = std::views::iota(0) | std::views::take(3);
        auto common = taken | std::views::common;
        static_assert(std::ranges::common_range<decltype(common)>);

        // 旧式需要同类型迭代器对
        std::vector<int> out(common.begin(), common.end());
        assert((out == std::vector<int>{0, 1, 2}));

        // 某些非 common view 不能直接喂 std::vector(it,it) 若类型不同
        // common_view 用 common_iterator 包装
        assert(std::ranges::equal(common, out));
        std::cout << "views::common for legacy APIs OK\n";
    }

    std::cout << "[common_range] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/common_range", run>;

}  // namespace
