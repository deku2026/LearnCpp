// Topic     : views::adjacent / adjacent_transform / pairwise (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 12.2
// Stage     : part2_stage10_stl_deep_dive
// Section   : section08_ranges_cpp23
// Item      : views_adjacent_pairwise_cpp23
// Topic id  : part2/stage10/section08/views_adjacent_pairwise_cpp23
// Refs      : https://en.cppreference.com/w/cpp/ranges/adjacent_view
//             P2321 / P2387

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

#if !defined(__cpp_lib_ranges_slide) || !(__cpp_lib_ranges_slide)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::views::slide not available (__cpp_lib_ranges_slide)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_adjacent_pairwise_cpp23", run>;
}  // namespace
#else
namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_adjacent_pairwise_cpp23] 入门：相邻 N 元组 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<int> pair_sums;
        for (auto [a, b] : v | std::views::adjacent<2>) pair_sums.push_back(a + b);
        assert((pair_sums == std::vector<int>{3, 5, 7, 9, 11}));

        // pairwise ≡ adjacent<2>
        std::vector<int> same;
        for (auto [a, b] : v | std::views::pairwise) same.push_back(a + b);
        assert(same == pair_sums);
        std::cout << "adjacent<2>/pairwise OK\n";
    }

    std::cout << "=== 进阶：adjacent_transform + adjacent<3> ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        std::vector<int> products;
        for (int p : v | std::views::adjacent_transform<2>(std::multiplies{})) products.push_back(p);
        assert((products == std::vector<int>{2, 6, 12, 20, 30}));

        [[maybe_unused]] int triple_count = 0;
        for (auto [a, b, c] : v | std::views::adjacent<3>) {
            (void)a;
            (void)b;
            (void)c;
            (void)a;
            (void)b;
            (void)c;
            assert(b == a + 1 && c == b + 1);
            ++triple_count;
        }
        assert(triple_count == 4);
        std::cout << "transform + triple OK\n";
    }

    std::cout << "=== 专家：vs slide + 长度 < N 空 + 编译期 N ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        // 长度 < N → 空
        assert(std::ranges::empty(std::vector{1, 2} | std::views::adjacent<3>));

        // adjacent 编译期 N、产出 tuple；slide 运行期 n、产出子范围
        assert(std::ranges::distance(v | std::views::adjacent<2>) == std::ranges::distance(v | std::views::slide(2)));

        // 窗口写回：tuple 元素是引用时可改底层
        for (auto [a, b] : v | std::views::adjacent<2>) {
            (void)a;
            (void)b;
            (void)a;
            (void)b;
            break;
        }
        assert(v.front() == 1);

#if defined(__cpp_lib_ranges_zip)  // adjacent 随 zip 提案族落地
        std::cout << "adjacent with ranges zip family available\n";
#endif
        std::cout << "vs slide + short-range empty OK\n";
    }

    std::cout << "[views_adjacent_pairwise_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_adjacent_pairwise_cpp23", run>;

}  // namespace
#endif  // __cpp_lib_ranges_slide
