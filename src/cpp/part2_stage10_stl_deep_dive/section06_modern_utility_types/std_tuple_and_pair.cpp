// Topic     : std::tuple / std::pair —— 固定异类聚合
// Doc       : 第2部分-阶段10 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/utility/tuple
//
// 要点: 结构化绑定; get<I>; tie; tuple_size; pair 是二元特例。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

namespace {

std::pair<int, std::string> id_name() {
    return {7, "Ada"};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_tuple_and_pair] ===\n";

    auto t = std::make_tuple(1, 2.5, std::string{"three"});
    assert(std::get<0>(t) == 1);
    assert(std::get<1>(t) == 2.5);
    assert(std::get<2>(t) == "three");
    static_assert(std::tuple_size_v<decltype(t)> == 3);

    auto [a, b, c] = t;
    assert(a == 1 && b == 2.5 && c == "three");

    // tie 解包到已有变量(可 ignore)
    int x = 0;
    double y = 0;
    std::string z;
    std::tie(x, y, z) = t;
    assert(x == 1 && z == "three");

    int only_first = 0;
    std::tie(only_first, std::ignore, std::ignore) = t;
    assert(only_first == 1);

    // pair
    std::pair<int, std::string> p{1, "one"};
    assert(p.first == 1 && p.second == "one");
    auto [id, name] = id_name();
    assert(id == 7 && name == "Ada");

    // tuple 比较: 字典序（assert 是宏，逗号需额外括号）
    assert((std::tuple{1, 2} < std::tuple{1, 3}));
    assert((std::pair{1, 2} == std::pair{1, 2}));

    // cat / forward_as_tuple
    auto t2 = std::tuple_cat(std::tuple{1}, std::tuple{2, 3});
    assert(std::get<2>(t2) == 3);

    std::cout << "[tuple/pair] get/structured-binding/tie/cat OK\n";
    std::cout << "std_tuple_and_pair: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_tuple_and_pair", run>;

}  // namespace
