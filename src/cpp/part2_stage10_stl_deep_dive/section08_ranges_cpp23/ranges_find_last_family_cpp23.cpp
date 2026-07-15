// Topic     : ranges::find_last / find_last_if / find_last_if_not (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.2
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/find_last
//
// 要点: 从后往前找; 返回 subrange[found, end)。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_find_last_family_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 2, 4, 2, 5};

#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
    [[maybe_unused]] auto r = std::ranges::find_last(v, 2);
    assert(!r.empty());
    assert(*r.begin() == 2);
    // 指向最后一个 2
    assert(r.begin() == v.begin() + 5);

    [[maybe_unused]] auto r2 = std::ranges::find_last_if(v, [](int x) { return x > 3; });
    assert(*r2.begin() == 5);

    [[maybe_unused]] auto r3 = std::ranges::find_last_if_not(v, [](int x) { return x < 5; });
    assert(*r3.begin() == 5);

    [[maybe_unused]] auto miss = std::ranges::find_last(v, 99);
    assert(miss.empty());

    // 投影
    struct Item {
        int k;
        char tag;
    };
    std::vector<Item> items{{1, 'a'}, {2, 'b'}, {1, 'c'}};
    [[maybe_unused]] auto last1 = std::ranges::find_last(items, 1, &Item::k);
    assert(last1.begin()->tag == 'c');

    std::cout << "[find_last family] library path OK\n";
#else
    // 回退：反向查找最后一个匹配
    std::cout << "library not yet available, fallback demo (rbegin/rend)\n";
    auto it = std::ranges::find(v.rbegin(), v.rend(), 2);
    assert(it != v.rend());
    assert(*it == 2);
    assert(it.base() - 1 == v.begin() + 5);

    auto it2 = std::ranges::find_if(v.rbegin(), v.rend(), [](int x) { return x > 3; });
    assert(it2 != v.rend() && *it2 == 5);

    auto it3 = std::ranges::find_if_not(v.rbegin(), v.rend(), [](int x) { return x < 5; });
    assert(it3 != v.rend() && *it3 == 5);

    assert(std::ranges::find(v.rbegin(), v.rend(), 99) == v.rend());

    struct Item {
        int k;
        char tag;
    };
    std::vector<Item> items{{1, 'a'}, {2, 'b'}, {1, 'c'}};
    auto last1 = std::ranges::find(items.rbegin(), items.rend(), 1, &Item::k);
    assert(last1 != items.rend() && last1->tag == 'c');

    std::cout << "[find_last family] fallback OK\n";
#endif

    std::cout << "ranges_find_last_family_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_find_last_family_cpp23", run>;

}  // namespace
