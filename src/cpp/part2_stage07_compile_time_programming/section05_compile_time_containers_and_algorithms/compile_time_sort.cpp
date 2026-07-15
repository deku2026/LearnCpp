// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_sort
// Topic id : part2/stage07/section05/compile_time_sort
//
// Refs:
//   https://en.cppreference.com/w/cpp/algorithm/sort
//   C++20: most algorithms are constexpr
//   P0784 / constexpr containers enable real compile-time pipelines

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — constexpr std::sort on std::array
// ---------------------------------------------------------------------------

constexpr auto sorted_copy(std::array<int, 5> a) {
    std::sort(a.begin(), a.end());
    return a;
}

constexpr auto kSorted = sorted_copy({5, 1, 4, 2, 3});

// ---------------------------------------------------------------------------
// §进阶 — vector 工作区 + 算法链 + 写回 array
// ---------------------------------------------------------------------------

constexpr int sum_of_sorted_vector() {
    std::vector<int> v{9, 4, 1, 8, 2, 7};
    std::sort(v.begin(), v.end());
    return std::accumulate(v.begin(), v.end(), 0);
}

constexpr auto unique_sorted_array() {
    std::vector<int> v{3, 1, 2, 1, 3, 2, 4};
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::array<int, 4> out{};
    for (std::size_t i = 0; i < out.size() && i < v.size(); ++i) {
        out[i] = v[i];
    }
    return out;
}

constexpr int nth_after_partial_sort() {
    std::array<int, 7> a{7, 1, 5, 3, 9, 2, 8};
    std::partial_sort(a.begin(), a.begin() + 3, a.end());
    // 前 3 个是最小的三个，且有序
    return a[0] + a[1] + a[2];
}

// ---------------------------------------------------------------------------
// §专家 — 自定义比较、稳定性、编译期成本
// ---------------------------------------------------------------------------

struct Item {
    int key;
    int weight;
};

constexpr auto sort_items_by_key() {
    std::array<Item, 4> items{{
        {2, 10},
        {1, 40},
        {4, 20},
        {3, 30},
    }};
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) { return a.key < b.key; });
    return items;
}

// 手写稳定插入排序：展示「等价键保序」；部分 STL 的 std::stable_sort
// 尚未标成 constexpr（MSVC STL 上常见），故不用它做 static_assert。
constexpr int stable_demo_first_weight() {
    std::array<Item, 4> items{{
        {1, 100},
        {2, 1},
        {1, 200},
        {2, 2},
    }};
    for (std::size_t i = 1; i < items.size(); ++i) {
        Item key = items[i];
        std::size_t j = i;
        while (j > 0 && key.key < items[j - 1].key) {
            items[j] = items[j - 1];
            --j;
        }
        items[j] = key;
    }
    // key==1 的两项应保持 100 在 200 前
    return items[0].weight;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/compile_time_sort ===\n";

    static_assert(kSorted[0] == 1 && kSorted[4] == 5);
    static_assert(sum_of_sorted_vector() == 31);
    constexpr auto uniq = unique_sorted_array();
    static_assert(uniq[0] == 1 && uniq[1] == 2 && uniq[2] == 3 && uniq[3] == 4);
    static_assert(nth_after_partial_sort() == 1 + 2 + 3);

    constexpr auto items = sort_items_by_key();
    static_assert(items[0].key == 1 && items[3].key == 4);
    static_assert(stable_demo_first_weight() == 100);

    std::cout << "[intro] sorted {5,1,4,2,3} -> ";
    for (std::size_t i = 0; i < kSorted.size(); ++i) {
        std::cout << kSorted[i] << (i + 1 < kSorted.size() ? "," : "");
    }
    std::cout << '\n';

    std::cout << "[advanced] sum_of_sorted_vector=" << sum_of_sorted_vector() << " unique_sorted first=" << uniq[0]
              << " last=" << uniq[3] << '\n';

    std::cout << "[expert] custom key sort first weight=" << items[0].weight
              << " stable_sort preserved weight=" << stable_demo_first_weight() << '\n';

    // 运行期同样算法
    std::array<int, 5> rt{5, 1, 4, 2, 3};
    std::sort(rt.begin(), rt.end());
    assert(rt == kSorted);

    std::cout << "compile_time_sort: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_sort", run>;

}  // namespace
