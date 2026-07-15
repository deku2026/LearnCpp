// Topic     : ranges::to —— 物化 view 为容器 (C++23, P1206)
// Doc       : 第2部分-阶段10 · 步骤 13.1
// cppreference: https://en.cppreference.com/cpp/ranges/to
//
// 要点: 管道终结; 可 to<vector>/set 等; 有 reserve 会用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_to_container_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto evens_squared = v | std::views::filter([](int x) { return x % 2 == 0; }) |
                         std::views::transform([](int x) { return x * x; }) | std::ranges::to<std::vector>();
    assert((evens_squared == std::vector<int>{4, 16, 36, 64, 100}));

    // 物化后可反复随机访问
    assert(evens_squared[2] == 36);
    assert(evens_squared.size() == 5);

    auto as_set = v | std::views::take(3) | std::ranges::to<std::set>();
    assert(as_set.size() == 3 && as_set.contains(2));

    // CTAD 风格 to<vector<int>>()
    auto explicit_t = std::views::iota(1, 4) | std::ranges::to<std::vector<int>>();
    assert((explicit_t == std::vector<int>{1, 2, 3}));

    // 字符串
    std::string s = "hello";
    auto upper_like = s | std::views::transform([](char c) { return static_cast<char>(c - 'a' + 'A'); }) |
                      std::ranges::to<std::string>();
    assert(upper_like == "HELLO");

    std::cout << "[ranges::to] vector/set/string materialize OK\n";
    std::cout << "ranges_to_container_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_to_container_cpp23", run>;

}  // namespace
