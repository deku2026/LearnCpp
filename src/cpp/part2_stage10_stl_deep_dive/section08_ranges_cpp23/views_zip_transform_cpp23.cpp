// Topic     : views::zip_transform —— 并行打包并立即变换 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.1
// cppreference: https://en.cppreference.com/cpp/ranges/zip_transform_view
//
// 要点: 等价 zip 后 transform, 但不经中间 tuple(概念上)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_zip_transform_cpp23] ===\n";

    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};

    std::vector<int> sums;
    for (int s : std::views::zip_transform(std::plus{}, a, b)) sums.push_back(s);
    assert((sums == std::vector<int>{11, 22, 33}));

    // 自定义: 拼字符串
    std::vector<std::string> names{"x", "y"};
    std::vector<int> ids{1, 2};
    std::vector<std::string> labels;
    for (auto s : std::views::zip_transform([](const std::string& n, int id) { return n + "#" + std::to_string(id); },
                                            names, ids))
        labels.push_back(s);
    assert(labels[0] == "x#1" && labels[1] == "y#2");

    // 三参数
    std::vector<int> c{1, 1, 1};
    int prod_sum = 0;
    for (int v : std::views::zip_transform([](int x, int y, int z) { return x * y + z; }, a, b, c)) prod_sum += v;
    assert(prod_sum == (1 * 10 + 1) + (2 * 20 + 1) + (3 * 30 + 1));

    std::cout << "[zip_transform] plus/custom/3-way OK\n";
    std::cout << "views_zip_transform_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_zip_transform_cpp23", run>;

}  // namespace
