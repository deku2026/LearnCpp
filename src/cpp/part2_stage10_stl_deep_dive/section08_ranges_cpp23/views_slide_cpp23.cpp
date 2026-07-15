// Topic     : views::slide —— 运行期滑动窗口 (C++23, 验收点)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_slide_cpp23
// Topic id  : part2/stage10/section08/views_slide_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/slide_view
//             P2442

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>
#include <version>

#if !defined(__cpp_lib_ranges_chunk) || !(__cpp_lib_ranges_chunk)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::views::chunk not available (__cpp_lib_ranges_chunk)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_slide_cpp23", run>;
}  // namespace
#else
namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_slide_cpp23] 入门：重叠滑动窗口 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<std::vector<int>> windows;
        for (auto window : v | std::views::slide(3)) {
            windows.emplace_back(window.begin(), window.end());
        }
        assert(windows.size() == 4);
        assert((windows[0] == std::vector<int>{1, 2, 3}));
        assert((windows[3] == std::vector<int>{4, 5, 6}));
        std::cout << "slide(3) windows OK\n";
    }

    std::cout << "=== 进阶：相邻差 + 移动平均教学 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<int> diffs;
        for (auto w : v | std::views::slide(2)) {
            auto it = w.begin();
            int a = *it++;
            int b = *it;
            diffs.push_back(b - a);
        }
        assert((diffs == std::vector<int>{1, 1, 1, 1, 1}));

        // 窗口和（简单移动和）
        std::vector<int> sums;
        for (auto w : v | std::views::slide(3)) {
            sums.push_back(std::accumulate(w.begin(), w.end(), 0));
        }
        assert((sums == std::vector<int>{6, 9, 12, 15}));
        std::cout << "diffs + window sums OK\n";
    }

    std::cout << "=== 专家：vs adjacent/chunk + n>size 空 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        // n > size → 空
        assert(std::ranges::empty(v | std::views::slide(100)));
        // slide(1) 每个元素一窗
        assert(std::ranges::distance(v | std::views::slide(1)) == 6);

        // adjacent<2> 产出 tuple；slide(2) 产出子范围（运行期 n）
        std::vector<int> adj_prod;
        for (auto [a, b] : v | std::views::adjacent<2>) adj_prod.push_back(a * b);
        assert(adj_prod.size() == 5);

        // chunk 不重叠：chunk(3) 窗口数 2；slide(3) 窗口数 4
        assert(std::ranges::distance(v | std::views::chunk(3)) == 2);
        assert(std::ranges::distance(v | std::views::slide(3)) == 4);

#if defined(__cpp_lib_ranges_slide)
        std::cout << "__cpp_lib_ranges_slide=" << __cpp_lib_ranges_slide << '\n';
#endif
        std::cout << "vs adjacent/chunk OK\n";
    }

    std::cout << "[views_slide_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_slide_cpp23", run>;

}  // namespace
#endif  // __cpp_lib_ranges_chunk
