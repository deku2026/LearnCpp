// Topic     : common_range —— begin/end 同类型
// Doc       : 第2部分-阶段10 · 步骤 10.2 / 10.3
// cppreference: https://en.cppreference.com/cpp/ranges/common_range
//
// 要点: 传统 STL 假设; iota 无限 / take 某些情况非 common; views::common 适配。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [common_range] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};
    static_assert(std::ranges::common_range<std::vector<int>>);
    static_assert(std::is_same_v<decltype(std::ranges::begin(v)), decltype(std::ranges::end(v))>);

    // 有界 iota 是 common
    auto bounded = std::views::iota(0, 5);
    static_assert(std::ranges::common_range<decltype(bounded)>);

    // 无限 iota: end 是 unreachable_sentinel → 非 common
    auto inf = std::views::iota(0);
    static_assert(!std::ranges::common_range<decltype(inf)>);

    // take 在某些底层上 end 为 sentinel
    auto taken = inf | std::views::take(3);
    // 用 views::common 变成同类型 begin/end, 以喂旧算法
    auto common = taken | std::views::common;
    static_assert(std::ranges::common_range<decltype(common)>);
    std::vector<int> out(common.begin(), common.end());  // 旧式迭代器对构造
    assert((out == std::vector<int>{0, 1, 2}));

    std::cout << "[common_range] vector yes; infinite iota no; views::common OK\n";
    std::cout << "common_range: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/common_range", run>;

}  // namespace
