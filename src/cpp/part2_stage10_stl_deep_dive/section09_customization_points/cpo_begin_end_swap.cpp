// Topic    : CPO — ranges::begin / end / swap 为何是定制点对象(验收点)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 10.4
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : cpo_begin_end_swap
// Topic id : part2/stage10/section09/cpo_begin_end_swap
// Refs     : https://en.cppreference.com/w/cpp/ranges#Customization_point_objects
//            https://en.cppreference.com/w/cpp/ranges/begin
//            https://en.cppreference.com/w/cpp/algorithm/ranges/swap

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <iostream>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

namespace {

// ── 用户类型 A：提供成员 begin/end ────────────────────────────────
struct MemberRange {
    std::array<int, 3> data{10, 20, 30};

    auto begin() noexcept { return data.begin(); }
    auto end() noexcept { return data.end(); }
    auto begin() const noexcept { return data.begin(); }
    auto end() const noexcept { return data.end(); }
};

// ── 用户类型 B：仅 ADL 自由 begin/end(无成员) ─────────────────────
struct AdlRange {
    std::array<int, 2> data{7, 8};
};

auto begin(AdlRange& r) noexcept {
    return r.data.begin();
}
auto end(AdlRange& r) noexcept {
    return r.data.end();
}
auto begin(const AdlRange& r) noexcept {
    return r.data.begin();
}
auto end(const AdlRange& r) noexcept {
    return r.data.end();
}

// ── 可 swap 的类型：成员 swap + 同命名空间 ADL swap 钩子 ─────────
struct SwapMe {
    int v = 0;
    void swap(SwapMe& other) noexcept { std::ranges::swap(v, other.v); }
};

void swap(SwapMe& a, SwapMe& b) noexcept {
    a.swap(b);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [cpo_begin_end_swap] CPO 三件事：ADL + 约束 + 禁特化 ===\n";

    // ① 数组：ranges::begin 直接返回指针(内建规则)
    {
        int arr[] = {1, 2, 3};
        auto* p = std::ranges::begin(arr);
        auto* e = std::ranges::end(arr);
        assert(p == arr && e == arr + 3);
        assert(*p == 1);
        std::cout << "  array: ranges::begin → &arr[0]\n";
    }

    // ② 成员 begin：CPO 优先走成员(概念约束：返回值是 iterator)
    {
        MemberRange mr;
        auto it = std::ranges::begin(mr);
        assert(*it == 10);
        assert(std::ranges::size(mr) == 3);
        static_assert(std::ranges::range<MemberRange>);
        std::cout << "  member begin/end found by ranges::begin\n";
    }

    // ③ ADL begin：无成员时 CPO 受控 ADL 查找自由函数
    {
        AdlRange ar;
        auto it = std::ranges::begin(ar);
        assert(*it == 7);
        assert(std::distance(std::ranges::begin(ar), std::ranges::end(ar)) == 2);
        static_assert(std::ranges::range<AdlRange>);
        std::cout << "  ADL begin/end found by ranges::begin\n";
    }

    // ④ ranges::swap 是 CPO：成员/ADL swap 优先，否则 move-swap
    {
        SwapMe a{1}, b{2};
        std::ranges::swap(a, b);
        assert(a.v == 2 && b.v == 1);

        int x = 3, y = 4;
        std::ranges::swap(x, y);
        assert(x == 4 && y == 3);
        std::cout << "  ranges::swap uses member/ADL or move-swap\n";
    }

    // ⑤ niebloid 风格算法 CPO：ranges::find 也是定制点式函数对象
    {
        std::vector<int> v{9, 8, 7, 6};
        auto it = std::ranges::find(v, 7);
        assert(it != v.end() && *it == 7);
        std::cout << "  ranges::find is also a CPO/niebloid\n";
    }

    // 教学要点(路线图验收):
    // - ranges::begin 不是可被用户特化的函数模板
    // - 定制路径：成员 begin 或同命名空间 ADL begin(钩子)
    // - CPO 封装：受控查找 + concept 约束 + 禁止错误特化
    std::cout << "[cpo_begin_end_swap] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section09/cpo_begin_end_swap", run>;

}  // namespace
