// Topic     : mdspan mapping：多维下标 → 一维偏移
// Doc       : 第2部分-阶段10 · 步骤 8.4
// cppreference: https://en.cppreference.com/cpp/container/mdspan/layout_mapping
//
// 要点: mapping 由 layout+extents 产生; operator() 算偏移; required_span_size。

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
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_mapping_cpp23", run>;
}  // namespace
#else

#include <cassert>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mdspan_mapping_cpp23] ===\n";

    using Ext = std::extents<std::size_t, 3, 4>;
    Ext ext{};

    std::layout_right::mapping<Ext> map_r(ext);
    assert(map_r(1, 2) == 6);  // 1*4+2
    assert(map_r(2, 3) == 11);
    assert(map_r.required_span_size() == 12);
    assert(map_r.is_exhaustive());
    assert(map_r.stride(0) == 4 && map_r.stride(1) == 1);

    std::layout_left::mapping<Ext> map_l(ext);
    (void)map_l;
    assert(map_l(1, 2) == 7);  // 2*3+1
    assert(map_l.required_span_size() == 12);
    assert(map_l.stride(0) == 1 && map_l.stride(1) == 3);

    // 用 mapping 构造 mdspan
    std::vector<int> data(map_r.required_span_size());
    std::mdspan<int, Ext, std::layout_right> m(data.data(), map_r);
    m[1, 2] = 99;
    assert(data[map_r(1, 2)] == 99);

    // 动态 extents 的 mapping
    std::dextents<std::size_t, 2> de{2, 5};
    std::layout_right::mapping<std::dextents<std::size_t, 2>> map_d(de);
    assert(map_d(1, 4) == 9);
    assert(map_d.required_span_size() == 10);

    std::cout << "[mapping] right(1,2)=6 left(1,2)=7 size=12 OK\n";
    std::cout << "mdspan_mapping_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_mapping_cpp23", run>;

}  // namespace
#endif  // LEARNCPP_HAS_MDSPAN
