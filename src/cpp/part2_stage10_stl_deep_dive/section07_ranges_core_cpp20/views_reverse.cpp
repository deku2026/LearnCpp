// Topic     : views::reverse —— 反向视图
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : views_reverse
// Topic id  : part2/stage10/section07/views_reverse
// Refs      : https://en.cppreference.com/w/cpp/ranges/reverse_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_reverse] 入门：反向遍历，不改底层 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        auto rev = v | std::views::reverse;
        std::vector<int> got(rev.begin(), rev.end());
        assert((got == std::vector<int>{5, 4, 3, 2, 1}));
        assert((v == std::vector<int>{1, 2, 3, 4, 5}));
        std::cout << "reverse order OK\n";
    }

    std::cout << "=== 进阶：双 reverse 恒等 + filter 组合 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] auto back = v | std::views::reverse | std::views::reverse;
        assert(std::ranges::equal(back, v));

        std::vector<int> odd_rev;
        for (int x : v | std::views::reverse | std::views::filter([](int x) { return x % 2 != 0; }))
            odd_rev.push_back(x);
        assert((odd_rev == std::vector<int>{5, 3, 1}));
        std::cout << "identity + reverse|filter OK\n";
    }

    std::cout << "=== 专家：需 bidirectional + 写回 + sized ===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        auto rev = v | std::views::reverse;
        static_assert(std::ranges::bidirectional_range<decltype(rev)>);
        static_assert(std::ranges::sized_range<decltype(rev)>);
        assert(std::ranges::size(rev) == 4);
        rev[0] = 99;  // 写回最后一个元素
        assert(v.back() == 99);

        // 单元素 / 空
        std::vector<int> one{7};
        assert(std::ranges::equal(one | std::views::reverse, one));
        std::vector<int> empty;
        assert(std::ranges::empty(empty | std::views::reverse));
        std::cout << "bidirectional + write-back OK\n";
    }

    std::cout << "[views_reverse] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_reverse", run>;

}  // namespace
