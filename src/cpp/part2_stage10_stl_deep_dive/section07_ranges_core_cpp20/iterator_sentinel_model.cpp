// Topic     : iterator-sentinel 分离模型
// Doc       : 第2部分-阶段10 · 步骤 10.3
// cppreference: https://en.cppreference.com/cpp/iterator/sentinel_for
//
// 要点: end 可不同类型; 无限范围 / 惰性终止; unreachable_sentinel。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterator_sentinel_model] ===\n";

    // 传统: begin/end 同型
    std::vector<int> v{1, 2, 3};
    static_assert(std::is_same_v<decltype(v.begin()), decltype(v.end())>);

    // 无限 iota: end 是 unreachable_sentinel_t
    auto inf = std::views::iota(1);
    using It = std::ranges::iterator_t<decltype(inf)>;
    using Sent = std::ranges::sentinel_t<decltype(inf)>;
    static_assert(!std::is_same_v<It, Sent>);
    static_assert(std::is_same_v<Sent, std::unreachable_sentinel_t>);

    // take 提供可比较的哨兵, 取前 5
    std::vector<int> first5;
    for (int x : inf | std::views::take(5)) first5.push_back(x);
    assert((first5 == std::vector<int>{1, 2, 3, 4, 5}));

    // 惰性终止: take_while
    std::vector<int> under10;
    for (int x : inf | std::views::take_while([](int x) { return x < 10; })) under10.push_back(x);
    assert(under10.size() == 9 && under10.back() == 9);

    // subrange 可显式绑不同型 iterator/sentinel
    auto r = std::ranges::subrange(v.begin(), v.end());
    assert(std::ranges::size(r) == 3);

    std::cout << "[iterator-sentinel] infinite iota + take/take_while OK\n";
    std::cout << "iterator_sentinel_model: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/iterator_sentinel_model", run>;

}  // namespace
