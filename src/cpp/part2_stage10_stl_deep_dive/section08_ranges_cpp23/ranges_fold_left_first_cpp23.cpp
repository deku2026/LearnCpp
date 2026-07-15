// Topic     : ranges::fold_left_first —— 首元素作初值 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.3
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/fold_left_first
//
// 要点: 返回 optional; 空范围 → nullopt。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_fold_left_first_cpp23] ===\n";

    std::vector<int> v{1, 2, 3, 4, 5};

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
    [[maybe_unused]] auto maxv = std::ranges::fold_left_first(v, [](int a, int b) { return a > b ? a : b; });
    assert(maxv.has_value() && *maxv == 5);

    [[maybe_unused]] auto minv = std::ranges::fold_left_first(v, [](int a, int b) { return a < b ? a : b; });
    assert(minv && *minv == 1);

    [[maybe_unused]] auto sum = std::ranges::fold_left_first(v, std::plus{});
    assert(sum && *sum == 15);

    [[maybe_unused]] auto empty = std::ranges::fold_left_first(std::vector<int>{}, std::plus{});
    assert(!empty);

    // 单元素
    [[maybe_unused]] auto one = std::ranges::fold_left_first(std::vector{42}, std::plus{});
    assert(one && *one == 42);

    std::cout << "[fold_left_first] library path OK\n";
#else
    std::cout << "library not yet available, fallback demo (accumulate)\n";
    auto fold_first = [](const std::vector<int>& r, auto op) -> std::optional<int> {
        if (r.empty()) return std::nullopt;
        return std::accumulate(std::next(r.begin()), r.end(), r.front(), op);
    };

    auto maxv = fold_first(v, [](int a, int b) { return a > b ? a : b; });
    assert(maxv.has_value() && *maxv == 5);

    auto minv = fold_first(v, [](int a, int b) { return a < b ? a : b; });
    assert(minv && *minv == 1);

    auto sum = fold_first(v, std::plus{});
    assert(sum && *sum == 15);

    auto empty = fold_first(std::vector<int>{}, std::plus{});
    assert(!empty);

    auto one = fold_first(std::vector{42}, std::plus{});
    assert(one && *one == 42);

    std::cout << "[fold_left_first] fallback OK\n";
#endif

    std::cout << "ranges_fold_left_first_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_fold_left_first_cpp23", run>;

}  // namespace
