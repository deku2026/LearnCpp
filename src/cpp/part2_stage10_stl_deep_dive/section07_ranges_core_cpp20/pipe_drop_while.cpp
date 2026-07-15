// Topic     : views::drop_while —— 跳到谓词为假
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/drop_while_view
//
// 要点: 跳过前缀满足谓词的元素; 首次假后保留剩余(含该元素)。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_drop_while] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 1, 2};
    auto rest = v | std::views::drop_while([](int x) { return x < 4; });
    std::vector<int> got(rest.begin(), rest.end());
    assert((got == std::vector<int>{4, 5, 1, 2}));

    // 全满足 → 空
    assert(std::ranges::empty(v | std::views::drop_while([](int x) { return x < 100; })));

    // 首元素就不满足 → 原样
    assert(std::ranges::equal(v | std::views::drop_while([](int x) { return x < 0; }), v));

    std::cout << "[drop_while] skip-prefix OK\n";
    std::cout << "pipe_drop_while: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop_while", run>;

}  // namespace
