// Topic     : views::chunk —— 不重叠分块 (C++23, 验收点)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_chunk_cpp23
// Topic id  : part2/stage10/section08/views_chunk_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/chunk_view
//             P2442

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_chunk_cpp23] 入门：不重叠切块 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
        std::vector<std::vector<int>> blocks;
        for (auto block : v | std::views::chunk(3)) {
            blocks.emplace_back(block.begin(), block.end());
        }
        assert(blocks.size() == 3);
        assert((blocks[0] == std::vector<int>{1, 2, 3}));
        assert((blocks[1] == std::vector<int>{4, 5, 6}));
        assert((blocks[2] == std::vector<int>{7}));  // 短末块
        std::cout << "chunk(3) with short tail OK\n";
    }

    std::cout << "=== 进阶：批处理 + 与 slide 对照 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
        int batch = 0;
        for (auto block : v | std::views::chunk(2)) {
            assert(!std::ranges::empty(block));
            ++batch;
        }
        assert(batch == 4);  // [1,2][3,4][5,6][7]

        // chunk 不重叠；slide 重叠
        assert(std::ranges::distance(v | std::views::chunk(3)) == 3);
        assert(std::ranges::distance(v | std::views::slide(3)) == 5);
        std::cout << "batch count + vs slide OK\n";
    }

    std::cout << "=== 专家：chunk(1)/整除/空 + feature-test ===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        int n1 = 0;
        for (auto b : v | std::views::chunk(1)) {
            assert(std::ranges::size(b) == 1);
            ++n1;
        }
        assert(n1 == 4);

        // 整除：无短块
        std::vector<std::vector<int>> even;
        for (auto b : v | std::views::chunk(2)) even.emplace_back(b.begin(), b.end());
        assert(even.size() == 2 && even[1].size() == 2);

        assert(std::ranges::empty(std::vector<int>{} | std::views::chunk(3)));

#if defined(__cpp_lib_ranges_chunk)
        std::cout << "__cpp_lib_ranges_chunk=" << __cpp_lib_ranges_chunk << '\n';
#endif
        std::cout << "chunk edges OK\n";
    }

    std::cout << "[views_chunk_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_chunk_cpp23", run>;

}  // namespace
