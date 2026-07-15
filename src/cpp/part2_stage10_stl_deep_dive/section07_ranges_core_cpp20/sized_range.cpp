// Topic     : sized_range —— O(1) 可知 size
// Doc       : 第2部分-阶段10 · 步骤 10.2
// cppreference: https://en.cppreference.com/cpp/ranges/sized_range
//
// 要点: ranges::size; filter 后通常不再 sized。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sized_range] ===\n";

    static_assert(std::ranges::sized_range<std::vector<int>>);
    static_assert(std::ranges::sized_range<std::list<int>>);  // list::size C++11 O(1)

    std::vector<int> v{1, 2, 3, 4, 5};
    assert(std::ranges::size(v) == 5);

    auto taken = v | std::views::take(3);
    static_assert(std::ranges::sized_range<decltype(taken)>);
    assert(std::ranges::size(taken) == 3);

    // filter 一般不是 sized_range(无法 O(1) 知多少通过)
    auto filtered = v | std::views::filter([](int x) { return x > 2; });
    static_assert(!std::ranges::sized_range<decltype(filtered)>);
    // 仍可 distance(可能线性)
    assert(std::ranges::distance(filtered) == 3);

    auto iota = std::views::iota(0, 10);
    static_assert(std::ranges::sized_range<decltype(iota)>);
    assert(std::ranges::size(iota) == 10);

    // 无限 iota 不是 sized
    auto inf = std::views::iota(0);
    static_assert(!std::ranges::sized_range<decltype(inf)>);

    std::cout << "[sized_range] vector/take/iota yes; filter/infinite no\n";
    std::cout << "sized_range: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/sized_range", run>;

}  // namespace
