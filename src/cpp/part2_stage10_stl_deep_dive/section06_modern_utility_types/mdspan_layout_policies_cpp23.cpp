// Topic     : mdspan layout policy：layout_right / left / stride
// Doc       : 第2部分-阶段10 · 步骤 8.3
// cppreference: https://en.cppreference.com/cpp/container/mdspan/layout_right
//
// 要点: 行主序 vs 列主序同一块内存不同映射; layout_stride 自定义步长。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <mdspan>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mdspan_layout_policies_cpp23] ===\n";

    // 填 0..11 的一维缓冲
    std::vector<int> data(12);
    for (int i = 0; i < 12; ++i) data[static_cast<std::size_t>(i)] = i;

    using Ext = std::extents<std::size_t, 3, 4>;

    // layout_right (默认, 行主序): m[i,j] = i*ncols + j
    // assert 是函数式宏，含逗号的表达式必须再包一层 ()，否则多维下标被拆成多个宏实参
    std::mdspan<int, Ext, std::layout_right> row(data.data());
    assert((row[1, 2] == 1 * 4 + 2));  // 6
    assert((row[0, 0] == 0 && row[2, 3] == 11));

    // layout_left (列主序): m[i,j] = j*nrows + i
    std::mdspan<int, Ext, std::layout_left> col(data.data());
    assert((col[1, 2] == 2 * 3 + 1));  // 7
    // 同一 [1,2] 落在不同内存!
    assert((row[1, 2] != col[1, 2]));

    std::cout << "[layout] row[1,2]=" << row[1, 2] << " col[1,2]=" << col[1, 2] << " (different offsets)\n";

    // layout_stride: 例如只看偶数列 (stride 行=4, 列=2) 的 3x2 视图
    // 原 3x4 行主序, 取列 0,2
    std::array<std::size_t, 2> strides{4, 2};
    std::layout_stride::mapping<std::dextents<std::size_t, 2>> map{std::dextents<std::size_t, 2>{3, 2}, strides};
    std::mdspan<int, std::dextents<std::size_t, 2>, std::layout_stride> sub(data.data(), map);
    // sub[i,j] 对应 row[i, j*2]
    assert((sub[0, 0] == row[0, 0]));
    assert((sub[0, 1] == row[0, 2]));
    assert((sub[1, 1] == row[1, 2]));

    std::cout << "[layout_stride] every-other-column view OK\n";
    std::cout << "mdspan_layout_policies_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_layout_policies_cpp23", run>;

}  // namespace
