// Topic     : ranges::to —— 物化 view 为容器 (C++23, P1206)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 13.1（验收点）
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : ranges_to_container_cpp23
// Topic id  : part2/stage10/section08/ranges_to_container_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/to
//             P1206R7

#include "learn/topic_registry.hpp"

#include <iostream>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_to_container_cpp23] 入门：管道终结物化 vector ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto evens_squared = v | std::views::filter([](int x) { return x % 2 == 0; }) |
                             std::views::transform([](int x) { return x * x; }) | std::ranges::to<std::vector>();
        assert((evens_squared == std::vector<int>{4, 16, 36, 64, 100}));
        // 物化后是真容器：可随机访问、可反复用
        assert(evens_squared[2] == 36);
        assert(evens_squared.size() == 5);
        std::cout << "filter|transform|to<vector> OK\n";
    }

    std::cout << "=== 进阶：to set/list/string + CTAD 显式类型 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto as_set = v | std::views::take(3) | std::ranges::to<std::set>();
        assert(as_set.size() == 3 && as_set.contains(2));

        auto as_list = v | std::views::take(3) | std::ranges::to<std::list<int>>();
        assert(as_list.front() == 1 && as_list.back() == 3);

        auto explicit_t = std::views::iota(1, 4) | std::ranges::to<std::vector<int>>();
        assert((explicit_t == std::vector<int>{1, 2, 3}));

        std::string s = "hello";
        auto upper_like = s | std::views::transform([](char c) { return static_cast<char>(c - 'a' + 'A'); }) |
                          std::ranges::to<std::string>();
        assert(upper_like == "HELLO");
        std::cout << "set/list/string materialize OK\n";
    }

    std::cout << "=== 专家：安全吞掉临时 + pair→map + feature-test 回退说明 ===\n";
    {
        // 文档纪律：管道夹临时要立刻 to 物化，不能存 view
        auto material = std::vector{1, 2, 3, 4, 5} | std::views::filter([](int x) { return x > 2; }) |
                        std::ranges::to<std::vector>();
        assert((material == std::vector<int>{3, 4, 5}));

        // to map：从 pair 范围
        std::vector<std::pair<std::string, int>> kvs{{"a", 1}, {"b", 2}};
        auto m = kvs | std::ranges::to<std::map>();
        assert(m.at("b") == 2);

#if defined(__cpp_lib_ranges_to_container)
        std::cout << "__cpp_lib_ranges_to_container=" << __cpp_lib_ranges_to_container << '\n';
#else
        // 旧实现回退：for + push_back / ranges::copy 到 back_inserter
        std::cout << "no ranges::to: fallback would be copy+back_inserter\n";
#endif
        std::cout << "temp materialize + map OK\n";
    }

    std::cout << "[ranges_to_container_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_to_container_cpp23", run>;

}  // namespace
