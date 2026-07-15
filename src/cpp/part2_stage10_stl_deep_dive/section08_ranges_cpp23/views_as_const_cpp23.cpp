// Topic     : views::as_const —— 元素变 const (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/as_const_view
//
// 要点: 只读视图; 防止管道中误改; range_reference_t 为 const&。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_as_const_cpp23] ===\n";

    std::vector<int> v{1, 2, 3};
    auto cv = v | std::views::as_const;
    using Ref = std::ranges::range_reference_t<decltype(cv)>;
    static_assert(std::is_same_v<Ref, const int&>);

    [[maybe_unused]] int sum = 0;
    for (const int& x : cv) sum += x;
    assert(sum == 6);

    // 原容器仍可写; 视图只读
    v[0] = 10;
    assert(*std::ranges::begin(cv) == 10);

    // 与 transform 组合: 只读输入
    std::vector<int> doubled;
    for (int x : v | std::views::as_const | std::views::transform([](const int& x) { return x * 2; }))
        doubled.push_back(x);
    assert((doubled == std::vector<int>{20, 4, 6}));

    std::cout << "[as_const] const reference view OK\n";
    std::cout << "views_as_const_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_as_const_cpp23", run>;

}  // namespace
