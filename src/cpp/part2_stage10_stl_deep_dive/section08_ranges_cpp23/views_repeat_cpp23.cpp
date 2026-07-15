// Topic     : views::repeat —— 重复值 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_repeat_cpp23
// Topic id  : part2/stage10/section08/views_repeat_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/repeat_view
//             P2474

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_repeat_cpp23] 入门：有界重复 ===\n";
    {
        std::vector<int> got;
        for (int x : std::views::repeat(7, 3)) got.push_back(x);
        assert((got == std::vector<int>{7, 7, 7}));
        assert(std::ranges::empty(std::views::repeat(1, 0)));
        std::cout << "repeat(7,3) OK\n";
    }

    std::cout << "=== 进阶：无限 repeat + take 截断 ===\n";
    {
        std::vector<int> got;
        for (int x : std::views::repeat(42) | std::views::take(4)) got.push_back(x);
        assert((got == std::vector<int>{42, 42, 42, 42}));

        // 与 transform 组合：填充常量序列
        std::vector<int> zeros;
        for (int x : std::views::repeat(0, 5) | std::views::transform([](int x) { return x + 1; })) zeros.push_back(x);
        assert((zeros == std::vector<int>{1, 1, 1, 1, 1}));
        std::cout << "unbounded + take/transform OK\n";
    }

    std::cout << "=== 专家：sized + random_access + 非拥有语义 ===\n";
    {
        auto r = std::views::repeat('x', 4);
        static_assert(std::ranges::sized_range<decltype(r)>);
        static_assert(std::ranges::random_access_range<decltype(r)>);
        assert(std::ranges::size(r) == 4);
        assert(r[2] == 'x');

        // repeat 存值副本（或可拷贝对象），不是指向外部容器
        int v = 9;
        [[maybe_unused]] auto rv = std::views::repeat(v, 2);
        v = 0;
        // 视图内已拷贝 9，不受 v 后续修改影响（值语义）
        assert(*rv.begin() == 9);

#if defined(__cpp_lib_ranges_repeat)
        std::cout << "__cpp_lib_ranges_repeat=" << __cpp_lib_ranges_repeat << '\n';
#endif
        std::cout << "category + value copy OK\n";
    }

    std::cout << "[views_repeat_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_repeat_cpp23", run>;

}  // namespace
