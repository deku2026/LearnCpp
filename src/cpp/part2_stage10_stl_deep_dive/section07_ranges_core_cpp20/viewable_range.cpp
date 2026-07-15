// Topic     : viewable_range —— 能安全转成 view 的范围
// Doc       : 第2部分-阶段10 · 步骤 10.2
// cppreference: https://en.cppreference.com/cpp/ranges/viewable_range
//
// 要点: 管道入口约束; views::all 把容器变成 ref_view。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [viewable_range] ===\n";

    std::vector<int> v{1, 2, 3};
    static_assert(std::ranges::viewable_range<std::vector<int>&>);
    static_assert(std::ranges::viewable_range<std::vector<int>>);  // rvalue 也可(移动进 owning_view)

    auto all = std::views::all(v);
    static_assert(std::ranges::view<decltype(all)>);
    assert(std::ranges::size(all) == 3);
    assert(std::ranges::equal(all, v));

    // 管道要求左侧 viewable_range
    auto pipeline = v | std::views::transform([](int x) { return x + 1; });
    std::vector<int> out;
    for (int x : pipeline) out.push_back(x);
    assert((out == std::vector<int>{2, 3, 4}));

    // 已是 view 也是 viewable
    static_assert(std::ranges::viewable_range<decltype(pipeline)>);

    std::cout << "[viewable_range] views::all + pipe entry OK\n";
    std::cout << "viewable_range: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/viewable_range", run>;

}  // namespace
