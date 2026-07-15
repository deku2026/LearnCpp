// Topic    : Ranges 渊源 — range-v3 → C++20 立柱 → C++23 大补齐
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 10.5
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : range_v3_origin
// Topic id : part2/stage10/section09/range_v3_origin
// Refs     : https://github.com/ericniebler/range-v3
//            https://en.cppreference.com/w/cpp/ranges
//            https://en.cppreference.com/w/cpp/ranges/to

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [range_v3_origin] Eric Niebler range-v3 → 标准 Ranges ===\n";

    // 历史脉络(教学):
    //   range-v3 (Eric Niebler) 验证了「惰性 view + 管道 | + 概念」设计
    //   C++20 纳入: 概念 / 基础 view(filter/transform/take/...) / 算法 CPO
    //   C++23 补齐: zip/enumerate/chunk/slide/stride/cartesian_product、
    //               ranges::to、fold 家族……把 range-v3 大量设施标准化
    //   range-v3 仍可能有 actions 等标准暂未覆盖的设施

    // ① C++20 立柱: 惰性管道(来自 range-v3 的经典写法)
    {
        std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto pipe = src | std::views::filter([](int x) { return x % 2 == 0; }) |
                    std::views::transform([](int x) { return x * x; }) | std::views::take(3);

        std::vector<int> out;
        for (int x : pipe) {
            out.push_back(x);
        }
        assert((out == std::vector<int>{4, 16, 36}));
        std::cout << "  C++20 pipeline filter|transform|take → 4 16 36\n";
    }

    // ② C++23 物化 ranges::to —— range-v3 时代常用 to<vector>()
#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    {
        auto squares = std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; }) |
                       std::ranges::to<std::vector>();
        assert((squares == std::vector<int>{1, 4, 9, 16, 25}));
        std::cout << "  C++23 ranges::to<vector> materialize (range-v3 heritage)\n";
    }
#else
    {
        std::vector<int> squares;
        for (int x : std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; })) {
            squares.push_back(x);
        }
        assert((squares == std::vector<int>{1, 4, 9, 16, 25}));
        std::cout << "  (no ranges::to) manual materialize OK\n";
    }
#endif

    // ③ C++23 zip —— range-v3 标志性 view 之一
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    {
        std::vector<std::string> names{"a", "b"};
        std::vector<int> ids{1, 2};
        std::vector<std::pair<std::string, int>> pairs;
        for (auto&& t : std::views::zip(names, ids)) {
            pairs.emplace_back(std::get<0>(t), std::get<1>(t));
        }
        assert(pairs.size() == 2);
        assert(pairs[0].first == "a" && pairs[0].second == 1);
        std::cout << "  C++23 views::zip (from range-v3 lineage)\n";
    }
#else
    std::cout << "  (no views::zip feature macro) skipped zip demo\n";
#endif

    // ④ niebloid 命名渊源: range-v3 算法用函数对象避免 ADL 劫持
    {
        std::vector<int> v{3, 1, 2};
        std::ranges::sort(v);
        assert((v == std::vector<int>{1, 2, 3}));
        std::cout << "  ranges algorithms as niebloids (Niebler + void)\n";
    }

    std::cout << "[range_v3_origin] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section09/range_v3_origin", run>;

}  // namespace
