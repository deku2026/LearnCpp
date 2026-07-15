// Topic     : ranges::iota 算法版 —— 写入递增序列 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.2
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/iota
//
// 要点: 区别于 views::iota(生成视图); 本算法就地填充输出范围。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_iota_algorithm_cpp23] ===\n";

    std::vector<int> buf(5);

#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
    [[maybe_unused]] auto result = std::ranges::iota(buf, 10);
    assert((buf == std::vector<int>{10, 11, 12, 13, 14}));
    // 返回 out_value_result: out 迭代器 + 下一 value
    assert(result.out == buf.end());
    assert(result.value == 15);

    std::vector<int> small(3);
    [[maybe_unused]] auto r2 = std::ranges::iota(small, 0);
    assert((small == std::vector<int>{0, 1, 2}));
    assert(r2.value == 3);

    std::cout << "[ranges::iota algo] library path OK\n";
#else
    std::cout << "library not yet available, fallback demo (std::iota)\n";
    std::iota(buf.begin(), buf.end(), 10);
    assert((buf == std::vector<int>{10, 11, 12, 13, 14}));
    const int next_value = 15;
    assert(next_value == 15);

    std::vector<int> small(3);
    std::iota(small.begin(), small.end(), 0);
    assert((small == std::vector<int>{0, 1, 2}));

    std::cout << "[ranges::iota algo] fallback OK\n";
#endif

    // 对比 views::iota: 不写缓冲, 惰性
    [[maybe_unused]] auto view = std::views::iota(10, 15);
    assert(std::ranges::equal(view, buf));

    std::cout << "ranges_iota_algorithm_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_iota_algorithm_cpp23", run>;

}  // namespace
