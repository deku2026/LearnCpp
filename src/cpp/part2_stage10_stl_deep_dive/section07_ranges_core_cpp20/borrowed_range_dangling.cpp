// Topic     : borrowed_range 与 ranges::dangling —— 编译期拦悬垂
// Doc       : 第2部分-阶段10 · 步骤 10.2 / 14.2
// cppreference: https://en.cppreference.com/cpp/ranges/borrowed_range
//               https://en.cppreference.com/cpp/ranges/dangling
//
// 要点: 临时拥有型 range 算法返回 dangling; string_view/span 是 borrowed。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [borrowed_range_dangling] ===\n";

    // lvalue 容器: 迭代器可借出
    std::vector<int> v{1, 2, 3, 4};
    static_assert(std::ranges::borrowed_range<std::vector<int>&>);
    static_assert(!std::ranges::borrowed_range<std::vector<int>>);  // rvalue 否

    auto it = std::ranges::find(v, 3);
    assert(it != v.end() && *it == 3);

    // 临时 vector → dangling(不能解引用)
    auto dangle = std::ranges::find(std::vector<int>{1, 2, 3}, 2);
    static_assert(std::is_same_v<decltype(dangle), std::ranges::dangling>);
    // *dangle; // ❌ 编译错误 —— 救了你

    // string_view / span / 有界 iota 是 borrowed_range(不拥有)
    static_assert(std::ranges::borrowed_range<std::string_view>);
    static_assert(std::ranges::borrowed_range<std::span<int>>);
    auto it2 = std::ranges::find(std::string_view{"abc"}, 'b');
    assert(*it2 == 'b');  // OK: view 不拥有, 字面量静态存储

    auto it3 = std::ranges::find(std::views::iota(0, 5), 3);
    assert(*it3 == 3);

    std::cout << "[borrowed/dangling] temp vector → dangling; sv OK\n";
    std::cout << "borrowed_range_dangling: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/borrowed_range_dangling", run>;

}  // namespace
