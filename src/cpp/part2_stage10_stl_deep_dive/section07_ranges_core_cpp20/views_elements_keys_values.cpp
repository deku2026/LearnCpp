// Topic     : views::elements / keys / values —— 取 tuple 分量 / map 键值
// Doc       : 第2部分-阶段10 · 步骤 11.2
// cppreference: https://en.cppreference.com/cpp/ranges/elements_view
//
// 要点: keys≡elements<0>; values≡elements<1>; 适 pair/tuple/map。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [views_elements_keys_values] ===\n";

    std::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}};

    std::vector<std::string> keys;
    for (auto&& k : m | std::views::keys) keys.push_back(k);
    assert((keys == std::vector<std::string>{"a", "b", "c"}));

    [[maybe_unused]] int sum = 0;
    for (int v : m | std::views::values | std::views::filter([](int x) { return x > 1; })) sum += v;
    assert(sum == 5);  // 2+3

    // elements<N> 于 tuple 向量
    std::vector<std::tuple<int, char, double>> rows{{1, 'x', 0.5}, {2, 'y', 1.5}};
    std::vector<char> mid;
    for (char c : rows | std::views::elements<1>) mid.push_back(c);
    assert((mid == std::vector<char>{'x', 'y'}));

    // pair 向量
    std::vector<std::pair<int, int>> pts{{0, 1}, {2, 3}};
    std::vector<int> xs(std::ranges::begin(pts | std::views::keys), std::ranges::end(pts | std::views::keys));
    assert((xs == std::vector<int>{0, 2}));

    std::cout << "[elements/keys/values] map + tuple OK\n";
    std::cout << "views_elements_keys_values: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_elements_keys_values", run>;

}  // namespace
