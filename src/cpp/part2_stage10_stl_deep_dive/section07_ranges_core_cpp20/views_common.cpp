// Topic     : views::common —— 转成 common_range 兼容旧算法
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/common_view
//
// 要点: begin/end 同型; 给 std::vector(it,it) / 旧 std:: 算法用。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_common] ===\n";

    auto taken = std::views::iota(0) | std::views::take(5);
    // 可能非 common → 包一层
    auto c = taken | std::views::common;
    static_assert(std::ranges::common_range<decltype(c)>);

    std::vector<int> v(c.begin(), c.end());
    assert((v == std::vector<int>{0, 1, 2, 3, 4}));

    // 旧 accumulate 要同型迭代器
    const int sum = std::accumulate(c.begin(), c.end(), 0);
    assert(sum == 10);

    // 已是 common 时 common 仍可用(可能 no-op 包装)
    std::vector<int> base{1, 2, 3};
    auto c2 = base | std::views::common;
    assert(std::ranges::equal(c2, base));

    std::cout << "[common] iota|take → vector/accumulate OK\n";
    std::cout << "views_common: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_common", run>;

}  // namespace
