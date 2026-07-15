// Topic     : mdspan extents：静态 / 动态 / 混合维度 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 8.2
// cppreference: https://en.cppreference.com/cpp/container/mdspan/extents
//
// 要点: extents 描述几维与每维大小; static / dynamic_extent / dextents。

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
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_extents_cpp23", run>;
}  // namespace
#else

#include <cassert>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mdspan_extents_cpp23] ===\n";

    // 静态: 编译期固定, 零运行期尺寸开销
    using StaticE = std::extents<std::size_t, 3, 4>;
    [[maybe_unused]] StaticE e_static{};
    static_assert(StaticE::rank() == 2);
    static_assert(StaticE::static_extent(0) == 3);
    static_assert(StaticE::static_extent(1) == 4);
    assert(e_static.extent(0) == 3 && e_static.extent(1) == 4);

    // 动态: dextents = 全 dynamic
    [[maybe_unused]] std::dextents<std::size_t, 2> e_dyn{5, 6};
    assert(e_dyn.rank() == 2);
    assert(e_dyn.extent(0) == 5 && e_dyn.extent(1) == 6);
    static_assert(std::dextents<std::size_t, 2>::static_extent(0) == std::dynamic_extent);

    // 混合: 部分静态 + 部分动态
    [[maybe_unused]] std::extents<std::size_t, 3, std::dynamic_extent> e_mix{7};  // 3 行, 7 列
    assert(e_mix.extent(0) == 3 && e_mix.extent(1) == 7);
    static_assert(std::extents<std::size_t, 3, std::dynamic_extent>::static_extent(0) == 3);

    // 接 mdspan
    std::vector<int> data(3 * 4, 0);
    std::mdspan<int, StaticE> m(data.data());
    assert(m.rank() == 2);
    assert(m.extent(0) == 3 && m.extent(1) == 4);
    m[1, 2] = 42;
    assert(data[1 * 4 + 2] == 42);

    // CTAD 动态
    std::vector<int> d2(2 * 3);
    auto m2 = std::mdspan(d2.data(), 2, 3);
    assert(m2.extent(0) == 2 && m2.extent(1) == 3);

    std::cout << "[extents] static 3x4, dynamic 5x6, mixed 3x7, mdspan OK\n";
    std::cout << "mdspan_extents_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_extents_cpp23", run>;

}  // namespace
#endif  // LEARNCPP_HAS_MDSPAN
