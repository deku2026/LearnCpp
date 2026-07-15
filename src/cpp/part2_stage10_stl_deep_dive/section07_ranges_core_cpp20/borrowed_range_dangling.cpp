// Topic     : borrowed_range 与 ranges::dangling —— 编译期拦悬垂
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 10.2 / 14.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section07_ranges_core_cpp20
// Item      : borrowed_range_dangling
// Topic id  : part2/stage10/section07/borrowed_range_dangling
// Refs      : https://en.cppreference.com/w/cpp/ranges/borrowed_range
//             https://en.cppreference.com/w/cpp/ranges/dangling

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [borrowed_range_dangling] 入门：lvalue 可借 / rvalue 容器否 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        static_assert(std::ranges::borrowed_range<std::vector<int>&>);
        static_assert(!std::ranges::borrowed_range<std::vector<int>>);  // rvalue 否

        [[maybe_unused]] auto it = std::ranges::find(v, 3);
        assert(it != v.end() && *it == 3);
        std::cout << "lvalue vector find OK\n";
    }

    std::cout << "=== 进阶：临时拥有范围 → dangling（编译期救你）===\n";
    {
        auto dangle = std::ranges::find(std::vector<int>{1, 2, 3}, 2);
        static_assert(std::is_same_v<decltype(dangle), std::ranges::dangling>);
        // *dangle; // ❌ 编译错误 —— 不能解引用

        // 安全：先具名
        auto tmp = std::vector{1, 2, 3};
        [[maybe_unused]] auto it = std::ranges::find(tmp, 2);
        assert(*it == 2);
        std::cout << "temporary → dangling type OK\n";
    }

    std::cout << "=== 专家：string_view/span/iota 是 borrowed + enable 语义 ===\n";
    {
        // 不拥有元素的 view：即使「临时」传算法也返回真迭代器
        static_assert(std::ranges::borrowed_range<std::string_view>);
        static_assert(std::ranges::borrowed_range<std::span<int>>);
        [[maybe_unused]] auto it2 = std::ranges::find(std::string_view{"abc"}, 'b');
        assert(*it2 == 'b');  // 字面量静态存储

        [[maybe_unused]] auto it3 = std::ranges::find(std::views::iota(0, 5), 3);
        assert(*it3 == 3);

        // 自定义不拥有视图可特化 enable_borrowed_range<T> = true
        // （教学注释：见 cppreference borrowed_range）

        // 对比：管道夹临时容器仍是运行期悬垂（dangling 管不到 pipe 存 view）
        // auto bad = std::vector{1,2,3} | views::filter(...); // 悬垂 view
        std::vector<int> owned{1, 2, 3, 4, 5};
        [[maybe_unused]] auto good = owned | std::views::filter([](int x) { return x > 2; });
        assert(std::ranges::distance(good) == 3);
        std::cout << "borrowed views + named pipe OK\n";
    }

    std::cout << "[borrowed_range_dangling] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/borrowed_range_dangling", run>;

}  // namespace
