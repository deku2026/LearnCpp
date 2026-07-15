// Topic     : views::join_with —— 展平并插入分隔符 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.3
// cppreference: https://en.cppreference.com/cpp/ranges/join_with_view
//
// 要点: 类似 join 但在子范围之间插入 delim。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_join_with_cpp23] ===\n";

    std::vector<std::string_view> words{"C++", "is", "fun"};
    std::string sentence;
    for (char c : words | std::views::join_with(' ')) sentence.push_back(c);
    assert(sentence == "C++ is fun");

    // 分隔串
    std::vector<std::string_view> parts{"a", "b", "c"};
    std::string csv;
    for (char c : parts | std::views::join_with(std::string_view{", "})) csv.push_back(c);
    assert(csv == "a, b, c");

    // 整数块
    std::vector<std::vector<int>> blocks{{1, 2}, {3}, {4, 5}};
    std::vector<int> flat;
    for (int x : blocks | std::views::join_with(0)) flat.push_back(x);
    assert((flat == std::vector<int>{1, 2, 0, 3, 0, 4, 5}));

    std::cout << "[join_with] space/csv/int-delim OK\n";
    std::cout << "views_join_with_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_join_with_cpp23", run>;

}  // namespace
