// Topic     : <algorithm> 全景：查找 / 排序划分 / 变换 / 集合 / 堆 + erase-remove
// Doc       : 第2部分-阶段10 · 步骤 5.1
// cppreference: https://en.cppreference.com/cpp/algorithm
//
// 要点: 算法与容器解耦(迭代器); remove 不真删; C++20 erase_if 一步到位。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

namespace {

void print_ints(std::string_view tag, const std::vector<int>& v) {
    std::cout << tag;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [algorithm_overview] ===\n";

    // -------------------------------------------------------------------------
    // §查找 / 谓词族
    // -------------------------------------------------------------------------
    std::vector<int> v{5, 2, 8, 1, 9, 2, 4};
    assert(std::find(v.begin(), v.end(), 8) != v.end());
    assert(std::count(v.begin(), v.end(), 2) == 2);
    assert(std::any_of(v.begin(), v.end(), [](int x) { return x > 8; }));
    assert(std::none_of(v.begin(), v.end(), [](int x) { return x < 0; }));
    assert(std::all_of(v.begin(), v.end(), [](int x) { return x > 0; }));

    auto sorted = v;
    std::sort(sorted.begin(), sorted.end());
    assert(std::binary_search(sorted.begin(), sorted.end(), 5));
    [[maybe_unused]] auto [lo, hi] = std::equal_range(sorted.begin(), sorted.end(), 2);
    assert(std::distance(lo, hi) == 2);
    std::cout << "[search] find/count/any_of/binary_search/equal_range OK\n";

    // -------------------------------------------------------------------------
    // §排序 / 划分 / nth_element
    // -------------------------------------------------------------------------
    auto s = v;
    std::partial_sort(s.begin(), s.begin() + 3, s.end());
    assert(s[0] <= s[1] && s[1] <= s[2]);
    assert(s[0] == 1 && s[1] == 2 && s[2] == 2);

    auto p = v;
    [[maybe_unused]] auto mid = std::partition(p.begin(), p.end(), [](int x) { return x % 2 == 0; });
    assert(std::all_of(p.begin(), mid, [](int x) { return x % 2 == 0; }));
    assert(std::none_of(mid, p.end(), [](int x) { return x % 2 == 0; }));

    auto n = v;
    std::nth_element(n.begin(), n.begin() + 3, n.end());
    // n[3] 是排序后下标 3 的元素; 左侧都 ≤ 它, 右侧都 ≥ 它
    for (std::size_t i = 0; i < 3; ++i) assert(n[i] <= n[3]);
    for (std::size_t i = 4; i < n.size(); ++i) assert(n[i] >= n[3]);
    std::cout << "[sort/partition] partial_sort / partition / nth_element OK\n";

    // -------------------------------------------------------------------------
    // §变换 / 生成 / erase-remove
    // -------------------------------------------------------------------------
    std::vector<int> dst;
    std::transform(v.begin(), v.end(), std::back_inserter(dst), [](int x) { return x * 10; });
    assert(dst.front() == 50 && dst.back() == 40);

    std::vector<int> filled(4);
    std::fill(filled.begin(), filled.end(), 7);
    assert(std::all_of(filled.begin(), filled.end(), [](int x) { return x == 7; }));

    std::vector<int> gen(5);
    int seed = 0;
    std::generate(gen.begin(), gen.end(), [&] { return ++seed; });
    assert(gen == std::vector<int>({1, 2, 3, 4, 5}));

    // remove 只重排, 不改 size —— 经典 erase-remove
    std::vector<int> er{1, 2, 3, 2, 4, 2, 5};
    auto logical_end = std::remove(er.begin(), er.end(), 2);
    assert(er.size() == 7);  // 还没真正删
    er.erase(logical_end, er.end());
    assert((er == std::vector<int>{1, 3, 4, 5}));

    // C++20: erase_if 一步到位
    std::vector<int> e2{1, 2, 3, 4, 5, 6};
    std::erase_if(e2, [](int x) { return x % 2 == 0; });
    assert((e2 == std::vector<int>{1, 3, 5}));
    print_ints("[erase-remove]", er);
    std::cout << "[mutate] transform/fill/generate/erase_if OK\n";

    // -------------------------------------------------------------------------
    // §有序集合算法 / 堆
    // -------------------------------------------------------------------------
    std::vector<int> a{1, 3, 5, 7};
    std::vector<int> b{2, 3, 6, 7};
    std::vector<int> uni, inter, diff, merged;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(uni));
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(inter));
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(diff));
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(merged));
    assert((uni == std::vector<int>{1, 2, 3, 5, 6, 7}));
    assert((inter == std::vector<int>{3, 7}));
    assert((diff == std::vector<int>{1, 5}));
    assert(std::is_sorted(merged.begin(), merged.end()));

    std::vector<int> heap{3, 1, 4, 1, 5, 9};
    std::make_heap(heap.begin(), heap.end());
    assert(std::is_heap(heap.begin(), heap.end()));
    assert(heap.front() == 9);
    std::pop_heap(heap.begin(), heap.end());
    heap.pop_back();
    assert(heap.front() == 5);
    std::cout << "[set/heap] set_* / merge / make_heap OK\n";

    std::cout << "algorithm_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/algorithm_overview", run>;

}  // namespace
