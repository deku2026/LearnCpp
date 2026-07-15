// Topic     : ranges::range 概念：可 begin/end 的可迭代范围
// Doc       : 第2部分-阶段10 · 步骤 10.2
// cppreference: https://en.cppreference.com/cpp/ranges/range
//
// 要点: range = ranges::begin + ranges::end; 算法直接吃范围。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <list>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

template <std::ranges::range R>
std::size_t count_elems(R&& r) {
    std::size_t n = 0;
    for (auto&& x : r) {
        (void)x;
        ++n;
    }
    return n;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [range_concept] ===\n";

    static_assert(std::ranges::range<std::vector<int>>);
    static_assert(std::ranges::range<std::list<int>>);
    static_assert(std::ranges::range<std::string>);
    static_assert(std::ranges::range<std::string_view>);
    static_assert(std::ranges::range<std::span<int>>);
    static_assert(std::ranges::range<int[4]>);

    std::vector<int> v{1, 2, 3};
    assert(std::ranges::begin(v) == v.begin());
    assert(std::ranges::end(v) == v.end());
    assert(count_elems(v) == 3);
    assert(count_elems(std::string_view{"abcd"}) == 4);

    // ranges 算法直接传容器
    assert(std::ranges::find(v, 2) != std::ranges::end(v));
    assert(std::ranges::count(v, 1) == 1);

    // 自定义类型: 提供 begin/end 成员即成 range
    struct Tiny {
        int data[2]{10, 20};
        int* begin() { return data; }
        int* end() { return data + 2; }
        const int* begin() const { return data; }
        const int* end() const { return data + 2; }
    };
    Tiny t;
    static_assert(std::ranges::range<Tiny>);
    assert(count_elems(t) == 2);

    std::cout << "[range] vector/list/string_view/array/custom OK\n";
    std::cout << "range_concept: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/range_concept", run>;

}  // namespace
