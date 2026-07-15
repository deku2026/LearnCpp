// Topic     : ranges::fold_left_with_iter —— 折叠并返回结束迭代器 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/fold_left_with_iter
//
// 要点: 返回 in_value_result{in, value}; 便于部分折叠。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_fold_left_with_iter_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    auto [it, sum] = std::ranges::fold_left_with_iter(v, 0, std::plus{});
    assert(it == v.end());
    assert(sum == 15);

    // 子范围折叠
    auto [it2, partial] = std::ranges::fold_left_with_iter(v.begin(), v.begin() + 3, 0, std::plus{});
    assert(it2 == v.begin() + 3);
    assert(partial == 6);

    // 空
    auto [it3, z] = std::ranges::fold_left_with_iter(v.end(), v.end(), 99, std::plus{});
    assert(it3 == v.end() && z == 99);

    std::cout << "[fold_left_with_iter] library path OK\n";
#else
    std::cout << "library not yet available, fallback demo (accumulate)\n";
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    assert(sum == 15);
    auto it = v.end();
    assert(it == v.end());

    const int partial = std::accumulate(v.begin(), v.begin() + 3, 0);
    assert(partial == 6);
    auto it2 = v.begin() + 3;
    assert(it2 == v.begin() + 3);

    const int z = 99;
    auto it3 = v.end();
    assert(it3 == v.end() && z == 99);

    std::cout << "[fold_left_with_iter] fallback OK\n";
#endif

    std::cout << "ranges_fold_left_with_iter_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_left_with_iter_cpp23", run>;

}  // namespace
