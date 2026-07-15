// Topic     : ranges::fold_right —— 右折叠 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/fold_right
//
// 要点: 从右结合; 对非交换 op 与 fold_left 结果不同。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_fold_right_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    // 减法: 左折 ((1-2)-3)-4 = -8; 右折 1-(2-(3-4)) = 1-(2-(-1)) = 1-3 = -2
    const int left_sub = std::ranges::fold_left(v, 0, std::minus{});  // 0-1-2-3-4 = -10 with init
    const int right_sub = std::ranges::fold_right(v, 0, std::minus{});
    // fold_right(v, 0, -) = 1-(2-(3-(4-0))) = 1-(2-(3-4)) = 1-(2-(-1)) = -2
    assert(right_sub == -2);
    (void)left_sub;

    const int sum = std::ranges::fold_right(v, 0, std::plus{});
    assert(sum == 10);

    // 字符串右折: 展示结合方向
    std::vector<std::string> parts{"A", "B", "C"};
    auto right_cat = std::ranges::fold_right(parts, std::string{""},
                                             [](const std::string& a, const std::string& b) { return a + b; });
    // fold_right: op(A, op(B, op(C, ""))) = op(A, op(B, C)) = op(A, BC) = ABC
    assert(right_cat == "ABC");

    std::cout << "[fold_right] non-assoc minus=" << right_sub << " sum=" << sum << '\n';
#else
    std::cout << "library not yet available, fallback demo (rbegin accumulate)\n";
    // 手写右折: 从右往左用 reverse_iterator 模拟
    auto fold_right = [](const std::vector<int>& r, int init, auto op) {
        int acc = init;
        for (auto it = r.rbegin(); it != r.rend(); ++it) acc = op(*it, acc);
        return acc;
    };
    const int right_sub = fold_right(v, 0, std::minus{});
    assert(right_sub == -2);

    const int sum = fold_right(v, 0, std::plus{});
    assert(sum == 10);

    std::vector<std::string> parts{"A", "B", "C"};
    std::string right_cat;
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) right_cat = *it + right_cat;
    assert(right_cat == "ABC");

    std::cout << "[fold_right] fallback minus=" << right_sub << " sum=" << sum << '\n';
#endif

    std::cout << "ranges_fold_right_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_right_cpp23", run>;

}  // namespace
