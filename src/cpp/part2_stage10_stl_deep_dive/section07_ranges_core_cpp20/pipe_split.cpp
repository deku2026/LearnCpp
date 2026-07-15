// Topic     : views::split —— 按分隔符切分
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/split_view
//
// 要点: 产出子范围; 分隔可以是元素或 pattern range。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pipe_split] ===\n";

    std::string_view csv = "a,bb,ccc";
    std::vector<std::string> parts;
    for (auto part : csv | std::views::split(',')) {
        parts.emplace_back(part.begin(), part.end());
    }
    assert(parts.size() == 3);
    assert(parts[0] == "a" && parts[1] == "bb" && parts[2] == "ccc");

    // 连续分隔产生空段
    std::string_view s = "x,,y";
    std::vector<std::string> parts2;
    for (auto part : s | std::views::split(',')) parts2.emplace_back(part.begin(), part.end());
    assert(parts2.size() == 3 && parts2[1].empty());

    // 整型序列按 0 切
    std::vector<int> seq{1, 2, 0, 3, 4, 0, 5};
    std::vector<std::vector<int>> groups;
    for (auto g : seq | std::views::split(0)) {
        groups.emplace_back(g.begin(), g.end());
    }
    assert(groups.size() == 3);
    assert((groups[0] == std::vector<int>{1, 2}));
    assert((groups[2] == std::vector<int>{5}));

    std::cout << "[split] csv/empty-segment/int-delimiter OK\n";
    std::cout << "pipe_split: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_split", run>;

}  // namespace
