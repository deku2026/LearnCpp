// Topic     : views::filter —— 惰性过滤
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.1 / 11.2 / 14.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_filter
// Topic id  : part2/stage10/section07/pipe_filter
// Refs      : https://en.cppreference.com/w/cpp/ranges/filter_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_filter] 入门：惰性保留满足谓词的元素 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });

        std::vector<int> got;
        for (int x : evens) got.push_back(x);
        assert((got == std::vector<int>{2, 4, 6, 8, 10}));

        // 空过滤结果
        auto none = v | std::views::filter([](int x) { return x > 100; });
        assert(std::ranges::empty(none));
        std::cout << "lazy even filter OK\n";
    }

    std::cout << "=== 进阶：管道组合 filter → transform + 可重复遍历 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> sq;
        for (int x :
             v | std::views::filter([](int x) { return x > 5; }) | std::views::transform([](int x) { return x * x; })) {
            sq.push_back(x);
        }
        assert((sq == std::vector<int>{36, 49, 64, 81, 100}));

        // 底层 vector 仍在：forward/bidirectional 可再遍历
        auto f = v | std::views::filter([](int x) { return x % 2 == 0; });
        std::vector<int> a(f.begin(), f.end());
        std::vector<int> b(f.begin(), f.end());
        assert(a == b);
        std::cout << "pipe combo + re-iterate OK\n";
    }

    std::cout << "=== 专家：category 降级 + 谓词稳定 + 勿夹临时 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });

        static_assert(std::ranges::random_access_range<std::vector<int>>);
        // filter 不能 O(1) 跳到第 n 个通过元素 → 非 random_access
        static_assert(!std::ranges::random_access_range<decltype(evens)>);
        static_assert(std::ranges::bidirectional_range<decltype(evens)>);
        // evens[0]; // ❌

        // ⚠️ 谓词应稳定：迭代期间修改「是否通过」的判定依赖易 UB/混乱
        // ⚠️ 管道夹临时：auto bad = make_vec() | views::filter(...); // 悬垂

        // 修改底层：view 即时看到（不拥有）
        auto odds = v | std::views::filter([](int x) { return x % 2 != 0; });
        v[0] = 100;  // 原 1 变为偶数 → 不再通过
        std::vector<int> after;
        for (int x : odds) after.push_back(x);
        assert((after == std::vector<int>{3, 5}));
        std::cout << "category drop + live underlying OK\n";
    }

    std::cout << "[pipe_filter] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_filter", run>;

}  // namespace
