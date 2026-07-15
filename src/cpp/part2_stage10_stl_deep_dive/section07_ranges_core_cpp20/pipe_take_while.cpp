// Topic     : views::take_while —— 取到谓词为假
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 11.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : pipe_take_while
// Topic id  : part2/stage10/section07/pipe_take_while
// Refs      : https://en.cppreference.com/w/cpp/ranges/take_while_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_take_while] 入门：前缀满足谓词则取 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 0, 6};
        std::vector<int> got;
        for (int x : v | std::views::take_while([](int x) { return x < 4; })) got.push_back(x);
        assert((got == std::vector<int>{1, 2, 3}));
        std::cout << "take while <4 OK\n";
    }

    std::cout << "=== 进阶：空/全取 + 与 drop_while 互补 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        assert(std::ranges::empty(v | std::views::take_while([](int) { return false; })));
        assert(std::ranges::equal(v | std::views::take_while([](int) { return true; }), v));

        // 互补：take_while(p) + drop_while(p) 拼回原序列
        auto p = [](int x) { return x < 3; };
        std::vector<int> head, tail;
        for (int x : v | std::views::take_while(p)) head.push_back(x);
        for (int x : v | std::views::drop_while(p)) tail.push_back(x);
        std::vector<int> joined = head;
        joined.insert(joined.end(), tail.begin(), tail.end());
        assert(joined == v);
        std::cout << "complement with drop_while OK\n";
    }

    std::cout << "=== 专家：遇假即停（中间再真也不恢复）+ 无限源 ===\n";
    {
        std::vector<int> v{2, 4, 1, 6, 8};
        std::vector<int> got;
        for (int x : v | std::views::take_while([](int x) { return x % 2 == 0; })) got.push_back(x);
        // 取 2,4；遇 1 停；后面 6,8 不取
        assert((got == std::vector<int>{2, 4}));

        // 无限 iota 上 take_while：惰性终止
        std::vector<int> small;
        for (int x : std::views::iota(0) | std::views::take_while([](int x) { return x < 4; })) small.push_back(x);
        assert((small == std::vector<int>{0, 1, 2, 3}));
        std::cout << "stop-on-false + infinite source OK\n";
    }

    std::cout << "[pipe_take_while] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take_while", run>;

}  // namespace
