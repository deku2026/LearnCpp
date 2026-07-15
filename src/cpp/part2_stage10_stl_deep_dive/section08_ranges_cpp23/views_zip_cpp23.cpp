// Topic     : views::zip —— 并行打包多个范围 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.1
// cppreference: https://en.cppreference.com/cpp/ranges/zip_view
//
// 要点: 产出 tuple; 长度取最短; category 取最弱。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_zip_cpp23] ===\n";

    std::vector<std::string> names{"Alice", "Bob", "Carol"};
    std::vector<int> ages{30, 25, 35};

    std::vector<std::string> lines;
    for (auto [name, age] : std::views::zip(names, ages)) {
        lines.push_back(name + ":" + std::to_string(age));
    }
    assert(lines.size() == 3);
    assert(lines[0] == "Alice:30" && lines[1] == "Bob:25");

    // 最短截断
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{10, 20};
    int n = 0;
    for (auto [x, y] : std::views::zip(a, b)) {
        (void)x;
        (void)y;
        ++n;
    }
    assert(n == 2);

    // 三路
    std::vector<char> tags{'A', 'B', 'C'};
    int sum = 0;
    for (auto [age, tag] : std::views::zip(ages, tags)) {
        if (tag == 'B') assert(age == 25);
        sum += age;
    }
    assert(sum == 30 + 25 + 35);

    std::cout << "[zip] parallel pack + min-length OK\n";
    std::cout << "views_zip_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_zip_cpp23", run>;

}  // namespace
