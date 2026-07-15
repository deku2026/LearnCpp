// Topic     : views::as_rvalue —— 元素当右值 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/as_rvalue_view
//
// 要点: 遍历时 move; 配合 ranges::copy 搬空容器元素。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_as_rvalue_cpp23] ===\n";

    std::vector<std::string> src{"alpha", "beta", "gamma"};
    std::vector<std::string> dst;

    // 通过 as_rvalue 移动进 dst
    for (std::string s : src | std::views::as_rvalue) {
        dst.push_back(std::move(s));
    }
    assert(dst.size() == 3);
    assert(dst[0] == "alpha" && dst[2] == "gamma");
    // 源元素处于有效但未指定状态(通常为空)
    assert(src[0].empty() || src[0] == "alpha");  // 实现可选择
    // 更稳: 检查至少发生了移动语义路径——用 moved-from 可探测技巧
    // 重新演示:
    std::vector<std::string> a{"x", "y"};
    std::vector<std::string> b;
    std::ranges::copy(a | std::views::as_rvalue, std::back_inserter(b));
    assert(b[0] == "x" && b[1] == "y");
    assert(a[0].empty() && a[1].empty());  // 被 move 走

    std::cout << "[as_rvalue] move elements out via view OK\n";
    std::cout << "views_as_rvalue_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_as_rvalue_cpp23", run>;

}  // namespace
