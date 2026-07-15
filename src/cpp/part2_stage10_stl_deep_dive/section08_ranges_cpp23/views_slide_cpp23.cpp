// Topic     : views::slide —— 运行期滑动窗口 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.2
// cppreference: https://en.cppreference.com/cpp/ranges/slide_view
//
// 要点: 产出子范围; 窗口大小运行期; 重叠滑动。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_slide_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6};

    std::vector<std::vector<int>> windows;
    for (auto window : v | std::views::slide(3)) {
        windows.emplace_back(window.begin(), window.end());
    }
    assert(windows.size() == 4);
    assert((windows[0] == std::vector<int>{1, 2, 3}));
    assert((windows[3] == std::vector<int>{4, 5, 6}));

    // 相邻差: slide(2)
    std::vector<int> diffs;
    for (auto w : v | std::views::slide(2)) {
        auto it = w.begin();
        int a = *it++;
        int b = *it;
        diffs.push_back(b - a);
    }
    assert((diffs == std::vector<int>{1, 1, 1, 1, 1}));

    // n > size → 空
    assert(std::ranges::empty(v | std::views::slide(100)));

    std::cout << "[slide] overlapping windows + diffs OK\n";
    std::cout << "views_slide_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_slide_cpp23", run>;

}  // namespace
