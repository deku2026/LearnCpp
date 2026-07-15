// Topic     : ranges::fold_right_last —— 末元素作初值的右折 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/fold_right_last
//
// 要点: 对称于 fold_left_first; 返回 optional。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <optional>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_fold_right_last_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    auto sum = std::ranges::fold_right_last(v, std::plus{});
    assert(sum && *sum == 15);

    // 非结合: 1-(2-(3-(4-5))) = 1-(2-(3-(-1))) = 1-(2-4) = 1-(-2) = 3
    auto nested = std::ranges::fold_right_last(v, std::minus{});
    assert(nested && *nested == 3);

    auto empty = std::ranges::fold_right_last(std::vector<int>{}, std::plus{});
    assert(!empty);

    auto one = std::ranges::fold_right_last(std::vector{7}, std::plus{});
    assert(one && *one == 7);

    std::cout << "[fold_right_last] library path OK\n";
#else
    std::cout << "library not yet available, fallback demo\n";
    auto fold_right_last = [](const std::vector<int>& r, auto op) -> std::optional<int> {
        if (r.empty()) return std::nullopt;
        int acc = r.back();
        for (auto it = std::next(r.rbegin()); it != r.rend(); ++it) acc = op(*it, acc);
        return acc;
    };

    auto sum = fold_right_last(v, std::plus{});
    assert(sum && *sum == 15);

    auto nested = fold_right_last(v, std::minus{});
    assert(nested && *nested == 3);

    auto empty = fold_right_last(std::vector<int>{}, std::plus{});
    assert(!empty);

    auto one = fold_right_last(std::vector{7}, std::plus{});
    assert(one && *one == 7);

    std::cout << "[fold_right_last] fallback OK\n";
#endif

    std::cout << "ranges_fold_right_last_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_right_last_cpp23", run>;

}  // namespace
