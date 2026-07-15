// Topic     : mdspan 矩阵访问 + 对称判断 + 选型对照 (验收点)
// Doc       : 第2部分-阶段10 · 步骤 8.6 / 8.7
// cppreference: https://en.cppreference.com/cpp/container/mdspan
//
// 要点: m[i,j] 多维下标; 对称性; vs 手写索引 / Eigen 定位。

#include "learn/topic_registry.hpp"

#if defined(__has_include)
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARNCPP_HAS_MDSPAN 1
#endif
#endif
#ifndef LEARNCPP_HAS_MDSPAN
#define LEARNCPP_HAS_MDSPAN 0
#endif

#if !LEARNCPP_HAS_MDSPAN
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] <mdspan> not available on this standard library\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_versus_alternatives_cpp23", run>;
}  // namespace
#else

#include <cassert>
#include <iostream>
#include <vector>

namespace {

[[maybe_unused]] bool is_symmetric(std::mdspan<const int, std::dextents<std::size_t, 2>> m) {
    if (m.extent(0) != m.extent(1)) return false;
    for (std::size_t i = 0; i < m.extent(0); ++i)
        for (std::size_t j = 0; j < m.extent(1); ++j)
            if (m[i, j] != m[j, i]) return false;
    return true;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mdspan_versus_alternatives_cpp23] ===\n";

    // 一维存储当 3×3 对称矩阵
    std::vector<int> data{1, 2, 3, 2, 4, 5, 3, 5, 6};
    auto m = std::mdspan(data.data(), 3, 3);

    std::cout << "[matrix]\n";
    for (std::size_t i = 0; i < m.extent(0); ++i) {
        std::cout << "  ";
        for (std::size_t j = 0; j < m.extent(1); ++j) std::cout << m[i, j] << ' ';
        std::cout << '\n';
    }
    assert(is_symmetric(m));
    // assert 是宏：多维下标中的逗号需额外括号保护
    assert((m[0, 2] == 3 && m[2, 0] == 3));

    // 非对称
    std::vector<int> asym{1, 2, 3, 4};
    auto a = std::mdspan(asym.data(), 2, 2);
    assert(!is_symmetric(a));

    // 手写索引对照(易错, 布局写死)
    [[maybe_unused]] const int cols = 3;
    assert((data[1 * cols + 2] == m[1, 2]));

    // 免拷贝"转置视图": 同一 data, layout_left 看 3x3
    // 原 layout_right 的转置 ≡ layout_left 解读同一缓冲
    using Ext = std::extents<std::size_t, 3, 3>;
    std::mdspan<int, Ext, std::layout_left> transposed(data.data());
    // transposed[i,j] 读的是原 row-major 的 [j,i] 位置语义演示:
    // layout_left[i,j] 偏移 = j*3+i, 恰为 layout_right 的 [j,i]
    assert((transposed[0, 2] == data[2 * 3 + 0]));
    assert((transposed[0, 2] == m[2, 0]));

    // 选型一句话:
    // 手写 i*cols+j — 轻但易错、布局写死
    // mdspan — 标准零拷贝、可换布局
    // Eigen/Kokkos — 全功能线性代数/GPU, 更重
    std::cout << "[vs] hand-index / mdspan / Eigen: pick by weight & need\n";
    std::cout << "mdspan_versus_alternatives_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_versus_alternatives_cpp23", run>;

}  // namespace
#endif  // LEARNCPP_HAS_MDSPAN
