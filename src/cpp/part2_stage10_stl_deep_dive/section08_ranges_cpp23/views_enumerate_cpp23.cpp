// Topic     : views::enumerate —— 带下标遍历 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 12.1
// cppreference: https://en.cppreference.com/cpp/ranges/enumerate_view
//
// 要点: 产出 (index, value); 常与 zip 组合。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_enumerate_cpp23] ===\n";

    std::vector<std::string> names{"Alice", "Bob", "Carol"};

    std::vector<std::string> lines;
    for (auto [i, name] : std::views::enumerate(names)) {
        lines.push_back("[" + std::to_string(i) + "] " + name);
    }
    assert(lines[0] == "[0] Alice");
    assert(lines[2] == "[2] Carol");

    // enumerate + zip
    std::vector<int> ages{30, 25, 35};
    int bob_i = -1;
    for (auto [i, pair] : std::views::enumerate(std::views::zip(names, ages))) {
        auto [name, age] = pair;
        if (name == "Bob") {
            bob_i = static_cast<int>(i);
            assert(age == 25);
        }
    }
    assert(bob_i == 1);

    // 管道形式
    int last = -1;
    for (auto [i, x] : std::views::iota(10, 13) | std::views::enumerate) last = static_cast<int>(i);
    assert(last == 2);

    std::cout << "[enumerate] index+value + zip combo OK\n";
    std::cout << "views_enumerate_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_enumerate_cpp23", run>;

}  // namespace
