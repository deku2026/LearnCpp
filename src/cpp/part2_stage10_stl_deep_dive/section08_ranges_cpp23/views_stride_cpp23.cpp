// Topic     : views::stride —— 每隔 n 取一个 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_stride_cpp23
// Topic id  : part2/stage10/section08/views_stride_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/stride_view
//             P1899

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_stride_cpp23] 入门：每隔 n 采样 ===\n";
    {
        std::vector<int> got;
        for (int x : std::views::iota(0, 10) | std::views::stride(3)) got.push_back(x);
        assert((got == std::vector<int>{0, 3, 6, 9}));

        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<int> every2;
        for (int x : v | std::views::stride(2)) every2.push_back(x);
        assert((every2 == std::vector<int>{1, 3, 5}));
        std::cout << "stride(3)/stride(2) OK\n";
    }

    std::cout << "=== 进阶：stride(1) 恒等 + 与 take/filter 组合 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        assert(std::ranges::equal(v | std::views::stride(1), v));

        std::vector<int> sample;
        for (int x : v | std::views::stride(2) | std::views::transform([](int x) { return x * 10; }))
            sample.push_back(x);
        assert((sample == std::vector<int>{10, 30, 50}));

        // 大 stride：可能只剩首元素
        std::vector<int> one;
        for (int x : v | std::views::stride(100)) one.push_back(x);
        assert((one == std::vector<int>{1}));
        std::cout << "identity + compose + large stride OK\n";
    }

    std::cout << "=== 专家：sized 语义 + 与 slide 对照 + feature-test ===\n";
    {
        std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7};
        auto s = v | std::views::stride(3);
        // 0,3,6 → size 3
        assert(std::ranges::size(s) == 3);
        assert(s[1] == 3);

        // stride 跳步不重叠；slide 是重叠窗口（对照教学）
        // stride(2) on 6 elems → 3 elems; slide(2) → 5 windows
        assert(std::ranges::distance(v | std::views::stride(2)) == 4);

#if defined(__cpp_lib_ranges_stride)
        std::cout << "__cpp_lib_ranges_stride=" << __cpp_lib_ranges_stride << '\n';
#endif
        std::cout << "sized + contrast-with-slide OK\n";
    }

    std::cout << "[views_stride_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_stride_cpp23", run>;

}  // namespace
