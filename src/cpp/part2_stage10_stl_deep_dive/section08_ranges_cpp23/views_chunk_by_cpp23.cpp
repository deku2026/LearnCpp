// Topic     : views::chunk_by —— 按相邻谓词分组 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.2
// cppreference: https://en.cppreference.com/cpp/ranges/chunk_by_view
//
// 要点: 相邻满足 pred 则同组; equal_to 可做 run-length 分组。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_chunk_by_cpp23] ===\n";

    std::vector<int> w{1, 2, 2, 3, 3, 3, 1};
    std::vector<std::vector<int>> groups;
    for (auto grp : w | std::views::chunk_by(std::ranges::equal_to{})) {
        groups.emplace_back(grp.begin(), grp.end());
    }
    assert(groups.size() == 4);
    assert((groups[0] == std::vector<int>{1}));
    assert((groups[1] == std::vector<int>{2, 2}));
    assert((groups[2] == std::vector<int>{3, 3, 3}));
    assert((groups[3] == std::vector<int>{1}));

    // 单调非降分段: 相邻 a<=b 同组
    std::vector<int> seq{1, 3, 5, 4, 6, 2};
    std::vector<std::vector<int>> mono;
    for (auto g : seq | std::views::chunk_by(std::ranges::less_equal{})) mono.emplace_back(g.begin(), g.end());
    assert(mono.size() == 3);
    assert((mono[0] == std::vector<int>{1, 3, 5}));
    assert((mono[1] == std::vector<int>{4, 6}));
    assert((mono[2] == std::vector<int>{2}));

    std::cout << "[chunk_by] equal runs + monotonic splits OK\n";
    std::cout << "views_chunk_by_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_chunk_by_cpp23", run>;

}  // namespace
