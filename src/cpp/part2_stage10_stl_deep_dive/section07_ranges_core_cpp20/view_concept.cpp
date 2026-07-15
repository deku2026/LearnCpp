// Topic     : ranges::view —— 轻量、可管道的范围
// Doc       : 第2部分-阶段10 · 步骤 10.2
// cppreference: https://en.cppreference.com/cpp/ranges/view
//
// 要点: view ⊂ range; O(1) 移动; 通常不拥有(或廉价拥有); 惰性。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [view_concept] ===\n";

    // 容器是 range 但通常不是 view(拥有元素、移动可能 O(n))
    static_assert(std::ranges::range<std::vector<int>>);
    static_assert(!std::ranges::view<std::vector<int>>);

    // string_view / span / views::* 是 view
    static_assert(std::ranges::view<std::string_view>);
    static_assert(std::ranges::view<decltype(std::views::iota(0, 5))>);

    std::vector<int> v{1, 2, 3, 4, 5};
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
    static_assert(std::ranges::view<decltype(evens)>);

    // 惰性: 构造不求值
    int calls = 0;
    auto squares = v | std::views::transform([&](int x) {
                       ++calls;
                       return x * x;
                   });
    assert(calls == 0);
    std::vector<int> got;
    for (int x : squares) got.push_back(x);
    assert(calls == 5);
    assert((got == std::vector<int>{1, 4, 9, 16, 25}));

    // view 可拷贝/移动(轻量)
    [[maybe_unused]] auto copy = evens;
    assert(std::ranges::distance(copy) == 2);

    std::cout << "[view] not vector; filter/transform lazy OK\n";
    std::cout << "view_concept: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/view_concept", run>;

}  // namespace
