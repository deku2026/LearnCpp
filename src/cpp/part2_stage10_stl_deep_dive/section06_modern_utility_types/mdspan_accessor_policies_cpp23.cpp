// Topic     : mdspan accessor policy：如何从句柄取元素
// Doc       : 第2部分-阶段10 · 步骤 8.5
// cppreference: https://en.cppreference.com/cpp/container/mdspan/default_accessor
//
// 要点: default_accessor 直接解引用; 可定制只读/缩放/检查等正交策略。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <mdspan>
#include <vector>

namespace {

// 教学用 checked accessor: 访问前断言偏移在 span 内(演示可替换性)
// 生产环境可用类似思路做边界检查 / 原子 / GPU 句柄等
template <class ElementType>
struct checked_accessor {
    using offset_policy = checked_accessor;
    using element_type = ElementType;
    using reference = ElementType&;
    using data_handle_type = ElementType*;

    std::size_t n{0};

    constexpr checked_accessor() = default;
    constexpr explicit checked_accessor(std::size_t size) : n(size) {}

    constexpr reference access(data_handle_type p, std::size_t i) const {
        assert(i < n);
        return p[i];
    }
    constexpr typename offset_policy::data_handle_type offset(data_handle_type p, std::size_t i) const {
        assert(i <= n);
        return p + i;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mdspan_accessor_policies_cpp23] ===\n";

    std::vector<int> data{1, 2, 3, 4, 5, 6};
    using Ext = std::extents<std::size_t, 2, 3>;

    // 默认 accessor: 可读写引用
    std::mdspan<int, Ext> m(data.data());
    m[0, 0] = 10;
    assert(data[0] == 10);
    // assert 是宏：多维下标中的逗号需额外括号保护
    assert((m[1, 2] == 6));

    // default_accessor 显式写出
    std::mdspan<int, Ext, std::layout_right, std::default_accessor<int>> m2(data.data());
    assert((m2[0, 1] == 2));

    // 自定义 checked accessor
    // 构造签名: mdspan(handle, mapping, accessor) —— 静态 extents 可用默认 mapping
    data = {1, 2, 3, 4, 5, 6};
    checked_accessor<int> acc{data.size()};
    using Map = std::layout_right::mapping<Ext>;
    std::mdspan<int, Ext, std::layout_right, checked_accessor<int>> mc(data.data(), Map{}, acc);
    mc[1, 0] = 99;
    assert(data[3] == 99);  // layout_right: [1,0] → 1*3+0
    assert((mc[0, 2] == 3));

    // 只读 default_accessor
    std::mdspan<const int, Ext> ro(data.data());
    assert((ro[1, 2] == 6));

    std::cout << "[accessor] default + checked custom policy OK\n";
    std::cout << "mdspan_accessor_policies_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_accessor_policies_cpp23", run>;

}  // namespace
