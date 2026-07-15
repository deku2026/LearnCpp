// Topic     : views::transform —— 惰性映射
// Doc       : 第2部分-阶段10 · 步骤 11.1 / 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/transform_view
//
// 要点: 迭代时调用 f; 可改类型; 保持底层 category(大致)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_transform] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};
    auto squares = v | std::views::transform([](int x) { return x * x; });
    std::vector<int> got(squares.begin(), squares.end());
    assert((got == std::vector<int>{1, 4, 9, 16, 25}));

    // 改类型
    auto words = v | std::views::transform([](int x) { return std::string(static_cast<std::size_t>(x), '*'); });
    std::vector<std::string> ws(words.begin(), words.end());
    assert(ws[2] == "***");

    // 经典管道: filter | transform | take
    std::vector<int> pipe_out;
    for (int x : v | std::views::filter([](int x) { return x % 2 == 0; }) |
                     std::views::transform([](int x) { return x * x; }) | std::views::take(2))
        pipe_out.push_back(x);
    assert((pipe_out == std::vector<int>{4, 16}));  // 2², 4²; 6 未算(惰性)

    std::cout << "[transform] map/type-change/lazy pipe OK\n";
    std::cout << "pipe_transform: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_transform", run>;

}  // namespace
