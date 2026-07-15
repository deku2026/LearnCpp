// Topic     : views::filter —— 惰性过滤
// Doc       : 第2部分-阶段10 · 步骤 11.1 / 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/filter_view
//
// 要点: 迭代时才判定; 降 category(非 random_access); 谓词要稳定。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_filter] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });

    std::vector<int> got;
    for (int x : evens) got.push_back(x);
    assert((got == std::vector<int>{2, 4, 6, 8, 10}));

    // category 降级: 不再 random_access
    static_assert(std::ranges::random_access_range<std::vector<int>>);
    static_assert(!std::ranges::random_access_range<decltype(evens)>);
    static_assert(std::ranges::bidirectional_range<decltype(evens)>);
    // evens[0]; // ❌

    // 组合: filter → transform
    std::vector<int> sq;
    for (int x :
         v | std::views::filter([](int x) { return x > 5; }) | std::views::transform([](int x) { return x * x; }))
        sq.push_back(x);
    assert((sq == std::vector<int>{36, 49, 64, 81, 100}));

    std::cout << "[filter] evens + category drop + pipe combo OK\n";
    std::cout << "pipe_filter: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_filter", run>;

}  // namespace
