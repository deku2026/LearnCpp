// Topic     : views::join_with —— 展平并插入分隔符 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_join_with_cpp23
// Topic id  : part2/stage10/section08/views_join_with_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/join_with_view

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_join_with_cpp23] 入门：子范围间插分隔符 ===\n";
    {
        std::vector<std::string_view> words{"C++", "is", "fun"};
        std::string sentence;
        for (char c : words | std::views::join_with(' ')) sentence.push_back(c);
        assert(sentence == "C++ is fun");
        std::cout << "space join OK\n";
    }

    std::cout << "=== 进阶：分隔串 + 与 join 对照 ===\n";
    {
        std::vector<std::string_view> parts{"a", "b", "c"};
        std::string csv;
        for (char c : parts | std::views::join_with(std::string_view{", "})) csv.push_back(c);
        assert(csv == "a, b, c");

        // 无分隔：join
        std::string bare;
        for (char c : parts | std::views::join) bare.push_back(c);
        assert(bare == "abc");
        std::cout << "csv delim + plain join OK\n";
    }

    std::cout << "=== 专家：元素级 delim + 单/空输入 ===\n";
    {
        std::vector<std::vector<int>> blocks{{1, 2}, {3}, {4, 5}};
        std::vector<int> flat;
        for (int x : blocks | std::views::join_with(0)) flat.push_back(x);
        assert((flat == std::vector<int>{1, 2, 0, 3, 0, 4, 5}));

        // 单块：无分隔符插入
        std::vector<std::string_view> one{"solo"};
        std::string s1;
        for (char c : one | std::views::join_with('-')) s1.push_back(c);
        assert(s1 == "solo");

        // 空外层
        assert(std::ranges::empty(std::vector<std::string_view>{} | std::views::join_with(',')));

#if defined(__cpp_lib_ranges_join_with)
        std::cout << "__cpp_lib_ranges_join_with=" << __cpp_lib_ranges_join_with << '\n';
#endif
        std::cout << "int delim + edges OK\n";
    }

    std::cout << "[views_join_with_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_join_with_cpp23", run>;

}  // namespace
