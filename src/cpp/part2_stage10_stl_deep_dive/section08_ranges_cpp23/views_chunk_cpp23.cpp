// Topic     : views::chunk —— 不重叠分块 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.2
// cppreference: https://en.cppreference.com/cpp/ranges/chunk_view
//
// 要点: 切成大小 n 的块; 末块可较短。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_chunk_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    std::vector<std::vector<int>> blocks;
    for (auto block : v | std::views::chunk(3)) {
        blocks.emplace_back(block.begin(), block.end());
    }
    assert(blocks.size() == 3);
    assert((blocks[0] == std::vector<int>{1, 2, 3}));
    assert((blocks[1] == std::vector<int>{4, 5, 6}));
    assert((blocks[2] == std::vector<int>{7}));  // 短末块

    // 批处理感
    int batch = 0;
    for (auto block : v | std::views::chunk(2)) {
        assert(!std::ranges::empty(block));
        ++batch;
    }
    assert(batch == 4);

    std::cout << "[chunk] non-overlapping blocks OK\n";
    std::cout << "views_chunk_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_chunk_cpp23", run>;

}  // namespace
