// Topic     : views::transform —— 惰性映射
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.1 / 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_transform
// Topic id  : part2/stage10/section07/pipe_transform
// Refs      : https://en.cppreference.com/w/cpp/ranges/transform_view

#include "learn/topic_registry.hpp"

#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_transform] 入门：逐元素映射（惰性）===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        std::vector<int> sq;
        for (int x : v | std::views::transform([](int x) { return x * x; })) sq.push_back(x);
        assert((sq == std::vector<int>{1, 4, 9, 16}));
        std::cout << "square map OK\n";
    }

    std::cout << "=== 进阶：类型变化 + 管道组合 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        std::vector<std::string> labels;
        for (auto s : v | std::views::transform([](int x) { return "n=" + std::to_string(x); })) labels.push_back(s);
        assert(labels[0] == "n=1" && labels[2] == "n=3");

        // transform 保持 random_access（若底层是）
        auto t = v | std::views::transform([](int x) { return x + 1; });
        static_assert(std::ranges::random_access_range<decltype(t)>);
        assert(t[1] == 3);

        // filter 后再 transform（文档管道）
        std::vector<int> v2{1, 2, 3, 4, 5, 6};
        std::vector<int> out;
        for (int x : v2 | std::views::filter([](int x) { return x % 2 == 0; }) |
                         std::views::transform([](int x) { return x * 10; })) {
            out.push_back(x);
        }
        assert((out == std::vector<int>{20, 40, 60}));
        std::cout << "type change + filter|transform OK\n";
    }

    std::cout << "=== 专家：引用投影陷阱 + 不修改源 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        // transform 默认不写回源（除非返回引用并赋值）
        auto doubled = v | std::views::transform([](int x) { return x * 2; });
        std::vector<int> got(doubled.begin(), doubled.end());
        assert((got == std::vector<int>{2, 4, 6}));
        assert((v == std::vector<int>{1, 2, 3}));  // 源未改

        // 通过返回引用写回（高级，需谨慎）
        auto ref_view = v | std::views::transform([](int& x) -> int& { return x; });
        ref_view[0] = 99;
        assert(v[0] == 99);

        // iota + transform：生成平方序列
        std::vector<int> squares;
        for (int x : std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; })) squares.push_back(x);
        assert((squares == std::vector<int>{1, 4, 9, 16, 25}));
        std::cout << "non-mutating + ref write-back + iota OK\n";
    }

    std::cout << "[pipe_transform] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_transform", run>;

}  // namespace
