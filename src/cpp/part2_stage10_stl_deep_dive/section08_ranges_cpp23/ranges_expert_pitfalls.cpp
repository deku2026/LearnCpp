// Topic     : Ranges 专家级陷阱：悬垂 / 单遍 / category 降级
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 14
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : ranges_expert_pitfalls
// Topic id  : part2/stage10/section08/ranges_expert_pitfalls
// Refs      : https://en.cppreference.com/w/cpp/ranges
//             https://en.cppreference.com/w/cpp/ranges/borrowed_range
//             https://en.cppreference.com/w/cpp/ranges/dangling

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>
#include <version>

namespace {

std::vector<int> make_vec() {
    return {1, 2, 3, 4, 5};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_expert_pitfalls] 入门：勿把 Ranges 当 LINQ ===\n";
    {
        // 三大反差：单遍、易悬垂、category 降级（见下文各节）
        std::vector<int> v{1, 2, 3, 4, 5};
        [[maybe_unused]] auto view = v | std::views::transform([](int x) { return x * 2; });
        // view 不是容器：不拥有、category 依赖管道
        assert(std::ranges::size(view) == 5);
        std::cout << "view is not a container OK\n";
    }

    std::cout << "=== 进阶：夹临时悬垂 + dangling 编译期拦截 ===\n";
    {
        // auto bad = make_vec() | std::views::filter([](int x){ return x > 2; });
        // // 临时析构 → bad 悬垂 → 迭代 UB

        auto owned = make_vec();
        auto good = owned | std::views::filter([](int x) { return x > 2; });
        std::vector<int> g(good.begin(), good.end());
        assert((g == std::vector<int>{3, 4, 5}));

#if defined(__cpp_lib_ranges_to_container)
        auto material = make_vec() | std::views::filter([](int x) { return x > 2; }) | std::ranges::to<std::vector>();
        assert(material.size() == 3);
#else
        auto tmp = make_vec();
        std::vector<int> material;
        for (int x : tmp | std::views::filter([](int x) { return x > 2; })) material.push_back(x);
        assert(material.size() == 3);
#endif

        auto d = std::ranges::find(std::vector{1, 2, 3}, 2);
        static_assert(std::is_same_v<decltype(d), std::ranges::dangling>);
        std::cout << "named storage / to / dangling OK\n";
    }

    std::cout << "=== 专家：category 降级 + 可复用纪律 + zip 最弱 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        static_assert(std::ranges::random_access_range<std::vector<int>>);
        auto filtered = v | std::views::filter([](int x) { return x > 1; });
        static_assert(!std::ranges::random_access_range<decltype(filtered)>);
        static_assert(std::ranges::bidirectional_range<decltype(filtered)>);
        // filtered[0]; // ❌

        auto z = std::views::zip(v, v);
        static_assert(std::ranges::random_access_range<decltype(z)>);

        // 可再遍历（底层仍在的 transform）
        auto view = v | std::views::transform([](int x) { return x * 2; });
#if defined(__cpp_lib_ranges_to_container)
        auto once = std::ranges::to<std::vector>(view);
        auto twice = std::ranges::to<std::vector>(view);
        assert(once == twice);
#else
        std::vector<int> once(view.begin(), view.end());
        std::vector<int> twice(view.begin(), view.end());
        assert(once == twice);
#endif

        // 单遍 input 直觉：基于 istream 的 view 第二遍为空/UB —— 勿假设可无限枚举
        // 组合前想清 category：filter 降到 bidirectional；join 可能降到 input
        std::cout << "category drop + materialize discipline OK\n";
    }

    std::cout << "[ranges_expert_pitfalls] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_expert_pitfalls", run>;

}  // namespace
