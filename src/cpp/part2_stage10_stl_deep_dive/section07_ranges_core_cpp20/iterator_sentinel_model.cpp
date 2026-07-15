// Topic     : iterator-sentinel 分离模型
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : iterator_sentinel_model
// Topic id  : part2/stage10/section07/iterator_sentinel_model
// Refs      : https://en.cppreference.com/w/cpp/iterator/sentinel_for
//             https://en.cppreference.com/w/cpp/iterator/unreachable_sentinel

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterator_sentinel_model] 入门：传统 begin/end 同型 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        static_assert(std::is_same_v<decltype(v.begin()), decltype(v.end())>);
        static_assert(std::ranges::common_range<std::vector<int>>);
        std::cout << "classic same-type end OK\n";
    }

    std::cout << "=== 进阶：无限 iota 的 unreachable_sentinel ===\n";
    {
        auto inf = std::views::iota(1);
        using It = std::ranges::iterator_t<decltype(inf)>;
        using Sent = std::ranges::sentinel_t<decltype(inf)>;
        static_assert(!std::is_same_v<It, Sent>);
        static_assert(std::is_same_v<Sent, std::unreachable_sentinel_t>);
        static_assert(!std::ranges::common_range<decltype(inf)>);

        // take 提供可比较哨兵，截成有限
        std::vector<int> first5;
        for (int x : inf | std::views::take(5)) first5.push_back(x);
        assert((first5 == std::vector<int>{1, 2, 3, 4, 5}));
        std::cout << "unreachable_sentinel + take OK\n";
    }

    std::cout << "=== 专家：惰性终止 + subrange 绑不同型 ===\n";
    {
        auto inf = std::views::iota(1);
        std::vector<int> under10;
        for (int x : inf | std::views::take_while([](int x) { return x < 10; })) under10.push_back(x);
        assert(under10.size() == 9 && under10.back() == 9);

        std::vector<int> v{1, 2, 3};
        auto r = std::ranges::subrange(v.begin(), v.end());
        assert(std::ranges::size(r) == 3);

        // 意义：无限范围 / C 字符串到 '\\0' 等惰性终止成为可能
        // （旧 STL 要求 begin/end 同型、范围必须有限）
        static_assert(std::sentinel_for<std::unreachable_sentinel_t, decltype(inf.begin())>);
        std::cout << "lazy stop + subrange + sentinel_for OK\n";
    }

    std::cout << "[iterator_sentinel_model] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/iterator_sentinel_model", run>;

}  // namespace
