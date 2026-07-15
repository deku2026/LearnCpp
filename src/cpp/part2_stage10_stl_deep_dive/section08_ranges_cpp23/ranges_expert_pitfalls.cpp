// Topic     : Ranges 专家级陷阱：悬垂 / 单遍 / category 降级
// Doc       : 第2部分-阶段10 · 步骤 14
// cppreference: https://en.cppreference.com/cpp/ranges
//
// 要点: 别当 LINQ; 管道勿夹临时; 复用要 to; filter 非 random_access。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

std::vector<int> make_vec() {
    return {1, 2, 3, 4, 5};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_expert_pitfalls] ===\n";

    // -------------------------------------------------------------------------
    // ① 生命周期: 夹临时容器 → 悬垂(演示正确写法)
    // -------------------------------------------------------------------------
    // auto bad = make_vec() | std::views::filter([](int x){ return x > 2; });
    // // 临时析构 → bad 悬垂 → 迭代 UB

    auto owned = make_vec();
    auto good = owned | std::views::filter([](int x) { return x > 2; });
    std::vector<int> g(good.begin(), good.end());
    assert((g == std::vector<int>{3, 4, 5}));

    // 或立刻物化
    auto material = make_vec() | std::views::filter([](int x) { return x > 2; }) | std::ranges::to<std::vector>();
    assert(material.size() == 3);

    std::cout << "[lifetime] named storage / ranges::to OK\n";

    // -------------------------------------------------------------------------
    // ② dangling: 临时拥有范围 → 编译期拦截
    // -------------------------------------------------------------------------
    auto d = std::ranges::find(std::vector{1, 2, 3}, 2);
    static_assert(std::is_same_v<decltype(d), std::ranges::dangling>);

    // -------------------------------------------------------------------------
    // ③ category 降级
    // -------------------------------------------------------------------------
    std::vector<int> v{1, 2, 3, 4, 5};
    static_assert(std::ranges::random_access_range<std::vector<int>>);
    auto filtered = v | std::views::filter([](int x) { return x > 1; });
    static_assert(!std::ranges::random_access_range<decltype(filtered)>);
    static_assert(std::ranges::bidirectional_range<decltype(filtered)>);
    // filtered[0]; // ❌

    // zip category 取最弱在概念上成立(两 random → random)
    auto z = std::views::zip(v, v);
    static_assert(std::ranges::random_access_range<decltype(z)>);

    // -------------------------------------------------------------------------
    // ④ view 不是容器: 要复用就物化
    // -------------------------------------------------------------------------
    auto view = v | std::views::transform([](int x) { return x * 2; });
    auto once = std::ranges::to<std::vector>(view);
    auto twice = std::ranges::to<std::vector>(view);  // 可再遍历(底层仍在)
    assert(once == twice);

    // 单遍 input 直觉: 不要假设任意 view 可无限重复枚举
    // (基于 istream 的 view 第二遍为空/UB)

    std::cout << "[pitfalls] dangling/category/materialize discipline OK\n";
    std::cout << "ranges_expert_pitfalls: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_expert_pitfalls", run>;

}  // namespace
